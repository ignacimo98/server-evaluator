/*
 * echoserver.c - A sequential echo server
 */

#include "nethelp.h"
#include "sobel.h"
#define FILE_NAME_SIZE 50
#define IMAGE_FOLDER "./received_images/"

static int image_count = 0;

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
    ++image_count;
    sprintf(file_name, "%s/%d.png", IMAGE_FOLDER, current_image_count);

    receive_save_image(connfd, file_name);

    apply_filter(file_name);
    printf("thread terminó de hacer filtro sobel\n");

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

void receive_save_image_(int connfd)
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

  while ((n = recv(connfd, buf, MAXLINE, 0)) > 0)
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