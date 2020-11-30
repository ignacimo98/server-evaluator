#include "nethelp.h"

/* 
 * open_listenfd - open and return a listening socket on port
 * Returns -1 in case of failure 
 */
int open_listenfd(int port)
{
    int listenfd, optval = 1;
    struct sockaddr_in serveraddr;

    /* Create a socket descriptor */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    /* Eliminates "Address already in use" error from bind. */
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                   (const void *)&optval, sizeof(int)) < 0)
        return -1;

    /* listenfd will be an endpoint for all requests to port
       on any IP address for this host */
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);
    if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
        return -1;

    /* Make it a listening socket ready to accept connection requests */
    if (listen(listenfd, LISTENQ) < 0)
        return -1;
    return listenfd;
} /* end open_listenfd */

/*
 * open_clientfd - open connection to server at <hostname, port>
 *   and return a socket descriptor ready for reading and writing.
 *   Return <0 in case of failure.
 */
int open_clientfd(char *hostname, int port)
{
    int clientfd;
    struct hostent *hp;
    struct sockaddr_in serveraddr;

    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    /* Fill in the server's IP address and port */
    if ((hp = gethostbyname(hostname)) == NULL)
        return -2;
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)hp->h_addr,
          (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
    serveraddr.sin_port = htons(port);

    /* Establish a connection with the server */
    if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
        return -1;
    return clientfd;
} /* end open_clientfd */

/*
 * readline - read a line of text
 * return the number of characters read
 * return -1 if error
 */
int readline(int fd, char *buf, int maxlen)
{
    int nc, n = 0;
    for (n = 0; n < maxlen - 1; n++)
    {
        nc = read(fd, &buf[n], 1);
        if (nc <= 0)
            return nc;
        if (buf[n] == '\n')
            break;
    }
    buf[n + 1] = 0;
    return n + 1;
}

void receive_save_image(int connfd, char *file_name)
{
    // pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    // pthread_mutex_lock(&lock);

    size_t n;
    int total = 0;
    char buf[MAXLINE];
    FILE *image;
    // if (strcasecmp(file_name, "100.png") == 0)
    // {
    //     image = tmpfile();
    // }

    // else
    // {
    image = fopen(file_name, "w");
    // }

    if (image == NULL)
    {
        // change it to end gracefully
        exit(EXIT_FAILURE);
    }

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
    // printf("Image size received: %d\n", value);

    //Receive Image from Client
    while ((n = recv(connfd, buf, MAXLINE, 0)) > 0)
    {
        total += n;
        // printf("Server received %ld bytes, total is %d\n", n,total);
        fwrite(buf, 1, n, image);
        if (total >= value)
        {
            // printf("Image is here\n");
            break;
        }
    }

    fclose(image);
}
