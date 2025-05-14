
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

void locateCursor(const int row, const int col) {
    printf("%c[%d;%dH", 27, row, col);
}

void clearScreen() {
    printf("\e[1;1H\e[2J");
}

void displayImage(MNIST_Image *img, int row, int col) {
    char imgStr[(MNIST_IMG_HEIGHT * MNIST_IMG_WIDTH) + ((col + 1) * MNIST_IMG_HEIGHT) + 1];
    strcpy(imgStr, "");

    for (int y = 0; y < MNIST_IMG_HEIGHT; y++) {
        for (int o = 0; o < col - 2; o++) strcat(imgStr, " ");
        strcat(imgStr, "|");
        for (int x = 0; x < MNIST_IMG_WIDTH; x++) {
            strcat(imgStr, img->pixel[y * MNIST_IMG_HEIGHT + x] ? "X" : ".");
        }
        strcat(imgStr, "\n");
    }

    if (col != 0 && row != 0) locateCursor(row, 0);
    printf("%s", imgStr);
}

void displayImageFrame(int row, int col) {
    if (col != 0 && row != 0) locateCursor(row, col);
    printf("------------------------------\n");
    for (int i = 0; i < MNIST_IMG_HEIGHT; i++) {
        for (int o = 0; o < col - 1; o++) printf(" ");
        printf("|                            |\n");
    }
    for (int o = 0; o < col - 1; o++) printf(" ");
    printf("------------------------------");
}

void displayLoadingProgressTesting(int imgCount, int y, int x) {
    float progress = (float)(imgCount + 1) / (float)(MNIST_MAX_TESTING_IMAGES) * 100;
    if (x != 0 && y != 0) locateCursor(y, x);
    printf("Testing image No. %5d of %5d images [%d%%]\n                                  ", (imgCount + 1), MNIST_MAX_TESTING_IMAGES, (int)progress);
}

void displayProgress(int imgCount, int errCount, int y, int x) {
    double successRate = 1 - ((double)errCount / (double)(imgCount + 1));
    if (x != 0 && y != 0) locateCursor(y, x);
    printf("Result: Correct=%5d  Incorrect=%5d  Success-Rate= %5.2f%%\n", imgCount + 1 - errCount, errCount, successRate * 100);
}

uint32_t flipBytes(uint32_t n) {
    uint32_t b0, b1, b2, b3;
    b0 = (n & 0x000000ff) << 24u;
    b1 = (n & 0x0000ff00) << 8u;
    b2 = (n & 0x00ff0000) >> 8u;
    b3 = (n & 0xff000000) >> 24u;
    return (b0 | b1 | b2 | b3);
}

void readImageFileHeader(FILE *imageFile, MNIST_ImageFileHeader *ifh) {
    ifh->magicNumber = 0;
    ifh->maxImages = 0;
    ifh->imgWidth = 0;
    ifh->imgHeight = 0;
    fread(&ifh->magicNumber, 4, 1, imageFile);
    ifh->magicNumber = flipBytes(ifh->magicNumber);
    fread(&ifh->maxImages, 4, 1, imageFile);
    ifh->maxImages = flipBytes(ifh->maxImages);
    fread(&ifh->imgWidth, 4, 1, imageFile);
    ifh->imgWidth = flipBytes(ifh->imgWidth);
    fread(&ifh->imgHeight, 4, 1, imageFile);
    ifh->imgHeight = flipBytes(ifh->imgHeight);
}

void readLabelFileHeader(FILE *imageFile, MNIST_LabelFileHeader *lfh) {
    lfh->magicNumber = 0;
    lfh->maxImages = 0;
    fread(&lfh->magicNumber, 4, 1, imageFile);
    lfh->magicNumber = flipBytes(lfh->magicNumber);
    fread(&lfh->maxImages, 4, 1, imageFile);
    lfh->maxImages = flipBytes(lfh->maxImages);
}

FILE *openMNISTImageFile(const char *fileName) {
    FILE *imageFile = fopen(fileName, "rb");
    if (imageFile == NULL) {
        printf("Abort! Could not find MNIST IMAGE file: %s\n", fileName);
        exit(0);
    }
    MNIST_ImageFileHeader imageFileHeader;
    readImageFileHeader(imageFile, &imageFileHeader);
    return imageFile;
}

FILE *openMNISTLabelFile(const char *fileName) {
    FILE *labelFile = fopen(fileName, "rb");
    if (labelFile == NULL) {
        printf("Abort! Could not find MNIST LABEL file: %s\n", fileName);
        exit(0);
    }
    MNIST_LabelFileHeader labelFileHeader;
    readLabelFileHeader(labelFile, &labelFileHeader);
    return labelFile;
}

MNIST_Image getImage(FILE *imageFile) {
    MNIST_Image img;
    size_t result = fread(&img, sizeof(img), 1, imageFile);
    if (result != 1) {
        printf("\nError when reading IMAGE file! Abort!\n");
        exit(1);
    }
    return img;
}

MNIST_Label getLabel(FILE *labelFile) {
    MNIST_Label lbl;
    size_t result = fread(&lbl, sizeof(lbl), 1, labelFile);
    if (result != 1) {
        printf("\nError when reading LABEL file! Abort!\n");
        exit(1);
    }
    return lbl;
}