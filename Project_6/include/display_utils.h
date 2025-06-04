
#ifndef DISPLAY_UTILS_H
#define DISPLAY_UTILS_H

#include "common.h" 

void locateCursor(const int row, const int col);
void clearScreen();
void displayImage(MNIST_Image *img, int row, int col);
void displayImageFrame(int row, int col);
void displayLoadingProgressTesting(int imgCount, int y, int x);
void displayProgress(int imgCount, int errCount, int y, int x);

#endif 

