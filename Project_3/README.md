# Serial Audio Processing Implementation

## Introduction
This report analyzes a serial implementation of an audio processing program in C++. The program reads a WAV file, applies multiple filters (IIR, FIR, Notch, and Band-pass), and measures the execution time for each filtering process.

## Code Overview

### Dependencies and Libraries
The program uses the following standard and external libraries:
- `<iostream>`: Standard input-output stream.
- `<vector>`: Dynamic array support.
- `<cmath>`: Mathematical functions.
- `<sndfile.h>`: Libsndfile for reading and writing WAV files.
- `<chrono>`: High-resolution timing for performance measurement.
- `<functional>`: Function wrappers for filter function pointers.

### Constants
- `BANDPASS_DELTA = 0.1`: Defines the bandwidth of the band-pass filter.
- `NOTCH_F0 = 1000.0`: Central frequency of the notch filter.
- `NOTCH_ORDER = 2`: Order of the notch filter.

## Functions

### `readAudioFile`
Reads a WAV file and stores the audio data in a vector.
- Uses `sf_open()` to open the file.
- Reads audio samples using `sf_read_double()`.
- Returns a vector of double values representing the audio samples.

### `writeAudioFile`
Writes processed audio data to an output WAV file.
- Uses `sf_open()` to create a new file.
- Writes samples using `sf_write_double()`.
- Closes the file after writing.

### `bandPassFilter`
Applies a band-pass filter to the audio data.
- Uses the formula: \( H(f) = \frac{f^2}{f^2 + \Delta^2} \)
- Attenuates frequencies outside the band.

### `notchFilter`
Applies a notch filter to attenuate frequencies around `NOTCH_F0`.
- Uses the formula: \( \text{factor} = \frac{1}{(f / F_0)^{2N} + 1} \)
- Reduces amplitude at the notch frequency.

### `iirFilter`
Implements an Infinite Impulse Response (IIR) filter.
- Uses the formula: \( y[n] = \alpha x[n] + (1 - \alpha) y[n-1] \)
- Implements a simple smoothing filter.

### `firFilter`
Implements a Finite Impulse Response (FIR) filter.
- Uses a moving average kernel of size `N = 10`.
- Convolves the kernel with the input data.

### `processFilter`
Measures execution time for a given filter function.
- Uses `std::chrono` to track execution time.
- Writes the filtered data to an output file.
- Prints the execution time to `stdout`.

### `main`
Entry point of the program.
- Reads the input WAV file.
- Calls `processFilter` for each filter (IIR, FIR, Notch, Band-pass).
- Measures total execution time.

## Execution Flow
1. The program starts by reading the input WAV file.
2. It processes the audio data with four filters sequentially.
3. Each filtering operation is timed and logged.
4. Processed audio files are saved as `iir_output.wav`, `fir_output.wav`, `notch_output.wav`, and `bandpass_output.wav`.
5. The total execution time is printed at the end.

## Performance Considerations
- The serial implementation processes filters one after another, which may be inefficient for large files.
- Using multi-threading or parallel processing could significantly reduce execution time.
- Memory allocation and vector operations could be optimized to improve efficiency.

## Conclusion
This program demonstrates a serial audio processing pipeline using multiple digital filters. Future improvements could involve parallel processing techniques using OpenMP or CUDA for faster execution.

---
---


# Parallel Audio Processing Implementation

## Introduction
This document describes the parallel implementation of an audio processing system that applies multiple filters to an input `.wav` file. The implementation utilizes C++ multithreading to execute different filters concurrently, improving performance over a serial implementation.

## Parallel Implementation
The parallel approach is implemented using the C++ `<thread>` library to run different filters simultaneously on separate threads.

### Key Features:
- Uses multiple threads to apply different filters concurrently.
- Reads audio data from a file and applies IIR, FIR, Notch, and Bandpass filters in parallel.
- Writes the processed data to output `.wav` files.
- Measures execution time for performance analysis.

## Code Overview
### **Audio File Handling**
- `readAudioFile()`: Reads audio data from a `.wav` file into a vector.
- `writeAudioFile()`: Writes processed audio data to a `.wav` file.

### **Filter Implementations**
- `bandPassFilter()`: Implements a bandpass filter.
- `notchFilter()`: Implements a notch filter.
- `iirFilter()`: Implements an Infinite Impulse Response (IIR) filter.
- `firFilter()`: Implements a Finite Impulse Response (FIR) filter.

### **Parallel Processing**
- `parallelProcessFilters()`: Creates multiple threads to apply filters concurrently.
- Each filter function runs in a separate thread, reducing processing time compared to serial execution.

## Execution Flow
1. **Read Audio File**: The input `.wav` file is read into memory.
2. **Start Parallel Processing**:
   - A separate thread is created for each filter function.
   - Filters are applied to the input data concurrently.
   - Processed data is written to separate output files.
3. **Measure Execution Time**: The execution time of each filter and the total time is recorded and displayed.

## Differences Between Serial and Parallel Implementations

| Aspect           | Serial Implementation | Parallel Implementation |
|----------------|----------------------|----------------------|
| Execution Time | Longer (filters run sequentially) | Shorter (filters run concurrently) |
| CPU Utilization | Single-threaded | Multi-threaded |
| Performance    | Limited by a single core | Utilizes multiple cores for efficiency |
| Complexity     | Simpler (easier to debug) | More complex (needs synchronization if data sharing is required) |

## Performance Considerations
- **Parallel Speedup**: Since each filter runs independently, the parallel approach significantly reduces processing time.
- **Thread Overhead**: Creating and managing threads introduces some overhead but is outweighed by performance gains for large datasets.
- **Scalability**: Can be extended to utilize more threads for additional filters if needed.

## Conclusion
The parallel implementation enhances performance by leveraging multithreading to apply filters concurrently. This results in reduced processing time and better CPU utilization compared to the serial approach. This approach is beneficial when handling large audio files where processing time is a critical factor.

