#pragma once

#include <semaphore.h>

#include "config.h"

typedef struct shared_data SharedData;

struct shared_data {
    int len_message;
    char message[BUF_SIZE];

    sem_t sem_write;
    sem_t sem_read;
};

int sampleSharedMemory(const char *fname_read, const char *fname_write);