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
#include <list>

#define BUFF_SIZE 1024

using namespace std;

static std::list<int> lt;
static std::list<int>::iterator iter;

void *recv_fc(void *argumentPointer){
    int client_socket = *reinterpret_cast<int*>(argumentPointer);
    char buffer[BUFF_SIZE];

    while(true){
        memset(buffer,0,sizeof(buffer));
        ssize_t numBytes = recv(client_socket, buffer, BUFF_SIZE, 0);
        if(numBytes == -1 || numBytes == 0){
            printf("=====client_socket %d down=====\n",client_socket);
            //lt.erase(iter);
            close(client_socket);
            break;
        }
        printf("Client_socket %d : %s",client_socket ,buffer);

        for(iter=lt.begin();iter !=lt.end();iter++)
            send(*iter, buffer, strlen(buffer), 0);
    }
}

void usage() {
    printf("syntax: echo_server <port> (option)\n");
    printf("sample: echo_server 1996 (-b)\n");
}

int main(int argc, char *argv[]){
    if(argc != 2){
        usage();
        return -1;
    }

    struct sockaddr_in server_addr, client_addr;
    int server_socket, client_socket;                               //fd = socket
    int len = sizeof(client_addr);
    pthread_t recv_thread;
    char temp[20];

    server_socket = socket(AF_INET,SOCK_STREAM,0);
    if(server_socket == -1){  //socket(IPv4, TCP, option 0)
        printf("Server: can not Open Socket\n");
        exit(0);
    }

    memset(&server_addr,0x00,sizeof(server_addr));          //server_Addr 을 NULL로 초기화

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(uint16_t(atoi(argv[1])));

    if(bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr))< 0){
        printf("Server: cat not bind local addrss\n");      //소켓을 할당 받으면 소켓에 주소, 프로토콜, 포트가 할당된다.
        exit(0);                                        //bind(할당받을 socket, (sockaddr *)소켓의 주소정보, 구조체의 크기)
    }

    if(listen(server_socket,5) < 0){                            //listen(소켓, 접속가능한 클라이언트 수)
        printf("Server: cat not listen connnect.\n");
        exit(0);
    }

    printf("=====[PORT] : %d =====\n",atoi(argv[1]));
    printf("=====Server : wating connection request.=====\n");


    while(true){
        lt.push_back(accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&len));
        client_socket = lt.back();
        if (lt.back() < 0){             // accept 기다리는 과정       accept(소켓, (sockaddr *)클라이언트의 주소를 저장할 구조체, 구조체의 크기)
            printf("Server: accept failed\n");
            exit(0);
        }

        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, temp, sizeof(temp));
        printf("=====Server: %s client connect, socket %d=====\n", temp, client_socket);

        pthread_create(&recv_thread, nullptr, recv_fc, static_cast<void*>(&client_socket));

        //pthread_join(recv_thread, nullptr);
    }

    return 0;
}
