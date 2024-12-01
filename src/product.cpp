#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "utils.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <product name> <parts file>" << std::endl;
        return 1;
    }

    std::string productName = argv[1];
    std::string partsFile = argv[2];

    // Parse the parts file
    std::vector<PartsData> parts = parsePartsFile(partsFile);

    // Process data for the specific product
    ProductReport report = generateProductReport(productName, parts);

    // Output the report
    std::cout << report.toString() << std::endl;

    return 0;
}
