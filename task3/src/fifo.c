#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <assert.h>
#include <time.h>

#include "config.h"
#include "fifo.h"

int sampleFifo(const char *fname_read, const char *fname_write) {
    // mkfifo(FIFO_NAME, 0666);
    assert(mkfifo(FIFO_NAME, 0666) == 0);

    struct timespec start, end;
    double elapsed_time = 0;

    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        int fd_rd = open(fname_read, O_RDONLY); 
        assert(fd_rd != -1);
        int fd_wr = open(fname_write, O_WRONLY | O_CREAT, 0644);
        assert(fd_wr != -1);

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            int fd_fifo_rd = open(FIFO_NAME, O_RDONLY);
            assert(fd_rd != -1);
            
            char buf[BUF_SIZE];

            for (ssize_t len_msg = 0; 1; ) {
                len_msg = read(fd_fifo_rd, buf, BUF_SIZE);

                if (len_msg) {
                    write(fd_wr, buf, len_msg);
                } else {
                    break;
                }
            }
            close(fd_fifo_rd);
            exit(EXIT_SUCCESS);
        } else {
            int fd_fifo_wr = open(FIFO_NAME, O_WRONLY);
            assert(fd_fifo_wr != -1);

            char buf[BUF_SIZE];

            clock_gettime(CLOCK_MONOTONIC, &start);

            for (int len_msg = 0; 1; ) {
                len_msg = read(fd_rd, buf, BUF_SIZE);

                if (len_msg) {
                    write(fd_fifo_wr, buf, len_msg);
                } else {
                    break;
                }
            }

            close(fd_fifo_wr);
        }

        int status = EXIT_FAILURE;
        wait(&status);
        if (status != EXIT_SUCCESS) {
            perror("wait failed");
            exit(EXIT_FAILURE);
        }

        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_time += (end.tv_sec - start.tv_sec) * 1e9;
        elapsed_time += (end.tv_nsec - start.tv_nsec);

        close(fd_wr);
        close(fd_rd);
    }

    unlink(FIFO_NAME);

    printf("Elapsed time: %.2f с\n", elapsed_time / 1e9 / NUM_ITERATIONS);

    return EXIT_SUCCESS;
}