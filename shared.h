#ifndef SHARED_H
#define SHARED_H

#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdbool.h>

#define NP           3         /* numero di thread produttori */
#define NC           3         /* numero di thread consumatori */
#define BUFFER_SIZE  8         /* capacità del buffer circolare */
#define INPUT_FILE   "file_in.txt"
#define OUTPUT_FILE  "file_out.txt"

typedef struct {
    char buf[BUFFER_SIZE];
    int testa;
    int coda;
    int count;
} buffer_t;

extern pthread_mutex_t mutex_file_in;
extern pthread_mutex_t mutex_file_out;
extern pthread_mutex_t mutex_buffer;

extern sem_t sem_empty;
extern sem_t sem_full;

extern FILE *file_in;
extern FILE *file_out;

extern int file_finished;
extern buffer_t shared_buffer;

#endif