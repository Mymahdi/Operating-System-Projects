# Warehouse Data Processing System

This project is a distributed system designed to process warehouse data using a **Map-Reduce** paradigm. The system reads CSV files containing warehouse transactions, processes them using multiple mapper processes, and aggregates the results using a reducer process. The project is implemented in C++ and utilizes Unix system calls for process management and inter-process communication (IPC).

---

## Project Structure

The project consists of the following files:

1. **main.cpp**: The main program that orchestrates the processing of warehouse data.
2. **ProductData.cpp**: Implements the `ProductData` class for storing and managing product information.
3. **ProductProcess.cpp**: Contains functions for processing product data.
4. **ReducerAggregator.cpp**: Implements the `ReducerAggregator` class for aggregating data from the mapper processes.
5. **ReducerProcess.cpp**: The main logic for the reducer process, which aggregates and processes data from mappers.
6. **WarehouseProcessor.cpp**: Similar to `main.cpp`, but with additional logging functionality.
7. **MapperProcess.cpp**: The mapper process that reads and processes individual warehouse CSV files.

---

## Implementation Details

### 1. **Main Program (`main.cpp` and `WarehouseProcessor.cpp`)**
   - The main program collects all CSV files from the `stores` directory (excluding `Parts.csv`).
   - It creates a **pipe** for communication between the mapper and reducer processes.
   - It spawns multiple **mapper processes** (one for each CSV file) and a single **reducer process**.
   - The program logs messages to both the terminal and a log file (`process_log.txt`).

#### Key Functions:
   - **`spawnMapperProcesses`**: Forks a new process for each CSV file and executes the `MapperProcess` program.
   - **`spawnReducerProcess`**: Forks a new process to execute the `ReducerProcess` program.
   - **`logMessage`**: Logs messages with timestamps to the terminal and a log file.

---

### 2. **Mapper Process (`MapperProcess.cpp`)**
   - The mapper process reads a single warehouse CSV file and outputs its contents line by line to the standard output.
   - The output is redirected to the pipe, which is then read by the reducer process.
   - The mapper process is responsible for processing individual files in parallel.

#### Key Features:
   - The mapper process takes a single argument: the path to the warehouse CSV file.
   - It reads the file line by line and prints each line to `stdout`, which is redirected to the pipe.
   - After processing, the mapper closes its `stdout` and exits.

---

### 3. **Reducer Process (`ReducerProcess.cpp`)**
   - The reducer process reads data from the pipe (which contains the output from all mapper processes).
   - It aggregates the data by product, calculating the total profit and remaining stock for each product.
   - The reducer processes two types of transactions: **input** (stock addition) and **output** (stock removal).

#### Key Features:
   - The reducer maintains a `map` of `ProductData` structures, where each product's transactions are stored.
   - For **input** transactions, the reducer adds the stock to the product's inventory.
   - For **output** transactions, the reducer calculates the profit based on the FIFO (First-In-First-Out) method.
   - After processing all transactions, the reducer prints the total profit and remaining stock for each product, as well as the overall totals.

---

### 4. **Product Data Management (`ProductData.cpp`)**
   - The `ProductData` class stores information about a product, including its name, price, quantity, and total profit.
   - It provides methods to add transactions and print product information.

#### Key Methods:
   - **`addTransaction`**: Adds a transaction to the product's history, updating the quantity and total profit.
   - **`printProductInfo`**: Prints the product's details, including its price, quantity, and total profit.

---

### 5. **Product Processing (`ProductProcess.cpp`)**
   - This file contains functions for processing product data.
   - Currently, the `processProduct` function simply prints the product name being processed.
   - The `aggregateProductData` function is a placeholder for future aggregation logic.

---

### 6. **Reducer Aggregator (`ReducerAggregator.cpp`)**
   - The `ReducerAggregator` class is responsible for reading data from the pipe and aggregating it.
   - The `aggregateDataFromPipe` function reads lines from `stdin`, parses them, and prints the product name being aggregated.

---

## Workflow

1. **Data Collection**:
   - The main program collects all CSV files from the `stores` directory.
   - Each CSV file represents a warehouse's transaction data.

2. **Mapper Phase**:
   - A mapper process is spawned for each CSV file.
   - Each mapper reads its assigned file and outputs the data to the pipe.

3. **Reducer Phase**:
   - The reducer process reads the aggregated data from the pipe.
   - It processes the transactions, calculates profits, and prints the results.

4. **Logging**:
   - All processes log their actions to `process_log.txt` and the terminal.

---

## How to Run the Project

1. **Compile the Code**:
   - Compile all `.cpp` files using a C++ compiler (e.g., `g++`).
   - Example:
     ```bash
     g++ main.cpp ProductData.cpp ProductProcess.cpp ReducerAggregator.cpp ReducerProcess.cpp MapperProcess.cpp -o warehouse_processor
     ```

2. **Prepare Data**:
   - Place all warehouse CSV files in the `stores` directory.
   - Ensure the CSV files are formatted correctly (e.g., `product_name,price,quantity,operation`).

3. **Run the Program**:
   - Execute the compiled program:
     ```bash
     ./warehouse_processor
     ```

4. **View Results**:
   - The program will output the aggregated results to the terminal.
   - Detailed logs will be saved in `process_log.txt`.

---

## Future Improvements

1. **Error Handling**:
   - Add more robust error handling for file I/O and process management.

2. **Scalability**:
   - Optimize the system for handling larger datasets and more complex transactions.

3. **Enhanced Logging**:
   - Include more detailed logs for debugging and monitoring.

4. **Additional Features**:
   - Implement more advanced aggregation and analysis features, such as trend analysis or inventory forecasting.

---

## Conclusion

This project demonstrates a distributed system for processing warehouse data using the Map-Reduce paradigm. It leverages Unix system calls for process management and IPC, making it efficient and scalable. The system is designed to be extensible, allowing for future enhancements and optimizations.