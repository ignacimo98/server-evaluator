#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>
#include <pthread.h>
#include "../include/nethelp.h"

#define PORT_NUMBER 5000
#define SERVER_ADDRESS "192.168.1.7"
#define FILE_NAME_SIZE 256

int send_file(int fp, int sockfd, int filesize);
void wait_server_response(int client_socket);

int main(int argc, char **argv)
{
    int client_socket;
    // ssize_t len;
    struct sockaddr_in remote_addr;
    char filename[FILE_NAME_SIZE];
    int image_file;

    /* Zeroing remote_addr struct */
    memset(&remote_addr, 0, sizeof(remote_addr));

    /* Construct remote_addr struct */
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    // inet_pton(AF_INET, SERVER_ADDRESS, &(remote_addr.sin_addr));
    remote_addr.sin_port = htons(PORT_NUMBER);

    /* Create client socket */
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        fprintf(stderr, "Error creating socket --> %s\n", strerror(errno));

        exit(EXIT_FAILURE);
    }

    /* Connect to the server */
    if (connect(client_socket, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "Error on connect --> %s\n", strerror(errno));

        exit(EXIT_FAILURE);
    }

    memset(filename, 0, FILE_NAME_SIZE);
    printf("Escriba el nombre de la imagen a enviar: ");
    scanf("%s", filename);


    if ((image_file = open(filename, O_RDONLY)) == -1)
    {
        fprintf(stderr, "Error opening image file --> %s\n", strerror(errno));
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Get the file size
    struct stat st;
    stat(filename, &st);
    int filesize = st.st_size;
    printf("Size of file is %d \n",filesize);

    send_file(image_file, client_socket, filesize);

    close(image_file);
    
    wait_server_response(client_socket);


    close(client_socket);
}

//  Send the file to the server
int send_file(int fp, int sockfd, int filesize)
{
    int n, total = 0;
    char sendline[256] = {0};
    char* tosend = (char*)&filesize;
    int remaining = sizeof(filesize);
    int result = 0;
    int sent = 0;
    //  Send image size to server
    while (remaining > 0) {
        result = send(sockfd, tosend+sent, remaining, 0);
        if (result > 0) {
            remaining -= result;
            sent += remaining;
        }
        else if (result < 0) {
            printf("ERROR sending the image size\n");
            break;
        }
    }

    //  Send Image to server
    while ((n = read(fp, sendline, 256)) > 0)
    {
        total += n;
        if (send(sockfd, sendline, n, 0) == -1)
        {
            perror("Can't send file");
            close(sockfd);
            close(fp);
            exit(1);
        }
        memset(sendline, 0, 256);
    }
    return total;
}

void wait_server_response(int client_socket){
    size_t n;
    char buf [5];
    char done [5];
    strcpy(done, "done");
    while((n = recv(client_socket, buf, 5, 0)) > 0){
        if (strcmp(buf, done) == 0){
            printf("RESPUESTA RECIBIDA\n");
            break;
        }
    }
}