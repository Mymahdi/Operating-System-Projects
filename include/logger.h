#ifndef LOGGER_H
#define LOGGER_H

#include <string>

// Initialize the logger with a file name
void initLogger(const std::string& logFileName);

// Log a message with INFO level
void logInfo(const std::string& message);

// Log a message with ERROR level
void logError(const std::string& message);

// Close the logger
void closeLogger();

#endif // LOGGER_H
