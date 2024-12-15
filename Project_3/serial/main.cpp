#include <iostream>
#include <vector>
#include <cmath>
#include <sndfile.h>
#include <chrono>
#include <functional>

using namespace std;
using namespace std::chrono;

// Constants for filters
const double BANDPASS_DELTA = 0.1;
const double NOTCH_F0 = 1000.0;
const int NOTCH_ORDER = 2;

// Read WAV file
vector<double> readAudioFile(const string& fileName, SF_INFO& sfinfo) {
    SNDFILE* file = sf_open(fileName.c_str(), SFM_READ, &sfinfo);
    if (!file) {
        cerr << "Error: Could not open input file " << fileName << endl;
        exit(EXIT_FAILURE);
    }
    vector<double> data(sfinfo.frames);
    sf_read_double(file, data.data(), sfinfo.frames);
    sf_close(file);
    return data;
}

// Write WAV file
void writeAudioFile(const string& fileName, const vector<double>& data, SF_INFO& sfinfo) {
    SNDFILE* file = sf_open(fileName.c_str(), SFM_WRITE, &sfinfo);
    if (!file) {
        cerr << "Error: Could not write output file " << fileName << endl;
        exit(EXIT_FAILURE);
    }
    sf_write_double(file, data.data(), data.size());
    sf_close(file);
}

// Band-pass Filter
void bandPassFilter(vector<double>& data, int sampleRate) {
    for (size_t i = 0; i < data.size(); ++i) {
        double f = static_cast<double>(i) / sampleRate;
        double H = f * f / (f * f + BANDPASS_DELTA * BANDPASS_DELTA);
        data[i] *= H;
    }
}

// Notch Filter
void notchFilter(vector<double>& data, int sampleRate) {
    for (size_t i = 0; i < data.size(); ++i) {
        double f = static_cast<double>(i) / sampleRate;
        double factor = 1.0 / (pow((f / NOTCH_F0), 2 * NOTCH_ORDER) + 1.0);
        data[i] *= factor;
    }
}

// IIR Filter
void iirFilter(vector<double>& data, int) {
    double alpha = 0.5;
    for (size_t i = 1; i < data.size(); ++i) {
        data[i] = alpha * data[i] + (1 - alpha) * data[i - 1];
    }
}

// FIR Filter
void firFilter(vector<double>& data, int) {
    const int N = 10; // Filter length
    vector<double> kernel(N, 1.0 / N);
    vector<double> result(data.size(), 0.0);

    for (size_t i = N / 2; i < data.size() - N / 2; ++i) {
        for (int j = 0; j < N; ++j) {
            result[i] += data[i - N / 2 + j] * kernel[j];
        }
    }
    data.swap(result);
}

// Function to measure filter execution time
double processFilter(const string& filterName, const string& outputFile, vector<double> inputData, SF_INFO& sfinfo, 
                     const function<void(vector<double>&, int)>& filterFunc) {
    auto start = high_resolution_clock::now();
    filterFunc(inputData, sfinfo.samplerate);
    auto end = high_resolution_clock::now();
    writeAudioFile(outputFile, inputData, sfinfo);
    double time = duration_cast<milliseconds>(end - start).count();
    cout << filterName << " Time: " << time << " ms" << endl;
    return time;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file.wav>" << endl;
        return EXIT_FAILURE;
    }

    SF_INFO sfinfo;
    auto readStart = high_resolution_clock::now();
    vector<double> originalData = readAudioFile(argv[1], sfinfo);
    auto readEnd = high_resolution_clock::now();
    double readTime = duration_cast<milliseconds>(readEnd - readStart).count();
    cout << "Read Time: " << readTime << " ms" << endl;

    double totalTime = 0.0;

    totalTime += processFilter("IIR Filter", "iir_output.wav", originalData, sfinfo, iirFilter);
    totalTime += processFilter("FIR Filter", "fir_output.wav", originalData, sfinfo, firFilter);
    totalTime += processFilter("Notch Filter", "notch_output.wav", originalData, sfinfo, notchFilter);
    totalTime += processFilter("Band-pass Filter", "bandpass_output.wav", originalData, sfinfo, bandPassFilter);

    cout << "Execution Time (Total): " << totalTime << " ms" << endl;

    return 0;
}
