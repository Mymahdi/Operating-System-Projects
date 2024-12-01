#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: MapperProcess <warehouse_file>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << argv[1] << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string productName, operation;
        double price;
        int quantity;

        std::getline(ss, productName, ',');
        ss >> price;
        ss.ignore();
        ss >> quantity;
        ss.ignore();
        ss >> operation;

        if (operation == "input" || operation == "output") {
            // Emit key-value pair
            std::cout << productName << "," << price << "," << quantity << "," << operation << std::endl;
        }
    }

    file.close();
    return 0;
}
