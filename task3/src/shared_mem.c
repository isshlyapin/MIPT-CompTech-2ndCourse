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

#include "shared_mem.h"
#include "config.h"

void sampleSharedMemory(const char *fname_read, const char *fname_write) {
    key_t key = ftok("shmfile", 65);
    
    int shmid = shmget(key, sizeof(SharedData), 0666 | IPC_CREAT);

    SharedData *data = (SharedData*)shmat(shmid, NULL, 0);

    sem_init(&data->sem_write, 1, 1);
    sem_init(&data->sem_read, 1, 0);

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
                sem_post(&data->sem_read);
                
                if (data->len_message == 0) {
                    break;
                }
            }
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

    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);

    printf("Elapsed time: %.2f с\n", elapsed_time / 1e9 / NUM_ITERATIONS);
}
