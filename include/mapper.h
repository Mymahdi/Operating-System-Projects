#ifndef MAPPER_H
#define MAPPER_H

#include <string>

// Mapper function that processes a single CSV file
void processWarehouse(const std::string& filePath, int writePipe);

#endif // MAPPER_H
