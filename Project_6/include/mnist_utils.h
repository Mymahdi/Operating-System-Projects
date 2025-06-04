#ifndef MNIST_UTILS_H
#define MNIST_UTILS_H

#include <stdio.h>
#include "common.h"

typedef struct MNIST_ImageFileHeader MNIST_ImageFileHeader;
typedef struct MNIST_LabelFileHeader MNIST_LabelFileHeader;


uint32_t flipBytes(uint32_t n);
void readImageFileHeader(FILE *imageFile, MNIST_ImageFileHeader *ifh);
void readLabelFileHeader(FILE *labelFile, MNIST_LabelFileHeader *lfh);
FILE *openMNISTImageFile(const char *fileName);
FILE *openMNISTLabelFile(const char *fileName);
MNIST_Image getImage(FILE *imageFile);
MNIST_Label getLabel(FILE *labelFile);

#endif


