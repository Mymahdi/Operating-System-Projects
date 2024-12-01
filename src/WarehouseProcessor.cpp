#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem> 
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>

namespace fs = std::filesystem;

void logMessage(const std::string& message);

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
        logMessage("No warehouse CSV files found in 'stores' directory.");
        return 1;
    }

    logMessage("Found " + std::to_string(warehouseFiles.size()) + " warehouse CSV files.");

    int mapperPipe[2];

    if (pipe(mapperPipe) == -1) {
        logMessage("Pipe creation failed");
        perror("pipe failed");
        return 1;
    }
    logMessage("Pipe created successfully.");


    spawnMapperProcesses(warehouseFiles, mapperPipe);

    spawnReducerProcess(mapperPipe);

    close(mapperPipe[0]);
    close(mapperPipe[1]);

    logMessage("Closed pipe in the parent process.");
    while (wait(NULL) > 0);

    logMessage("Parent process waiting for all child processes to finish.");

    return 0;
}

void spawnMapperProcesses(const std::vector<std::string>& warehouseFiles, int writePipe[]) {
    for (const auto& file : warehouseFiles) {
        pid_t pid = fork();
        if (pid == 0) { 
            close(writePipe[0]); 
            dup2(writePipe[1], STDOUT_FILENO); 
            close(writePipe[1]);

            logMessage("Mapper process spawned for file: " + file);

            execl("./MapperProcess", "./MapperProcess", file.c_str(), NULL);
            logMessage("execl failed for MapperProcess");
            perror("execl failed");
            exit(1);
        } else if (pid > 0) {
            logMessage("Forked a mapper process with PID: " + std::to_string(pid));
        } else {
            logMessage("Failed to fork mapper process.");
            perror("fork failed");
        }
    }
}

void spawnReducerProcess(int readPipe[]) {
    pid_t pid = fork();
    if (pid == 0) { 
        close(readPipe[1]);
        dup2(readPipe[0], STDIN_FILENO);
        close(readPipe[0]);

        logMessage("Reducer process spawned.");

        execl("./ReducerProcess", "./ReducerProcess", NULL);
        logMessage("execl failed for ReducerProcess");
        perror("execl failed");
        exit(1);
    } else if (pid > 0) {
        logMessage("Forked a reducer process with PID: " + std::to_string(pid));
    } else {
        logMessage("Failed to fork reducer process.");
        perror("fork failed");
    }
}

void logMessage(const std::string& message) {
    time_t now = time(0);
    char* dt = ctime(&now);
    dt[strlen(dt) - 1] = '\0'; 

    std::cerr << "[" << dt << "] " << message << std::endl;
    std::cerr.flush(); 
}
