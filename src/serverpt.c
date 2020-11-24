#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>

#include "nethelp.h"
#include "queue.h"
#include "sobel.h"

#define FILE_NAME_SIZE 50
#define IMAGE_FOLDER "./received_images/"
#define QUEUE_CAPACITY 256

void receive_save_image(int connfd);
// void *thread(void *vargp);
void *thread(void* arg);

sem_t *semaphore;

int main(int argc, char **argv) {
  semaphore = sem_open("/serverpt_sem", O_CREAT, S_IRWXU | S_IRWXG, 0);

  int listenfd, port;

  socklen_t clientlen = sizeof(struct sockaddr_in);
  struct sockaddr_in clientaddr;

  struct Queue *request_queue = create_queue(QUEUE_CAPACITY);

  if (argc != 3) {
    fprintf(stderr, "usage: %s <port> <thread_amount>\n", argv[0]);
    exit(0);
  }
  port = atoi(argv[1]);
  int thread_amount = atoi(argv[2]);

  pthread_t tids[thread_amount];

  for (int i = 0; i < thread_amount; ++i) {
    pthread_create(&tids[i], NULL, thread, (void*) request_queue);
  }
  listenfd = open_listenfd(port);
  while (1) {
    // connfdp = (int *)malloc(sizeof(int));
    // *connfdp = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
    enqueue(request_queue,
            accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen));
    sem_post(semaphore);
    }
}

void *thread(void* arg) {
  struct Queue *queue = (struct Queue*) arg;
  while (1) {
    sem_wait(semaphore);
    int connfd = dequeue(queue);
    receive_save_image(connfd);
    close(connfd);
  }
}

void receive_save_image(int connfd) { sleep(1); }

// /* thread routine */
// void *thread(void *vargp) {
//   while (1) {
//     int connfd = *((int *)vargp);
//     // pthread_detach(pthread_self());
//     free(vargp);
//     // echo(connfd);
//     receive_save_image(connfd);
//     close(connfd);
//   }
//   return NULL;
// }