#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h> // For mkfifo
#include <fcntl.h>    // For open, O_WRONLY
#include <unistd.h>   // For write, close
#include "utils.h"    // For extractProductKey and other utilities
#include <sys/wait.h> // For waitpid

#define MAX_BUFFER 1024

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path to stores folder>" << std::endl;
        return 1;
    }

    std::string storesPath = argv[1];
    std::string partsFile = storesPath + "/parts.csv";

    // Parse the parts file to get the product list
    std::vector<std::string> products = parsePartsFile(partsFile);
    if (products.empty()) {
        std::cerr << "Error: No products found in parts.csv!" << std::endl;
        return 1;
    }

    // Get all warehouse files (dynamic)
    std::vector<std::string> warehouseFiles = listFiles(storesPath, "parts.csv");
    if (warehouseFiles.empty()) {
        std::cerr << "Error: No warehouse files found in the stores folder!" << std::endl;
        return 1;
    }

    // Create pipes for communication with warehouse processes
    int warehousePipes[warehouseFiles.size()][2];
    for (size_t i = 0; i < warehouseFiles.size(); ++i) {
        if (pipe(warehousePipes[i]) == -1) {
            perror("pipe");
            return 1;
        }
    }

    // Fork warehouse processes
    std::vector<pid_t> warehousePIDs;
    for (size_t i = 0; i < warehouseFiles.size(); ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process (warehouse)
            close(warehousePipes[i][0]); // Close reading end
            execl("./warehouse", "warehouse", warehouseFiles[i].c_str(),
                  std::to_string(warehousePipes[i][1]).c_str(), nullptr);
            perror("execl");
            exit(1);
        } else if (pid > 0) {
            // Parent process
            close(warehousePipes[i][1]); // Close writing end
            warehousePIDs.push_back(pid);
        } else {
            perror("fork");
            return 1;
        }
    }

    // Collect data from warehouse processes
    std::vector<std::string> warehouseData;
    for (size_t i = 0; i < warehouseFiles.size(); ++i) {
        char buffer[MAX_BUFFER];
        read(warehousePipes[i][0], buffer, sizeof(buffer));
        warehouseData.push_back(std::string(buffer));
        close(warehousePipes[i][0]);
    }

    // Create named pipes for product processes
    for (const auto& product : products) {
        std::string pipePath = "/tmp/" + product + "_pipe";
        mkfifo(pipePath.c_str(), 0666);
    }

    // Fork product processes
    std::vector<pid_t> productPIDs;
    for (const auto& product : products) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process (product)
            execl("./product", "product", product.c_str(), nullptr);
            perror("execl");
            exit(1);
        } else if (pid > 0) {
            productPIDs.push_back(pid);
        } else {
            perror("fork");
            return 1;
        }
    }

    // Send data from warehouses to product processes via named pipes
    for (const auto& data : warehouseData) {
        std::string productKey = extractProductKey(data);
        std::string pipePath = "/tmp/" + productKey + "_pipe";
        int fd = open(pipePath.c_str(), O_WRONLY);
        write(fd, data.c_str(), data.size());
        close(fd);
    }

    // Wait for product processes to finish
    for (const auto& pid : productPIDs) {
        waitpid(pid, nullptr, 0);
    }

    // Cleanup named pipes
    for (const auto& product : products) {
        std::string pipePath = "/tmp/" + product + "_pipe";
        unlink(pipePath.c_str());
    }

    // Final output and summary
    std::cout << "Processing complete. Results generated successfully!" << std::endl;

    return 0;
}
