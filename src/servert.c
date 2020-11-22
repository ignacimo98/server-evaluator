/* 
 * echoservert.c - A concurrent echo server using threads
 */

#include "nethelp.h"
#include "sobel.h"
#include <stdio.h>
#include <string.h>

#include <pthread.h>

#define FILE_NAME_SIZE 50
#define IMAGE_FOLDER "./received_images/"

static int image_count = 0;

void echo(int connfd);
void receive_save_image(int connfd);
void *thread(void *vargp);

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

    listenfd = open_listenfd(port);
    while (1)
    {
        connfdp = (int *)malloc(sizeof(int));
        *connfdp = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
        pthread_create(&tid, NULL, thread, connfdp);
    }
}

/* thread routine */
void *thread(void *vargp)
{
    int connfd = *((int *)vargp);
    // pthread_detach(pthread_self());
    free(vargp);
    // echo(connfd);
    receive_save_image(connfd);
    close(connfd);
    return NULL;
}

/*
 * echo - read and echo text lines until client closes connection
 */
void receive_save_image(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    FILE *image;
    char file_name[FILE_NAME_SIZE];
    char count[3];
    memset(file_name, 0, FILE_NAME_SIZE);
    memset(count, 0, 3);

    strcat(file_name, IMAGE_FOLDER);
    sprintf(count, "%d", image_count);
    strcat(file_name, count);
    strcat(file_name, ".png");

    image = fopen(file_name, "w");
    if (image == NULL)
    {
        // change it to end gracefully
        exit(EXIT_FAILURE);
    }

    ++image_count;

    while((n = recv(connfd, buf, MAXLINE, 0)) > 0)
    // while ((n = readline(connfd, buf, MAXLINE)) != 0)
    {
        printf("server received %ld bytes\n", n);
        // fwrite(image,n,)
        fwrite(buf, 1, n, image);
        // write(image, buf, n);
    }

    fclose(image);

    apply_filter(file_name);
    printf("thread terminó de hacer filtro sobel\n");
}
