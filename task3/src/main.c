#include <stdlib.h>
#include <stdio.h>

#include "shared_mem.h"
#include "message_queue.h"
#include "fifo.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("usage: %s <filename_read> <filename_write>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // sampleSharedMemory(argv[1], argv[2]);

    // sampleMessageQueue(argv[1], argv[2]);

    sampleFifo(argv[1], argv[2]);

    return 0;
}
