#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFF_SIZE 1024

void *send_fc(void *argumentPointer){
    int client_socket = *reinterpret_cast<int*>(argumentPointer);
    char message[BUFF_SIZE];

    while(true){
        memset(message,0,sizeof(message));
        printf("Client : ");
        fgets(message,BUFF_SIZE,stdin);

        ssize_t numBytes = send(client_socket, message, strlen(message), 0);
        if (numBytes == -1){
            printf("send() error");
            close(client_socket);
            exit(1);
        }
        if(strcmp(message,"quit\n") == 0){
            close(client_socket);
            exit(1);
        }
        sleep(1);
    }
}

void *recv_fc(void *argumentPointer){
    int client_socket = *reinterpret_cast<int*>(argumentPointer);
    char buffer[BUFF_SIZE];                            //make message recv buffer

    while(true){
        memset(buffer,0,sizeof(buffer));
        ssize_t numBytes = recv(client_socket, buffer, BUFF_SIZE, 0);
        if(numBytes == -1 || numBytes == 0){
            printf("recv fail \n");
            exit(1);
        }
        printf("echo_server : %s",buffer);
    }
}

void usage() {
    printf("syntax: echo_client <server_ip> <port>\n");
    printf("sample: echo_client 127.0.0.1 1996\n");
}

int main(int argc, char *argv[]){
    if(argc != 3){
        usage();
        return -1;
    }

    int client_socket;
    struct sockaddr_in server_addr;
    pthread_t send_thread, recv_thread;

    client_socket = socket(PF_INET,SOCK_STREAM,0);
    if(client_socket == -1){
        printf("socket make fail\n");
        exit(1);
    }

    memset(&server_addr,0,sizeof(server_addr));

    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port        = htons(atoi(argv[2]));

    if(connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        printf("Can't connect\n");
        close(client_socket);
        exit(1);
    }

    pthread_create(&send_thread, nullptr, send_fc, static_cast<void*>(&client_socket));
    pthread_create(&recv_thread, nullptr, recv_fc, static_cast<void*>(&client_socket));
    pthread_join(send_thread, nullptr);
    pthread_join(recv_thread, nullptr);

    close(client_socket);
    return 0;
}
