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
#include <time.h>

#include "nethelp.h"

//#define PORT_NUMBER 5000
//#define SERVER_ADDRESS "192.168.1.7"
#define FILE_NAME_SIZE 256
#define MEASUREMENT_FOLDER "./test/measurements"

int send_file(int fp, int sockfd, int filesize);
void wait_server_response(int client_socket);
void *thread_routine();

int *global_port;
int *global_cicles;
char *global_ip;
char *global_image_name;

int main(int argc, char **argv)
{
    int thread_number, cicle_number, port_number;
    //  Arguments Check
    if (argc != 6)
    {
        fprintf(stderr, "usage: %s <ip> <port> <image> <N−threads> <N−cicles>\n", argv[0]);
        exit(0);
    }

    //  Thread number assignment
    port_number = atoi(argv[2]);
    thread_number = atoi(argv[4]);
    cicle_number = atoi(argv[5]);

    //  Make ip number global
    global_ip = argv[1];
    //  Make port number global
    global_port = &port_number;
    //  Make image name number global
    global_image_name = argv[3];
    //  Make cicles number global
    global_cicles = &cicle_number;

    //  Start clock
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    pthread_t tid[thread_number];

    //  Thread creation
    int i = 0;
    while (i < thread_number)
    {
        if (pthread_create(&tid[i], NULL, thread_routine, NULL) != 0)
        {
            printf("Error creating threads\n");
        }
        else
        {
            i++;
        }
    }

    //  Thread join
    i = 0;
    while (i < thread_number)
    {
        pthread_join(tid[i++], NULL);
    }

    //  Stop clock
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Tiempo total: %f\n", elapsed);

    //  Create and write to bin file for report

    char file_name[256];

    sprintf(file_name, "%s/%d.bin", MEASUREMENT_FOLDER, port_number);

    FILE *binfile;
    binfile = fopen(file_name, "ab");
    if (binfile == NULL)
    {
        printf("Error creating binfile\n");
    }
    //  Writing Thread Number
    fwrite(&thread_number, sizeof(thread_number), 1, binfile);
    //  Writing Cicle Number
    fwrite(&cicle_number, sizeof(cicle_number), 1, binfile);
    //  Writing Elapsed Time
    fwrite(&elapsed, sizeof(elapsed), 1, binfile);

    return 0;
}

void *thread_routine()
{
    for (int i = 0; i < *global_cicles; ++i)
    {
        int client_socket;
        struct sockaddr_in remote_addr;
        char filename[10];
        int image_file;

        //  Getting global values
        int port_number = *global_port;
        strcpy(filename, global_image_name);

        //  Zeroing remote_addr struct
        memset(&remote_addr, 0, sizeof(remote_addr));

        //  Construct remote_addr struct
        remote_addr.sin_family = AF_INET;
        remote_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        // remote_addr.sin_addr.s_addr = inet_addr(global_ip);
        remote_addr.sin_port = htons(port_number);

        //  Create client socket
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1)
        {
            fprintf(stderr, "Error creating socket: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        //  Connect to the server
        if (connect(client_socket, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
        {
            fprintf(stderr, "Error on connect: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        //  Open the image to be send
        if ((image_file = open(filename, O_RDONLY)) == -1)
        {
            fprintf(stderr, "Error opening image file: %s\n", strerror(errno));
            close(client_socket);
            exit(EXIT_FAILURE);
        }
        // Get the file size
        struct stat st;
        stat(filename, &st);
        int filesize = st.st_size;

        send_file(image_file, client_socket, filesize);
        close(image_file);
        wait_server_response(client_socket);
        close(client_socket);
    }
    pthread_exit(NULL);
}

//  Send the file to the server
int send_file(int fp, int sockfd, int filesize)
{
    int n, total = 0;
    char sendline[256] = {0};
    char *tosend = (char *)&filesize;
    int remaining = sizeof(filesize);
    int result = 0;
    int sent = 0;
    //  Send image size to server
    while (remaining > 0)
    {
        result = send(sockfd, tosend + sent, remaining, 0);
        if (result > 0)
        {
            remaining -= result;
            sent += remaining;
        }
        else if (result < 0)
        {
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

void wait_server_response(int client_socket)
{
    size_t n;
    char buf[5];
    char done[5];
    strcpy(done, "done");
    while ((n = recv(client_socket, buf, 5, 0)) > 0)
    {
        // Response received, shuting down connection
        if (strcmp(buf, done) == 0)
        {
            break;
        }
    }
}