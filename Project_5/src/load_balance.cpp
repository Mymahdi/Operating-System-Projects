#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>
#include <sstream>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstdlib>
#include <sys/types.h>

using namespace std;
#define NUM_WORKERS 3

void log(const string& msg) {
    cerr << "[LOAD BALANCER] " << msg << endl;
}

struct WorkerPipe {
    int write_fd;
    int read_fd;
};

int main() {
    WorkerPipe workers[NUM_WORKERS];

    for (int i = 0; i < NUM_WORKERS; ++i) {
        int in_pipe[2];  
        int out_pipe[2]; 

        if (pipe(in_pipe) == -1 || pipe(out_pipe) == -1) {
            cerr << "Pipe creation failed for worker " << i << endl;
            exit(1);
        }

        pid_t pid = fork();
        if (pid < 0) {
            cerr << "Fork failed for worker " << i << endl;
            exit(1);
        } else if (pid == 0) {
           
            close(in_pipe[1]); 
            dup2(in_pipe[0], STDIN_FILENO);
            close(in_pipe[0]);

            close(out_pipe[0]); 
            dup2(out_pipe[1], STDOUT_FILENO);
            close(out_pipe[1]);

            log("Worker " + to_string(i) + " starting...");
            execlp("../bin/grid_worker", "grid_worker", nullptr);
            cerr << "Failed to exec worker " << i << endl;
            exit(1);
        } else {
           
            close(in_pipe[0]); 
            close(out_pipe[1]); 

            workers[i].write_fd = in_pipe[1];
            workers[i].read_fd = out_pipe[0];

            log("Forked worker " + to_string(i) + " with PID " + to_string(pid));
        }
    }

    log("Start reading lines from stdin...");

    string line;
    int current_worker = 0;
    int line_count = 0;

    while (getline(cin, line)) {
        line_count++;
        int idx = current_worker % NUM_WORKERS;
        line += '\n';
        write(workers[idx].write_fd, line.c_str(), line.size());
        log("Sent line " + to_string(line_count) + " to worker " + to_string(idx));
        current_worker++;
    }

    for (int i = 0; i < NUM_WORKERS; ++i) {
        close(workers[i].write_fd);
    }

   
    char buffer[1024];
    for (int i = 0; i < NUM_WORKERS; ++i) {
        FILE* pipe_read = fdopen(workers[i].read_fd, "r");
        if (!pipe_read) continue;

        while (fgets(buffer, sizeof(buffer), pipe_read)) {
            cout << buffer;
        }

        fclose(pipe_read);
    }

    for (int i = 0; i < NUM_WORKERS; ++i) {
        wait(nullptr);
    }

    log("Load balancer finished.");
    return 0;
}
