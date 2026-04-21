#include "consumer.h"
#include "shared.h"

void *consumer(void* args) {
  (void)args;
  while (true)  {
    pthread_mutex_lock(&mutex_file_out);

    sem_wait(&sem_full);
    pthread_mutex_lock(&mutex_buffer);

    char ch = shared_buffer.buf[shared_buffer.testa];

    shared_buffer.testa = (shared_buffer.testa + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex_buffer);
    sem_post(&sem_empty);

    if(ch == '\0') {
      pthread_mutex_unlock(&mutex_file_out);
      break;
    }

    fputc(ch, file_out);

    pthread_mutex_unlock(&mutex_file_out);
  }
  
  return NULL; //same practice of Returning pointer as in the producer.c
}
