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

    // Collect warehouse files
    for (const auto& entry : fs::directory_iterator("stores")) {
        if (entry.path().extension() == ".csv" && entry.path().filename() != "Parts.csv") {
            warehouseFiles.push_back(entry.path().string());
        }
    }

    if (warehouseFiles.empty()) {
        logMessage("No warehouse CSV files found in 'stores' directory.");
        std::cerr << "No warehouse CSV files found in 'stores' directory." << std::endl;
        return 1;
    }

    int mapperPipe[2];

    if (pipe(mapperPipe) == -1) {
        perror("Pipe creation failed");
        logMessage("Failed to create pipe for mapper processes.");
        return 1;
    }
    logMessage("Pipe for mapper processes created successfully.");

    spawnMapperProcesses(warehouseFiles, mapperPipe);
    spawnReducerProcess(mapperPipe);

    close(mapperPipe[0]);
    logMessage("Mapper pipe read end closed in main.");
    close(mapperPipe[1]);
    logMessage("Mapper pipe write end closed in main.");

    while (wait(NULL) > 0);
    logMessage("All child processes have terminated.");

    return 0;
}

void logMessage(const std::string& message) {
    // Write to log file
    std::ofstream logFile("process_log.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << message << std::endl;
        logFile.close();
    }

    // Also write to the terminal
    std::cout << message << std::endl;
}

void spawnMapperProcesses(const std::vector<std::string>& warehouseFiles, int writePipe[]) {
    for (const auto& file : warehouseFiles) {
        pid_t pid = fork();
        if (pid == 0) {
            close(writePipe[0]);
            logMessage("Mapper process: Read end of the pipe closed.");
            dup2(writePipe[1], STDOUT_FILENO);
            close(writePipe[1]);
            logMessage("Mapper process: Write end of the pipe duplicated to STDOUT.");

            execl("./MapperProcess", "./MapperProcess", file.c_str(), NULL);
            perror("execl failed for MapperProcess");
            logMessage("execl failed for MapperProcess: " + file);
            exit(1);
        }
        logMessage("Mapper process spawned for file: " + file);
    }
    close(writePipe[1]);
    logMessage("Parent process: Write end of the mapper pipe closed after spawning mapper processes.");
}

void spawnReducerProcess(int readPipe[]) {
    pid_t pid = fork();
    if (pid == 0) {
        close(readPipe[1]);
        logMessage("Reducer process: Write end of the pipe closed.");
        dup2(readPipe[0], STDIN_FILENO);
        close(readPipe[0]);
        logMessage("Reducer process: Read end of the pipe duplicated to STDIN.");

        execl("./ReducerProcess", "./ReducerProcess", NULL);
        perror("execl failed for ReducerProcess");
        logMessage("execl failed for ReducerProcess.");
        exit(1);
    }
    logMessage("Reducer process spawned.");
    close(readPipe[0]);
    logMessage("Parent process: Read end of the reducer pipe closed.");
}
