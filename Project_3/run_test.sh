#!/bin/bash

# Function to extract the execution time from the output
get_execution_time() {
    grep "Execution Time (Total):" | awk '{print $4}' | sed 's/ms//'
}

# Running the serial version
echo "Running Serial..."
serial_time=$(cd serial && ./VoiceFilters.out ../input.wav | get_execution_time)

# Running the parallel version
echo "Running Parallel..."
parallel_time=$(cd parallel && ./VoiceFilters.out ../input.wav | get_execution_time)

# Displaying the results
echo "Serial Time: ${serial_time} ms"
echo "Parallel Time: ${parallel_time} ms"

# Calculate the speedup
if [[ $serial_time -gt 0 ]]; then
    speedup=$(echo "scale=2; $serial_time / $parallel_time" | bc)
    echo "Speedup: $speedup"
else
    echo "Error: Serial time is zero or invalid!"
fi
