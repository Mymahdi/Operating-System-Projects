#include <iostream>
#include <vector>
#include <cmath>
#include <sndfile.h>
#include <cstring>
#include <string>

// Class to handle audio file operations
class AudioFileHandler {
public:
    AudioFileHandler() {
        std::memset(&fileInfo, 0, sizeof(fileInfo));
    }

    void readWavFile(const std::string& inputFile, std::vector<float>& data) {
        SNDFILE* inFile = sf_open(inputFile.c_str(), SFM_READ, &fileInfo);
        if (!inFile) {
            std::cerr << "Error opening input file: " << sf_strerror(NULL) << std::endl;
            exit(1);
        }

        data.resize(fileInfo.frames * fileInfo.channels);
        sf_count_t numFrames = sf_readf_float(inFile, data.data(), fileInfo.frames);
        if (numFrames != fileInfo.frames) {
            std::cerr << "Error reading frames from file." << std::endl;
            sf_close(inFile);
            exit(1);
        }

        sf_close(inFile);
        std::cout << "Successfully read " << numFrames << " frames from " << inputFile << std::endl;
    }

void writeWavFile(const std::string& outputFile, const std::vector<float>& data) {
    SF_INFO updatedFileInfo = fileInfo; // Make a copy of the file info
    updatedFileInfo.frames = data.size() / fileInfo.channels; // Update the frame count

    SNDFILE* outFile = sf_open(outputFile.c_str(), SFM_WRITE, &updatedFileInfo);
    if (!outFile) {
        std::cerr << "Error opening output file: " << sf_strerror(NULL) << std::endl;
        exit(1);
    }

    sf_count_t numFrames = sf_writef_float(outFile, data.data(), updatedFileInfo.frames);
    if (numFrames != updatedFileInfo.frames) {
        std::cerr << "Error writing frames to file." << std::endl;
        sf_close(outFile);
        exit(1);
    }

    sf_close(outFile);
    std::cout << "Successfully wrote " << numFrames << " frames to " << outputFile << std::endl;
}


    SF_INFO getFileInfo() const {
        return fileInfo;
    }

private:
    SF_INFO fileInfo;
};

// Function prototypes
std::vector<float> applyBandPassFilter(const std::vector<float>& signal, double f, double deltaF);
std::vector<float> applyNotchFilter(const std::vector<float>& signal, double f0, int n);
void initializeFilterParameters(double& f, double& deltaF, double& f0, int& n);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    AudioFileHandler audioHandler;
    std::vector<float> audioData;

    // Read input file
    audioHandler.readWavFile(inputFile, audioData);

    // Filter parameters
    double f, deltaF, f0;
    int n;
    initializeFilterParameters(f, deltaF, f0, n);

    // Apply Band-Pass Filter
    std::vector<float> bandPassData = applyBandPassFilter(audioData, f, deltaF);
    audioHandler.writeWavFile("band_pass_output.wav", bandPassData);

    // Apply Notch Filter
    std::vector<float> notchData = applyNotchFilter(audioData, f0, n);
    audioHandler.writeWavFile("notch_filter_output.wav", notchData);

    // Apply IIR Filter (Placeholder for demonstration)
    std::vector<float> iirData = audioData; // Replace with actual IIR filter implementation
    audioHandler.writeWavFile("iir_filter_output.wav", iirData);

    // Apply FIR Filter (Placeholder for demonstration)
    std::vector<float> firData = audioData; // Replace with actual FIR filter implementation
    audioHandler.writeWavFile("fir_filter_output.wav", firData);

    std::cout << "Filtered audio files saved: \n"
              << "1. band_pass_output.wav\n"
              << "2. notch_filter_output.wav\n"
              << "3. iir_filter_output.wav\n"
              << "4. fir_filter_output.wav\n";

    return 0;
}



// Band-pass filter implementation
std::vector<float> applyBandPassFilter(const std::vector<float>& signal, double f, double deltaF) {
    std::vector<float> filtered(signal.size());
    double denominator = f * f + deltaF * deltaF;

    for (size_t i = 0; i < signal.size(); ++i) {
        double numerator = f * f;
        double H = numerator / denominator;
        filtered[i] = signal[i] * H;
    }

    return filtered;
}

// Notch filter implementation
std::vector<float> applyNotchFilter(const std::vector<float>& signal, double f0, int n) {
    std::vector<float> filtered(signal.size());

    for (size_t i = 0; i < signal.size(); ++i) {
        double H = 1.0 / (std::pow(signal[i] / f0, 2 * n) + 1);
        filtered[i] = signal[i] * H;
    }

    return filtered;
}

// Function to initialize filter parameters
void initializeFilterParameters(double& f, double& deltaF, double& f0, int& n) {
    f = 500.0;       // Center frequency for band-pass filter
    deltaF = 100.0;  // Bandwidth for band-pass filter
    f0 = 1000.0;     // Frequency to notch
    n = 2;           // Order for notch filter
}
