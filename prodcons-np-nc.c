#include "producer.h"
#include "consumer.h"
#include "shared.h"

pthread_mutex_t mutex_file_in = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_file_out = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_buffer = PTHREAD_MUTEX_INITIALIZER;

buffer_t shared_buffer;

sem_t sem_empty;
sem_t sem_full;

FILE *file_in;
FILE *file_out;

int file_finished = 0;

int main(void) { // int argv[], char* argc
  pthread_t threads_prod[NP];
  pthread_t threads_cons[NC];

  shared_buffer.coda = 0;
  shared_buffer.count = 0;
  shared_buffer.testa = 0;

  sem_init(&sem_empty, 0, BUFFER_SIZE);
  sem_init(&sem_full, 0, 0);

  file_in = fopen(INPUT_FILE, "r");
  if(file_in == NULL) {
    perror("File opening error...");
    return 1;
  }

  file_out = fopen(OUTPUT_FILE, "w");
  if(file_out == NULL) {
    perror("File opening error...");
    return 1;
  }

  for(int i = 0; i < NP; i++) {
    if (pthread_create(&threads_prod[i], NULL, producer, NULL) != 0) {
      perror("Thread allocation error...");
    }
  }

  for(int i = 0; i < NC; i++) {
    if (pthread_create(&threads_cons[i], NULL, consumer, NULL) != 0) {
      perror("Thread allocation error...");
    }
  }

  for(int i = 0; i < NP; i++) {
    if (pthread_join(threads_prod[i], NULL) != 0) {
      perror("There is error happened during joining thread...");
    }
  }

  for(int i = 0; i < NC; i++) {
    if (pthread_join(threads_cons[i], NULL) != 0) {
      perror("There is error happened during joining thread...");
    }
  }

  fclose(file_in);
  fclose(file_out);

  sem_destroy(&sem_empty);
  sem_destroy(&sem_full);

  pthread_mutex_destroy(&mutex_file_in);
  pthread_mutex_destroy(&mutex_file_out);
  pthread_mutex_destroy(&mutex_buffer);

  return 0;
}