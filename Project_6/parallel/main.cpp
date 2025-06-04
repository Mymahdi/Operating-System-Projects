#include <iostream>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#include "common.h"
#include "display_utils.h"
#include "nn_params.h"
#include "thread_functions.h"

std::vector<Hidden_Node> hidden_nodes(NUMBER_OF_HIDDEN_CELLS);
std::vector<Output_Node> output_nodes(NUMBER_OF_OUTPUT_CELLS);
std::vector<MNIST_Image> images(MNIST_MAX_TESTING_IMAGES);
std::vector<MNIST_Label> labels(MNIST_MAX_TESTING_IMAGES);
int current_image = -1;
int errCount = 0;
volatile bool done = false;

pthread_mutex_t image_mutex;
pthread_mutex_t error_mutex;
sem_t input_ready, hidden_ready, output_ready;
pthread_barrier_t hidden_barrier;

int main() {
    time_t startTime = time(NULL);
    clearScreen();
    printf("    MNIST-NN: Parallel implementation with pthreads\n");

    pthread_mutex_init(&image_mutex, NULL);
    pthread_mutex_init(&error_mutex, NULL);
    sem_init(&input_ready, 0, 0);
    sem_init(&hidden_ready, 0, 0);
    sem_init(&output_ready, 0, 0);
    pthread_barrier_init(&hidden_barrier, NULL, 8); 

    allocateHiddenParameters();
    allocateOutputParameters();

    pthread_t input_tid;
    pthread_t hidden_tids[8];
    pthread_t output_tids[10];
    pthread_t prediction_tid;
    
    HiddenThreadParams hidden_params[8];
    int output_indices[10];

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    struct sched_param param;

    pthread_create(&input_tid, &attr, input_thread, NULL);

    param.sched_priority = sched_get_priority_max(SCHED_FIFO) - 10;
    pthread_attr_setschedparam(&attr, &param);
    for (int i = 0; i < 8; i++) {
        hidden_params[i].start_idx = i * (NUMBER_OF_HIDDEN_CELLS / 8);
        hidden_params[i].end_idx = (i + 1) * (NUMBER_OF_HIDDEN_CELLS / 8);
        pthread_create(&hidden_tids[i], &attr, hidden_thread, &hidden_params[i]);
    }

    param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    pthread_attr_setschedparam(&attr, &param);
    for (int i = 0; i < 10; i++) {
        output_indices[i] = i;
        pthread_create(&output_tids[i], &attr, output_thread, &output_indices[i]);
    }

    pthread_attr_destroy(&attr);
    pthread_attr_init(&attr);
    pthread_create(&prediction_tid, &attr, prediction_thread, NULL);

    pthread_join(input_tid, NULL);
    for (int i = 0; i < 8; i++) pthread_join(hidden_tids[i], NULL);
    for (int i = 0; i < 10; i++) pthread_join(output_tids[i], NULL);
    pthread_join(prediction_tid, NULL);

    pthread_mutex_destroy(&image_mutex);
    pthread_mutex_destroy(&error_mutex);
    sem_destroy(&input_ready);
    sem_destroy(&hidden_ready);
    sem_destroy(&output_ready);
    pthread_barrier_destroy(&hidden_barrier);

    time_t endTime = time(NULL);
    double executionTime = difftime(endTime, startTime);
    locateCursor(38, 5);
    printf("\n    DONE! Total execution time: %.1f sec\n\n", executionTime);

    return 0;
}

