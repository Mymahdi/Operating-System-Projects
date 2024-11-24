#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <fcntl.h>      // For open and O_RDONLY
#include <unistd.h>     // For read and close
#include <sys/types.h>  // For POSIX system calls

// Define the maximum buffer size
#define MAX_BUFFER 1024

int main(int argc, char** argv) {
    std::string pipePath = "/tmp/pipe";  // Replace with actual pipe path
    char buffer[MAX_BUFFER];  // Buffer to hold data from the pipe
    
    // Open the pipe for reading
    int fd = open(pipePath.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error opening pipe" << std::endl;
        return 1;
    }

    // Read data from the pipe
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead == -1) {
        std::cerr << "Error reading from pipe" << std::endl;
        close(fd);
        return 1;
    }

    // Process the data from the buffer
    std::string data(buffer, bytesRead);
    std::cout << "Data read from pipe: " << data << std::endl;

    // Close the pipe
    close(fd);

    return 0;
}
