#pragma once

#include "config.h"

typedef struct message Message;

struct message {
    long mtype;
    char mtext[BUF_SIZE];
};

enum TypeMessage {
    STD_TYPE_MESSAGE = 1,
    DONE_TYPE_MESSAGE = 2
};

int sampleMessageQueue(const char *fname_read, const char *fname_write);