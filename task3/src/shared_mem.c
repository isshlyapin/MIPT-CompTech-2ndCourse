#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "config.h"
#include "shared_mem.h"

int exchangeSharedMemory(const char *fname_read, const char *fname_write) {
    key_t key = ftok(fname_read, 65);
    if (key == -1) {
        perror("Error ftok failed");
        return EXIT_FAILURE;
    }
    
    int shmid = shmget(key, sizeof(SharedData), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("Error shmget failed");
        return EXIT_FAILURE;
    }

    SharedData *data = (SharedData*)shmat(shmid, NULL, 0);
    if (data == (void*)-1) {
        perror("Error shmat failed");
        return EXIT_FAILURE;
    }

    if (sem_init(&data->sem_write, 1, 1) == -1) {
        perror("Error sem_init failed");
        return EXIT_FAILURE;
    }

    if (sem_init(&data->sem_read, 1, 0) == -1) {
        perror("Error sem_init failed");
        return EXIT_FAILURE;
    }

    struct timespec start, end;
    double elapsed_time = 0;

    for (int i = 0; i < NUM_ITERATIONS; ++i) {
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

        pid_t pid = fork();

        if (pid < 0) {
            perror("Error fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            while (1) {
                sem_wait(&data->sem_read);
                write(fd_wr, data->message, data->len_message);
                sem_post(&data->sem_write);
                
                if (data->len_message == 0) {
                    break;
                }
            }
            shmdt(data);
            exit(EXIT_SUCCESS);
        } else {
            clock_gettime(CLOCK_MONOTONIC, &start);

            while (1) {
                sem_wait(&data->sem_write);
                data->len_message = read(fd_rd, data->message, BUF_SIZE);
                printf("Parent len message: %d\n", data->len_message);
                sem_post(&data->sem_read);
                
                if (data->len_message == 0) {
                    break;
                }
            }
        }

        int status = EXIT_FAILURE;
        wait(&status);
        if (status != EXIT_SUCCESS) {
            perror("Error wait failed");
            exit(EXIT_FAILURE);
        }

        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_time += (end.tv_sec - start.tv_sec) * 1e9;
        elapsed_time += (end.tv_nsec - start.tv_nsec);

        close(fd_wr);
        close(fd_rd);
    }


    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);

    printf("Copy time with shared memory: %.2f мс\n", elapsed_time / 1e6 / NUM_ITERATIONS);

    return EXIT_SUCCESS;
}
