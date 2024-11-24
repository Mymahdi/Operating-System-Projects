#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "utils.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <warehouse file> <pipe write fd>" << std::endl;
        return 1;
    }

    std::string warehouseFile = argv[1];
    int pipeFD = std::stoi(argv[2]);

    // Parse the warehouse file
    std::vector<InventoryData> inventory = parseWarehouseFile(warehouseFile);

    // Calculate remaining stock and profits
    std::vector<ProductData> results = calculateWarehouseData(inventory);

    // Send data to parent process
    std::string serializedData = serializeData(results);
    write(pipeFD, serializedData.c_str(), serializedData.size());
    close(pipeFD);

    return 0;
}
