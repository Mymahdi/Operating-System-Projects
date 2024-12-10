#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>

namespace fs = std::filesystem;

void spawnMapperProcesses(const std::vector<std::string>& warehouseFiles, int writePipe[]);
void spawnReducerProcess(int readPipe[]);

int main() {
    std::vector<std::string> warehouseFiles;

    for (const auto& entry : fs::directory_iterator("stores")) {
        if (entry.path().extension() == ".csv" && entry.path().filename() != "Parts.csv") {
            warehouseFiles.push_back(entry.path().string());
        }
    }

    if (warehouseFiles.empty()) {
        std::cerr << "No warehouse CSV files found in 'stores' directory." << std::endl;
        return 1;
    }

    int mapperPipe[2];

    if (pipe(mapperPipe) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    spawnMapperProcesses(warehouseFiles, mapperPipe);
    spawnReducerProcess(mapperPipe);

    close(mapperPipe[0]); 
    close(mapperPipe[1]); 

    while (wait(NULL) > 0); 

    return 0;
}

void spawnMapperProcesses(const std::vector<std::string>& warehouseFiles, int writePipe[]) {
    for (const auto& file : warehouseFiles) {
        pid_t pid = fork();
        if (pid == 0) {
            close(writePipe[0]);
            dup2(writePipe[1], STDOUT_FILENO);
            close(writePipe[1]);

            execl("./MapperProcess", "./MapperProcess", file.c_str(), NULL);
            perror("execl failed for MapperProcess");
            exit(1);
        }
    }
    close(writePipe[1]); 
}

void spawnReducerProcess(int readPipe[]) {
    pid_t pid = fork();
    if (pid == 0) {
        close(readPipe[1]);
        dup2(readPipe[0], STDIN_FILENO); 
        close(readPipe[0]);

        execl("./ReducerProcess", "./ReducerProcess", NULL);
        perror("execl failed for ReducerProcess");
        exit(1);
    }
    close(readPipe[0]);
}
