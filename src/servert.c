/* 
 * echoservert.c - A concurrent echo server using threads
 */

#include "nethelp.h"
#include "sobel.h"
#include <stdio.h>
#include <string.h>

#include <pthread.h>

#define FILE_NAME_SIZE 50
#define IMAGE_FOLDER "./received_images"

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

    if (pthread_mutex_init(&lock, NULL) != 0) { 
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

    sprintf(file_name, "%s/%d.png", IMAGE_FOLDER, current_image_count);

    receive_save_image(connfd, file_name);

    apply_filter(file_name);
    printf("thread terminó de hacer filtro sobel\n");
    



//************* RESPUESTA AL CLIENTE
  
    char buf [5];
    strcpy(buf, "done");
    if (send(connfd,buf,5,0) == -1)
        {
            perror("Can't send done flag to client");
            close(connfd);
            exit(1);
        }
    printf("Response sent, socked finished\n");

   
//********************************************


    close(connfd);
    return NULL;
}

/*
 * echo - read and echo text lines until client closes connection
 */
void receive_save_image_(int connfd)
{
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
    pthread_mutex_lock(&lock);

    size_t n;
    int total = 0;
    char buf[MAXLINE];
    FILE *image;
    char file_name[FILE_NAME_SIZE];
    char count[3];
    memset(file_name, 0, FILE_NAME_SIZE);
    memset(count, 0, 3);

    //image_count+=1;
    strcat(file_name, IMAGE_FOLDER);
    sprintf(count, "%d", image_count);
    strcat(file_name, count);
    strcat(file_name, ".png");


    ++image_count;

    image = fopen(file_name, "w");
    if (image == NULL)
    {
        // change it to end gracefully
        exit(EXIT_FAILURE);
    }

    pthread_mutex_unlock(&lock);


    //Receive image size from client
    int value = 0;
    char* recv_buffer = (char*)&value;
    int remaining = sizeof(int);
    int received = 0;
    int result = 0;
    while (remaining > 0) {
        result = recv(connfd, recv_buffer+received, remaining, 0);
        if (result > 0) {
            remaining -= result;
            received += result;
        }
        else if (result == 0) {
            printf("Remote side closed his end of the connection before all data was received\n");
            break;
        }
        else if (result < 0) {
            printf("Error receiving the image size\n");
            break;
        }
    }
    printf("Image size received: %d\n", value);



    //Receive Image from Client 
    while((n = recv(connfd, buf, MAXLINE, 0)) > 0)
    {   
        total+=n;
        printf("Server received %ld bytes, total is %d\n", n,total);
        fwrite(buf, 1, n, image);
        if (total >= value){
            printf("Image is here\n");
            break;
        }
    }
    
    

    fclose(image);

    apply_filter(file_name);
    printf("thread terminó de hacer filtro sobel\n");

    //pthread_mutex_destroy(&lock);
}
