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

#define PORT_NUMBER 9097
#define SERVER_ADDRESS "192.168.1.7"
#define FILE_NAME_SIZE 256

int send_file(int fp, int sockfd);

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

    /* Send filename */

    // sprintf(filename, "%s", basename(FILENAME));

    // if ((len = send(client_socket, basename(filename), FILE_NAME_SIZE, 0)) < 0)
    // {
    //     fprintf(stderr, "Error on sending filename --> %s", strerror(errno));
    //     close(client_socket);
    //     close(image_file);
    //     exit(EXIT_FAILURE);
    // }

    /* Send image file */

    send_file(image_file, client_socket);

    close(image_file);
    close(client_socket);
}

// sends a file that is already opened to a socket that is already opened
int send_file(int fp, int sockfd)
{
    int n, total;
    char sendline[256] = {0};

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