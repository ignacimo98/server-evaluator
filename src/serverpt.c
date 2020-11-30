#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>

#include "nethelp.h"
#include "queue.h"
#include "sobel.h"

#define FILE_NAME_SIZE 50
#define IMAGE_FOLDER "./received_images/received_images_t_pool"
#define QUEUE_CAPACITY 256
#define MAX_IMAGES 100

static int image_count = 0;

// void *thread(void *vargp);
void *thread(void *arg);

sem_t *semaphore;
pthread_mutex_t lock;

int main(int argc, char **argv)
{
  semaphore = sem_open("/serverpt_sem", O_CREAT, S_IRWXU | S_IRWXG, 0);

  int listenfd, port;

  socklen_t clientlen = sizeof(struct sockaddr_in);
  struct sockaddr_in clientaddr;

  struct Queue *request_queue = create_queue(QUEUE_CAPACITY);

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s <port> <thread_amount>\n", argv[0]);
    exit(0);
  }
  port = atoi(argv[1]);
  int thread_amount = atoi(argv[2]);

  if (pthread_mutex_init(&lock, NULL) != 0)
  {
    printf("\n mutex init has failed\n");
    return 1;
  }

  pthread_t tids[thread_amount];

  for (int i = 0; i < thread_amount; ++i)
  {
    pthread_create(&tids[i], NULL, thread, (void *)request_queue);
  }
  listenfd = open_listenfd(port);
  while (1)
  {
    // connfdp = (int *)malloc(sizeof(int));
    // *connfdp = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
    enqueue(request_queue,
            accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen));
    sem_post(semaphore);
  }
}

void *thread(void *arg)
{
  struct Queue *queue = (struct Queue *)arg;
  while (1)
  {

    sem_wait(semaphore);
    int connfd = dequeue(queue);
    int current_image_count;
    char file_name[FILE_NAME_SIZE];

    pthread_mutex_lock(&lock);
    current_image_count = image_count;
    // if (image_count < MAX_IMAGES)
    ++image_count;
    pthread_mutex_unlock(&lock);

    sprintf(file_name, "%s/%d.png", IMAGE_FOLDER, current_image_count % MAX_IMAGES);

    receive_save_image(connfd, file_name);

    apply_filter(file_name);
    // printf("thread terminó de hacer filtro sobel\n");

    //************* RESPUESTA AL CLIENTE

    char buf[5];
    strcpy(buf, "done");
    if (send(connfd, buf, 5, 0) == -1)
    {
      perror("Can't send done flag to client");
      close(connfd);
      exit(1);
    }
    // printf("Response sent, socked finished\n");

    //********************************************

    close(connfd);
  }
}

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