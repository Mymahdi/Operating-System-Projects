#include "logger.h"
#include <fstream>
#include <iostream>
#include <ctime>

std::ofstream logFile;

std::string getCurrentTime() {
    std::time_t now = std::time(nullptr);
    char buf[80];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

void initLogger(const std::string& logFileName) {
    logFile.open(logFileName, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error: Could not open log file: " << logFileName << std::endl;
        exit(1);
    }
    logFile << "========== Program Started at " << getCurrentTime() << " ==========\n";
}

void logInfo(const std::string& message) {
    if (logFile.is_open()) {
        logFile << "[INFO] [" << getCurrentTime() << "] " << message << "\n";
    }
}

void logError(const std::string& message) {
    if (logFile.is_open()) {
        logFile << "[ERROR] [" << getCurrentTime() << "] " << message << "\n";
    }
}

void closeLogger() {
    if (logFile.is_open()) {
        logFile << "========== Program Ended at " << getCurrentTime() << " ==========\n";
        logFile.close();
    }
}
