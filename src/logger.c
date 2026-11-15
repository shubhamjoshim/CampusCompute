#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define Q_SIZE 10
#define MESSAGE_SIZE 128
#define TOTAL_MESSAGES 25

char messages[Q_SIZE][MESSAGE_SIZE];

int head = 0;
int tail = 0;
int count = 0;
int producerDone = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notFull = PTHREAD_COND_INITIALIZER;
pthread_cond_t notEmpty = PTHREAD_COND_INITIALIZER;

void produce(const char *message){
  pthread_mutex_lock(&mutex);

  while(count == Q_SIZE){
    pthread_cond_wait(&notFull,&mutex);
  }

  snprintf(messages[tail],MESSAGE_SIZE,"%s",message);

  tail = (tail+1)%Q_SIZE;
  count ++;

  printf("Producer added message (queue count: %d)\n",count);

  pthread_cond_signal(&notEmpty);
  pthread_mutex_unlock(&mutex);
}

int consume(char *buffer, size_t size){
  pthread_mutex_lock(&mutex);
  
  while(count == 0 && !producerDone){
    pthread_cond_wait(&notEmpty, &mutex);
  }

  if(count == 0 && producerDone){
    pthread_mutex_unlock(&mutex);
    return 0;
  }

  snprintf(buffer,size,"%s",messages[head]);

  head = (head + 1) % Q_SIZE;
  count --;

  printf("Consumer removed message (queue count: %d)\n",count);

  pthread_cond_signal(&notFull);
  pthread_mutex_unlock(&mutex);

  return 1;
}

void *produceThread(void *arg){
  (void) arg;

  char buffer[MESSAGE_SIZE];

  for (int i=1;i<=TOTAL_MESSAGES;i++){
    snprintf(buffer,sizeof(buffer),"Job %d completed successfully\n",i);
    produce(buffer);
  }

  pthread_mutex_lock(&mutex);
  producerDone = 1;
  pthread_cond_broadcast(&notEmpty);
  pthread_mutex_unlock(&mutex);
  printf("Producer finished producing messages\n");

  return NULL;
}

void *consumeThread(void *arg){
  FILE *file = (FILE *)arg;
  char buffer[MESSAGE_SIZE];

  while(consume(buffer,sizeof(buffer))){
    fprintf(file,"%s\n",buffer);
    printf("Logger %s\n",buffer);
  }

  fflush(file);

  printf("Consumer finished consuming messges\n");

  return NULL;
}

int runLogger(void){
  pthread_t producer;
  pthread_t consumer;

  FILE *fp = fopen("campus.log","w");

  if(fp == NULL){
    perror("fopen");
    return 1;
  }

  head = 0;
  tail = 0;
  count = 0;
  producerDone = 0;

  int rc = pthread_create(&consumer,NULL,consumeThread,fp);

  if(rc != 0){
    fprintf(stderr,"Failed to cosume thread: %s\n",strerror(rc));
    fclose(fp);
    return 1;
  }
  
 rc = pthread_create(&producer, NULL, produceThread, NULL);

 if (rc != 0) {
      fprintf(stderr, "Failed to create producer thread: %s\n",
              strerror(rc));

      pthread_mutex_lock(&mutex);
      producerDone = 1;
      pthread_cond_broadcast(&notEmpty);
      pthread_mutex_unlock(&mutex);

      pthread_join(consumer, NULL);
      fclose(fp);
      return 1;
  }

  pthread_join(producer,NULL);
  pthread_join(consumer,NULL);

  if(fclose(fp) != 0){
    perror("fclose");
    return 1;
  }
  printf("All thread terminated successfully\n");
  printf("Logs saved to campus.log\n");

  return 0;
}

int main(){
  int result = runLogger();

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&notFull);
  pthread_cond_destroy(&notEmpty);

  return result;
}


