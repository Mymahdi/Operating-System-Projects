
#include "thread_functions.h"
#include "common.h"
#include "mnist_utils.h"
#include "display_utils.h"
#include <iostream>
#include <cmath>

void *input_thread(void *arg) {
    FILE *imageFile = openMNISTImageFile(MNIST_TESTING_SET_IMAGE_FILE_NAME);
    FILE *labelFile = openMNISTLabelFile(MNIST_TESTING_SET_LABEL_FILE_NAME);

    for (int i = 0; i < MNIST_MAX_TESTING_IMAGES; i++) {
        pthread_mutex_lock(&image_mutex);
        images[i] = getImage(imageFile);
        labels[i] = getLabel(labelFile);
        current_image = i;
        displayLoadingProgressTesting(i, 5, 5);
        displayImage(&images[i], 8, 6);
        pthread_mutex_unlock(&image_mutex);
        sem_post(&input_ready);
    }

    pthread_mutex_lock(&image_mutex);
    done = true;
    pthread_mutex_unlock(&image_mutex);

    for (int i = 0; i < 8; i++) {
        sem_post(&input_ready);
    }

    fclose(imageFile);
    fclose(labelFile);
    return NULL;
}

void *hidden_thread(void *arg) {
    HiddenThreadParams *params = (HiddenThreadParams *)arg;
    while (true) {
        sem_wait(&input_ready);

        bool local_done_flag;
        int local_img_idx;

        pthread_mutex_lock(&image_mutex);
        local_done_flag = done;
        local_img_idx = current_image;
        pthread_mutex_unlock(&image_mutex);

        if (local_done_flag && local_img_idx == (MNIST_MAX_TESTING_IMAGES - 1)) {
            int barrier_ret = pthread_barrier_wait(&hidden_barrier);
            if (barrier_ret == PTHREAD_BARRIER_SERIAL_THREAD) {
                for (int i = 0; i < NUMBER_OF_OUTPUT_CELLS; i++) {
                    sem_post(&hidden_ready);
                }
            }
            break;
        }

        for (int j = params->start_idx; j < params->end_idx; j++) {
            hidden_nodes[j].output = 0;
            for (int z = 0; z < NUMBER_OF_INPUT_CELLS; z++) {
                hidden_nodes[j].output += (double)images[local_img_idx].pixel[z] * hidden_nodes[j].weights[z];
            }
            hidden_nodes[j].output += hidden_nodes[j].bias;
            hidden_nodes[j].output = (hidden_nodes[j].output >= 0) ? hidden_nodes[j].output : 0;
        }

        int barrier_ret = pthread_barrier_wait(&hidden_barrier);
        if (barrier_ret == PTHREAD_BARRIER_SERIAL_THREAD) {
            for (int i = 0; i < NUMBER_OF_OUTPUT_CELLS; ++i) {
                sem_post(&hidden_ready);
            }
        }
    }
    return NULL;
}

void *output_thread(void *arg) {
    int neuron_idx = *(int *)arg;
    while (true) {
        sem_wait(&hidden_ready);

        bool local_done_flag;
        int local_img_idx;

        pthread_mutex_lock(&image_mutex);
        local_done_flag = done;
        local_img_idx = current_image;
        pthread_mutex_unlock(&image_mutex);

        if (local_done_flag && local_img_idx == (MNIST_MAX_TESTING_IMAGES - 1)) {
            sem_post(&output_ready);
            break;
        }

        output_nodes[neuron_idx].output = 0;
        for (int j = 0; j < NUMBER_OF_HIDDEN_CELLS; j++) {
            output_nodes[neuron_idx].output += hidden_nodes[j].output * output_nodes[neuron_idx].weights[j];
        }
        output_nodes[neuron_idx].output += output_nodes[neuron_idx].bias;
        output_nodes[neuron_idx].output = 1 / (1 + exp(-output_nodes[neuron_idx].output));

        sem_post(&output_ready);
    }
    return NULL;
}

void *prediction_thread(void *arg) {
    while (true) {
        for (int i = 0; i < NUMBER_OF_OUTPUT_CELLS; i++) {
            sem_wait(&output_ready);
        }

        bool local_done_flag;
        int local_img_idx;

        pthread_mutex_lock(&image_mutex);
        local_done_flag = done;
        local_img_idx = current_image;
        pthread_mutex_unlock(&image_mutex);

        if (local_done_flag && local_img_idx == (MNIST_MAX_TESTING_IMAGES - 1)) {
            break;
        }

        double maxOut = 0;
        int maxInd = 0;
        for (int i = 0; i < NUMBER_OF_OUTPUT_CELLS; i++) {
            if (output_nodes[i].output > maxOut) {
                maxOut = output_nodes[i].output;
                maxInd = i;
            }
        }

        pthread_mutex_lock(&error_mutex);
        if (maxInd != labels[local_img_idx]) errCount++;
        printf("\n      Prediction: %d   Actual: %d ", maxInd, labels[local_img_idx]);
        displayProgress(local_img_idx, errCount, 5, 66);
        pthread_mutex_unlock(&error_mutex);
    }
    return NULL;
}


