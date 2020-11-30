/* 
 * echoservert.c - A concurrent echo server using threads
 */

#include "nethelp.h"
#include "sobel.h"
#include <stdio.h>
#include <string.h>

#include <pthread.h>

#define FILE_NAME_SIZE 50
#define IMAGE_FOLDER "./received_images/received_images_t/"
#define MAX_IMAGES 100

static int image_count = 1;

void echo(int connfd);
void *thread(void *vargp);

pthread_mutex_t lock;

int main(int argc, char **argv)
{

    int listenfd, port;
    int *connfdp;
    socklen_t clientlen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;
    pthread_t tid;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    port = atoi(argv[1]);

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
        return 1;
    }

    listenfd = open_listenfd(port);
    while (1)
    {
        connfdp = (int *)malloc(sizeof(int));
        *connfdp = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
        pthread_create(&tid, NULL, thread, connfdp);
    }

    pthread_mutex_destroy(&lock);
    return 0;
}

/* thread routine */
void *thread(void *vargp)
{
    int connfd = *((int *)vargp);
    int current_image_count;
    char file_name[FILE_NAME_SIZE];

    free(vargp);

    pthread_mutex_lock(&lock);
    current_image_count = image_count;

    ++image_count;
    pthread_mutex_unlock(&lock);

    sprintf(file_name, "%s/%d.png", IMAGE_FOLDER, current_image_count % MAX_IMAGES);

    receive_save_image(connfd, file_name);

    apply_filter(file_name);

    //************* RESPUESTA AL CLIENTE

    char buf[5];
    strcpy(buf, "done");
    if (send(connfd, buf, 5, 0) == -1)
    {
        perror("Can't send done flag to client");
        close(connfd);
        exit(1);
    }

    //********************************************

    close(connfd);
    return NULL;
}
