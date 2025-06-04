#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>
#include <stdint.h>
#include <vector>
#include <semaphore.h>

typedef struct Output_Node Output_Node;
typedef struct Hidden_Node Hidden_Node;
typedef struct MNIST_Image MNIST_Image;
typedef uint8_t MNIST_Label;

#define NUMBER_OF_INPUT_CELLS  784
#define NUMBER_OF_HIDDEN_CELLS 256
#define NUMBER_OF_OUTPUT_CELLS 10
#define MNIST_MAX_TESTING_IMAGES 10000
#define MNIST_IMG_WIDTH 28
#define MNIST_IMG_HEIGHT 28

#define MNIST_TESTING_SET_IMAGE_FILE_NAME "data/t10k-images-idx3-ubyte"
#define MNIST_TESTING_SET_LABEL_FILE_NAME "data/t10k-labels-idx1-ubyte"
#define HIDDEN_WEIGHTS_FILE "net_params/hidden_weights.txt"
#define HIDDEN_BIASES_FILE "net_params/hidden_biases.txt"
#define OUTPUT_WEIGHTS_FILE "net_params/out_weights.txt"
#define OUTPUT_BIASES_FILE "net_params/out_biases.txt"

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
    uint32_t magicNumber, maxImages, imgWidth, imgHeight;
};

struct MNIST_LabelFileHeader {
    uint32_t magicNumber, maxImages;
};

struct HiddenThreadParams {
    int start_idx;
    int end_idx;
};

extern std::vector<Hidden_Node> hidden_nodes;
extern std::vector<Output_Node> output_nodes;
extern std::vector<MNIST_Image> images;
extern std::vector<MNIST_Label> labels;
extern int current_image;
extern int errCount;
extern volatile bool done;

extern pthread_mutex_t image_mutex;
extern pthread_mutex_t error_mutex;
extern sem_t input_ready, hidden_ready, output_ready;
extern pthread_barrier_t hidden_barrier;

#endif