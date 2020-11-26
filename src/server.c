/*
 * echoserver.c - A sequential echo server
 */

#include "nethelp.h"
#include "sobel.h"
#define FILE_NAME_SIZE 50
#define IMAGE_FOLDER "./received_images/"

static int image_count = 0;

void receive_save_image(int connfd);

int main(int argc, char **argv)
{
    int listenfd, connfd, port;
    socklen_t clientlen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    port = atoi(argv[1]);

    listenfd = open_listenfd(port);
    while (1)
    {
        connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
        receive_save_image(connfd); /* Service client */

        //************* RESPUESTA AL CLIENTE

        char buf[5];
        strcpy(buf, "done");
        if (send(connfd, buf, 5, 0) == -1)
        {
            perror("Can't send done flag to client");
            close(connfd);
            exit(1);
        }
        printf("Response sent, socked finished\n");

        //********************************************

        close(connfd); /* Close connection with client */
    }
}

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
    if (image_count < 100)
        ++image_count;

    //Receive image size from client
    int value = 0;
    char *recv_buffer = (char *)&value;
    int remaining = sizeof(int);
    int received = 0;
    int result = 0;
    while (remaining > 0)
    {
        result = recv(connfd, recv_buffer + received, remaining, 0);
        if (result > 0)
        {
            remaining -= result;
            received += result;
        }
        else if (result == 0)
        {
            printf("Remote side closed his end of the connection before all data was received\n");
            break;
        }
        else if (result < 0)
        {
            printf("Error receiving the image size\n");
            break;
        }
    }
    printf("Image size received: %d\n", value);

    int total = 0;
    //Receive Image from Client
    while ((n = recv(connfd, buf, MAXLINE, 0)) > 0)
    // while ((n = readline(connfd, buf, MAXLINE)) != 0)
    {
        total += n;
        printf("server received %ld bytes\n", n);
        // fwrite(image,n,)
        fwrite(buf, 1, n, image);
        if (total >= value)
        {
            printf("Image is here\n");
            break;
        }
        // write(image, buf, n);
    }

    fclose(image);

    apply_filter(file_name);
    printf("thread terminó de hacer filtro sobel\n");
}