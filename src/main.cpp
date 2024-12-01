#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem> // C++17 feature for file system operations
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>

namespace fs = std::filesystem;

void spawnMapperProcesses(const std::vector<std::string>& warehouseFiles, int writePipe[]);
void spawnReducerProcess(int readPipe[]);

int main() {
    std::vector<std::string> warehouseFiles;

    // Collect all dynamic CSV files in 'stores' directory except 'Parts.csv'
    for (const auto& entry : fs::directory_iterator("stores")) {
        if (entry.path().extension() == ".csv" && entry.path().filename() != "Parts.csv") {
            warehouseFiles.push_back(entry.path().string());
        }
    }

    if (warehouseFiles.empty()) {
        std::cerr << "No warehouse CSV files found in 'stores' directory." << std::endl;
        return 1;
    }

    int mapperPipe[2]; // Pipe for mapper-to-reducer communication

    if (pipe(mapperPipe) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    // Spawn mapper processes
    spawnMapperProcesses(warehouseFiles, mapperPipe);

    // Spawn reducer process
    spawnReducerProcess(mapperPipe);

    close(mapperPipe[0]); // Close read end in the main process
    close(mapperPipe[1]); // Close write end in the main process

    // Wait for all child processes
    while (wait(NULL) > 0);

    return 0;
}

void spawnMapperProcesses(const std::vector<std::string>& warehouseFiles, int writePipe[]) {
    for (const auto& file : warehouseFiles) {
        pid_t pid = fork();
        if (pid == 0) { // Child process
            close(writePipe[0]); // Close read end in mapper
            dup2(writePipe[1], STDOUT_FILENO); // Redirect stdout to pipe
            close(writePipe[1]);

            execl("./MapperProcess", "./MapperProcess", file.c_str(), NULL);
            perror("execl failed for MapperProcess");
            exit(1);
        }
    }
}

void spawnReducerProcess(int readPipe[]) {
    pid_t pid = fork();
    if (pid == 0) { // Child process
        close(readPipe[1]); // Close write end in reducer
        dup2(readPipe[0], STDIN_FILENO); // Redirect stdin to pipe
        close(readPipe[0]);

        execl("./ReducerProcess", "./ReducerProcess", NULL);
        perror("execl failed for ReducerProcess");
        exit(1);
    }
}
