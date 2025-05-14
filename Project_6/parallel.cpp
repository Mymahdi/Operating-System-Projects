#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <omp.h>

using namespace std;

typedef struct Output_Node Output_Node;
typedef struct Hidden_Node Hidden_Node;
typedef struct MNIST_ImageFileHeader MNIST_ImageFileHeader;
typedef struct MNIST_LabelFileHeader MNIST_LabelFileHeader;

typedef struct MNIST_Image MNIST_Image;
typedef uint8_t MNIST_Label;

#define MNIST_TESTING_SET_IMAGE_FILE_NAME "data/t10k-images-idx3-ubyte"
#define MNIST_TESTING_SET_LABEL_FILE_NAME "data/t10k-labels-idx1-ubyte"

#define HIDDEN_WEIGHTS_FILE "net_params/hidden_weights.txt"
#define HIDDEN_BIASES_FILE "net_params/hidden_biases.txt"
#define OUTPUT_WEIGHTS_FILE "net_params/out_weights.txt"
#define OUTPUT_BIASES_FILE "net_params/out_biases.txt"

#define NUMBER_OF_INPUT_CELLS  784
#define NUMBER_OF_HIDDEN_CELLS 256
#define NUMBER_OF_OUTPUT_CELLS 10

#define MNIST_MAX_TESTING_IMAGES 10000
#define MNIST_IMG_WIDTH          28
#define MNIST_IMG_HEIGHT         28

struct Hidden_Node {
    double weights[NUMBER_OF_INPUT_CELLS];
    double bias;
    double output;
};

struct Output_Node {
    double weights[NUMBER_OF_HIDDEN_CELLS];
    double bias;
    double output;
};

struct MNIST_Image {
    uint8_t pixel[NUMBER_OF_INPUT_CELLS];
};

struct MNIST_ImageFileHeader {
    uint32_t magicNumber;
    uint32_t maxImages;
    uint32_t imgWidth;
    uint32_t imgHeight;
};

struct MNIST_LabelFileHeader {
    uint32_t magicNumber;
    uint32_t maxImages;
};

vector<Hidden_Node> hidden_nodes(NUMBER_OF_HIDDEN_CELLS);
vector<Output_Node> output_nodes(NUMBER_OF_OUTPUT_CELLS);