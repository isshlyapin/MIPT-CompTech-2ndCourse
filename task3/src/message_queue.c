#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>

#include "config.h"
#include "message_queue.h"

int sampleMessageQueue(const char *fname_read, const char *fname_write) {
    key_t key = ftok("shmfile", 65);
    
    int msgid = msgget(key, 0666 | IPC_CREAT);
    assert(msgid != -1);

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
            Message msg_cons;
            
            for (ssize_t len_msg = 0; 1; ) {
                len_msg = msgrcv(msgid, &msg_cons, sizeof(msg_cons.mtext), 0, 0);

                if (msg_cons.mtype != DONE_TYPE_MESSAGE) {
                    write(fd_wr, msg_cons.mtext, len_msg);
                } else {
                    break;
                }
            }

            exit(EXIT_SUCCESS);
        } else {
            Message msg_prod;
            clock_gettime(CLOCK_MONOTONIC, &start);

            for (int len_msg = 0; 1; ) {
                len_msg = read(fd_rd, msg_prod.mtext, BUF_SIZE);
                                
                if (len_msg) {
                    msg_prod.mtype = STD_TYPE_MESSAGE;
                    msgsnd(msgid, &msg_prod, len_msg, 0);
                } else {
                    msg_prod.mtype = DONE_TYPE_MESSAGE;
                    msgsnd(msgid, &msg_prod, 0, 0);
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

    msgctl(msgid, IPC_RMID, NULL);

    printf("Elapsed time: %.2f с\n", elapsed_time / 1e9 / NUM_ITERATIONS);

    return EXIT_SUCCESS;
}
