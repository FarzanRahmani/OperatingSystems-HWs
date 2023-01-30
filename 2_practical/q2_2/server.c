#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>


// in your browser type: http://localhost:8090
// IF error: address in use then change the PORT number
#define PORT 9999

typedef struct {int new_socket; char *hello; char buffer[30000];int buffer_size;} myarg_t;

void *response_function(void *arg) {
    myarg_t *args = (myarg_t *) arg;
    int new_socket = args->new_socket;
    char *hello = args->hello;
    char *buffer = args->buffer;
    int buffer_size = args->buffer_size;
    long valread = read( new_socket , buffer, 30000);
    printf("%s\n",buffer ); // print request
    // uncomment following line and connect many clients
    sleep(5);
    write(new_socket , hello , strlen(hello)); // comes in browser
    printf("-------------Hello message sent---------------"); // write in cosole
    close(new_socket);
    // free(args);
    // free(buffer);
    return NULL;
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        pthread_t thread_id;
        char buffer[30000] = {0};
        myarg_t args = {new_socket, hello, buffer, 30000};
        pthread_create(&thread_id, NULL, &response_function, &args);

        /// -----------------------------------------
        // printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        // if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        // {
        //     perror("In accept");
        //     exit(EXIT_FAILURE);
        // }
        
        // char buffer[30000] = {0};
        // valread = read( new_socket , buffer, 30000);
        // printf("%s\n",buffer ); // print request
        // // uncomment following line and connect many clients
        // sleep(5);
        // write(new_socket , hello , strlen(hello)); // comes in browser
        // printf("-------------Hello message sent---------------"); // write in cosole
        // close(new_socket);
        // ------------------------------------------------------
    }
    return 0;
}

