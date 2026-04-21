#include "producer.h"
#include "shared.h"

void *producer(void *args) {
  (void)args;
  while(true) {
    pthread_mutex_lock(&mutex_file_in);

    int ch = fgetc(file_in);

    if (ch == EOF) { //EOF checking section

      if (file_finished == 0) {
        file_finished = 1; // Flag to be sure that nobody later is going to elaborate with file
        
        pthread_mutex_unlock(&mutex_file_in); 

        for (int i = 0; i < NC; i++) {
          sem_wait(&sem_empty);
          pthread_mutex_lock(&mutex_buffer);

          shared_buffer.buf[shared_buffer.coda] = '\0';
          shared_buffer.coda = (shared_buffer.coda + 1) % BUFFER_SIZE;
          
          pthread_mutex_unlock(&mutex_buffer);
          sem_post(&sem_full);
        }
      }
      pthread_mutex_unlock(&mutex_file_in);
      break; //exiting from while(true cycle); we have finished 
    }
    
    sem_wait(&sem_empty);
    pthread_mutex_lock(&mutex_buffer);

    shared_buffer.buf[shared_buffer.coda] = (char)ch;
    shared_buffer.coda = (shared_buffer.coda + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex_buffer);
    sem_post(&sem_full);

    pthread_mutex_unlock(&mutex_file_in);
  }
  return NULL; // better ro return smth and don't have warning
  
}