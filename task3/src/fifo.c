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
    if (mkfifo(FIFO_NAME, 0666) != 0) {
        perror("Error mkfifo failed");
        return EXIT_FAILURE; 
    }

    int fd_rd = open(fname_read, O_RDONLY); 
    if (fd_rd == -1) {
        perror("Error open failed");
        exit(EXIT_FAILURE);
    }

    int fd_wr = open(fname_write, O_WRONLY | O_CREAT, 0644);
    if (fd_wr == -1) {
        perror("Error open failed");
        exit(EXIT_FAILURE);
    }

    struct timespec start, end;
    double elapsed_time = 0;

    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            int fd_fifo_rd = open(FIFO_NAME, O_RDONLY);
            if (fd_fifo_rd == -1) {
                perror("Error open failed");
                exit(EXIT_FAILURE);
            }
            
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
            if (fd_fifo_wr == -1) {
                perror("Error open failed");
                exit(EXIT_FAILURE);
            }

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

        lseek(fd_rd, 0, SEEK_SET);
        lseek(fd_wr, 0, SEEK_SET);
    }
    close(fd_wr);
    close(fd_rd);

    unlink(FIFO_NAME);

    printf("Copy time with FIFO: %.2f мс\n", elapsed_time / 1e6 / NUM_ITERATIONS);

    return EXIT_SUCCESS;
}