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
#include "../include/nethelp.h"


#define PORT_NUMBER 5000
#define SERVER_ADDRESS "192.168.1.7"
#define FILE_NAME_SIZE 256

int send_file(int fp, int sockfd, int filesize);
void wait_server_response(int client_socket);
void * thread_routine ();

int main(int argc, char **argv)
{   
    int thread_number, cicle_number, port_number;
    //  Arguments Check
    if (argc != 1)
    {
        fprintf(stderr, "usage: %s <ip> <port> <image> <N−threads> <N−cicles>\n", argv[0]);
        exit(0);
    }
   
    //  Thread number assignment
    thread_number = 5;
    cicle_number = 2;
    port_number = 5000;

    //  Start clock
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    //  Execute N cicles
    pthread_t tid[thread_number];
    for (int i = 0; i<cicle_number; ++i){
        //  Thread creation
        //pthread_t tid[thread_number];
        int i = 0;
        while(i<thread_number){
            if (pthread_create(&tid[i], NULL, thread_routine, NULL) != 0){
                printf("Error creating threads\n");
            } else {
                printf("Creado thread %d\n", i);
            }
            i++;
        }

        //  Thread join
        i = 0;
        while(i<thread_number){
            pthread_join(tid[i++], NULL);
            printf("Numero: %d\n",i);
        }
    }
    
    //  Stop clock
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Tiempo total: %f\n",elapsed);
   

    //  Create and write to bin file for report
    char port_c[8];
    char file_name[12];
    sprintf(port_c, "%d", port_number);
    strcat(file_name, port_c);
    strcat(file_name, ".bin");
    FILE *binfile;
    binfile = fopen(file_name, "wb");
        if (binfile == NULL) {
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

void * thread_routine (){
    int client_socket;
    struct sockaddr_in remote_addr;
    char filename[10];
    int image_file;

    //filename ="image.jpg"; //********** change to =argv[4]
    strcpy(filename, "image.jpg");  

    //  Zeroing remote_addr struct 
    memset(&remote_addr, 0, sizeof(remote_addr));

    //  Construct remote_addr struct 
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    // inet_pton(AF_INET, SERVER_ADDRESS, &(remote_addr.sin_addr));
    remote_addr.sin_port = htons(PORT_NUMBER);

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
    printf("Size of file is %d \n",filesize);

    send_file(image_file, client_socket, filesize);
    close(image_file);
    wait_server_response(client_socket);
    close(client_socket);
    pthread_exit(NULL);
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
            printf("Response received, shuting down connection.\n");
            break;
        }
    }
}