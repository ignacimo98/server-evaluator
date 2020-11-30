/*
 * echoserver.c - A sequential echo server
 */

#include "nethelp.h"
#include "sobel.h"
#define FILE_NAME_SIZE 50
#define IMAGE_FOLDER "./received_images/received_images_s"
#define MAX_IMAGES 100

static int image_count = 1;

// void receive_save_image(int connfd);

int main(int argc, char **argv)
{
    char file_name[FILE_NAME_SIZE];
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
        int current_image_count;

        connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
        current_image_count = image_count;
        if (image_count < MAX_IMAGES)
            ++image_count;
        sprintf(file_name, "%s/%d.png", IMAGE_FOLDER, current_image_count);

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
        close(connfd); /* Close connection with client */
    }
}
