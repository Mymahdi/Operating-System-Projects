
#include "mnist_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

uint32_t flipBytes(uint32_t n) {
    uint32_t b0 = (n & 0x000000ff) << 24u;
    uint32_t b1 = (n & 0x0000ff00) << 8u;
    uint32_t b2 = (n & 0x00ff0000) >> 8u;
    uint32_t b3 = (n & 0xff000000) >> 24u;
    return (b0 | b1 | b2 | b3);
}


void readImageFileHeader(FILE *imageFile, MNIST_ImageFileHeader *ifh) {
    size_t result;

    result = fread(&ifh->magicNumber, 4, 1, imageFile);
    if (result != 1) { std::cerr << "Error reading image magicNumber!" << std::endl; exit(1); }
    ifh->magicNumber = flipBytes(ifh->magicNumber);

    result = fread(&ifh->maxImages, 4, 1, imageFile);
    if (result != 1) { std::cerr << "Error reading image maxImages!" << std::endl; exit(1); }
    ifh->maxImages = flipBytes(ifh->maxImages);

    result = fread(&ifh->imgWidth, 4, 1, imageFile);
    if (result != 1) { std::cerr << "Error reading image imgWidth!" << std::endl; exit(1); }
    ifh->imgWidth = flipBytes(ifh->imgWidth);

    result = fread(&ifh->imgHeight, 4, 1, imageFile);
    if (result != 1) { std::cerr << "Error reading image imgHeight!" << std::endl; exit(1); }
    ifh->imgHeight = flipBytes(ifh->imgHeight);
}

void readLabelFileHeader(FILE *labelFile, MNIST_LabelFileHeader *lfh) {
    size_t result;

    result = fread(&lfh->magicNumber, 4, 1, labelFile);
    if (result != 1) { std::cerr << "Error reading label magicNumber!" << std::endl; exit(1); }
    lfh->magicNumber = flipBytes(lfh->magicNumber);

    result = fread(&lfh->maxImages, 4, 1, labelFile);
    if (result != 1) { std::cerr << "Error reading label maxImages!" << std::endl; exit(1); }
    lfh->maxImages = flipBytes(lfh->maxImages);
}

FILE *openMNISTImageFile(const char *fileName) {
    FILE *imageFile = fopen(fileName, "rb");
    if (imageFile == NULL) {
        printf("Abort! Could not find MNIST IMAGE file: %s\n", fileName);
        exit(1);
    }
    MNIST_ImageFileHeader imageFileHeader;
    readImageFileHeader(imageFile, &imageFileHeader);
    return imageFile;
}

FILE *openMNISTLabelFile(const char *fileName) {
    FILE *labelFile = fopen(fileName, "rb");
    if (labelFile == NULL) {
        printf("Abort! Could not find MNIST LABEL file: %s\n", fileName);
        exit(1);
    }
    MNIST_LabelFileHeader labelFileHeader;
    readLabelFileHeader(labelFile, &labelFileHeader);
    return labelFile;
}

MNIST_Image getImage(FILE *imageFile) {
    MNIST_Image img;
    size_t result = fread(&img, sizeof(img), 1, imageFile);
    if (result!=1) {
        printf("\nError when reading IMAGE file! Abort!\n");
        exit(1);
    }
    return img;
}

MNIST_Label getLabel(FILE *labelFile) {
    MNIST_Label lbl;
    size_t result = fread(&lbl, sizeof(lbl), 1, labelFile);
    if (result!=1) {
        printf("\nError when reading LABEL file! Abort!\n");
        exit(1);
    }
    return lbl;
}