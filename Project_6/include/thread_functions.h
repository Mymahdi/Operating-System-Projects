#ifndef THREAD_FUNCTIONS_H
#define THREAD_FUNCTIONS_H

#include <pthread.h> 

void *input_thread(void *arg);
void *hidden_thread(void *arg);
void *output_thread(void *arg);
void *prediction_thread(void *arg);


#endif 

