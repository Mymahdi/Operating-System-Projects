#include "mapper.h"
#include "csv_parser.h"
#include <sstream>
#include <unistd.h>
#include "logger.h"

void processWarehouse(const std::string& filePath, int writePipe) {
    logInfo("Processing warehouse file: " + filePath);
    auto data = parseWarehouseFile(filePath);

    std::stringstream ss;
    for (const auto& [product, stats] : data) {
        ss << product << "," << stats.first << "," << stats.second << ";";
    }

    std::string serializedData = ss.str();
    write(writePipe, serializedData.c_str(), serializedData.size());
    close(writePipe);
}
