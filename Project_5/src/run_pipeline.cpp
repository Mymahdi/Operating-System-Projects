#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./run_pipeline <input_csv1> <input_csv2> ..." << std::endl;
        return 1;
    }

    int pipe1[2], pipe2[2], pipe3[2];
    pipe(pipe1); pipe(pipe2); pipe(pipe3);

    pid_t merger = fork();
    if (merger == 0) {
       
        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[0]);
        close(pipe1[1]);

        for (int i = 1; i < argc; ++i) {
            pid_t extractor = fork();
            if (extractor == 0) {
                execl("../bin/extract_transform", "extract_transform", argv[i], nullptr);
                perror("exec extract_transform");
                exit(1);
            }
            waitpid(extractor, NULL, 0); 
        }
        exit(0);
    }

    pid_t p2 = fork();
    if (p2 == 0) {
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[0]); close(pipe2[1]);
        execl("../bin/load_balancer", "load_balancer", nullptr);
        perror("exec load_balancer");
        exit(1);
    }

    pid_t p3 = fork();
    if (p3 == 0) {
        dup2(pipe2[0], STDIN_FILENO);
        dup2(pipe3[1], STDOUT_FILENO);
        close(pipe2[0]); close(pipe2[1]);
        close(pipe3[0]); close(pipe3[1]);
        execl("../bin/grid_node", "grid_node", nullptr);
        perror("exec grid_node");
        exit(1);
    }

    pid_t p4 = fork();
    if (p4 == 0) {
        dup2(pipe3[0], STDIN_FILENO);
        close(pipe3[0]); close(pipe3[1]);
        execl("../bin/final_output", "final_output", nullptr);
        perror("exec final_output");
        exit(1);
    }

    close(pipe1[0]); close(pipe1[1]);
    close(pipe2[0]); close(pipe2[1]);
    close(pipe3[0]); close(pipe3[1]);

    waitpid(merger, NULL, 0);
    waitpid(p2, NULL, 0);
    waitpid(p3, NULL, 0);
    waitpid(p4, NULL, 0);

    return 0;
}