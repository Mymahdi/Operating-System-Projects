#include <iostream>
#include <vector>
#include <cmath>
#include <sndfile.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Filter Parameters
const double BANDPASS_DELTA = 0.1;
const double NOTCH_F0 = 1000.0;
const int NOTCH_ORDER = 2;

// Band-pass Filter
void bandPassFilter(vector<double>& data, int sampleRate) {
    for (size_t i = 0; i < data.size(); ++i) {
        double f = static_cast<double>(i) / sampleRate;
        double H = f * f / (f * f + BANDPASS_DELTA * BANDPASS_DELTA);
        data[i] *= H;
    }
}

// Read Audio File
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

// Write Audio File
void writeAudioFile(const string& fileName, const vector<double>& data, SF_INFO& sfinfo) {
    SNDFILE* file = sf_open(fileName.c_str(), SFM_WRITE, &sfinfo);
    if (!file) {
        cerr << "Error: Could not write output file " << fileName << endl;
        exit(EXIT_FAILURE);
    }
    sf_write_double(file, data.data(), data.size());
    sf_close(file);
}

// Process Filter and Measure Execution Time
void processFilter(const string& filterName, const vector<double>& inputData, SF_INFO sfinfo, 
                   void (*filterFunc)(vector<double>&), const string& outputFile) {
    vector<double> data = inputData;
    auto start = high_resolution_clock::now();
    filterFunc(data);
    auto end = high_resolution_clock::now();

    writeAudioFile(outputFile, data, sfinfo);
    cout << filterName << " Time: " 
         << duration_cast<milliseconds>(end - start).count() << " ms" << endl;
}

void processFilterWithRate(const string& filterName, const vector<double>& inputData, SF_INFO sfinfo, 
                           void (*filterFunc)(vector<double>&, int), const string& outputFile) {
    vector<double> data = inputData;
    auto start = high_resolution_clock::now();
    filterFunc(data, sfinfo.samplerate);
    auto end = high_resolution_clock::now();

    writeAudioFile(outputFile, data, sfinfo);
    cout << filterName << " Time: " 
         << duration_cast<milliseconds>(end - start).count() << " ms" << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file.wav>" << endl;
        return EXIT_FAILURE;
    }

    // Read input file
    SF_INFO sfinfo;
    vector<double> originalData = readAudioFile(argv[1], sfinfo);

    // Apply filters
    processFilterWithRate("Band-pass Filter", originalData, sfinfo, bandPassFilter, "bandpass_output.wav");

    return 0;
}
