#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include <string.h>
#include<pthread.h>
#include<iostream>
#include <cstring>
#include <ctime>
#define MAX 10
using namespace std;

#define LOCAL_IP "127.0.0.1"
#define LOCAL_PORT 1234
struct user{int sock;string name;};

void* receive(void* arg){
    int *temp=((int*)arg);
    int sock=*temp;
    while(true){
        char recvBuf[1024] = {};
        int reLen = recv(sock, recvBuf, 1024, 0);
        if(reLen>0){
        time_t result = time(NULL);
        cout<<recvBuf<<"\t"<<asctime(localtime(&result));
        }
        else{
        printf("the server is unavaliable!");
        break;
        }
    }
    pthread_exit(NULL);
}

int main()
{
    int sock = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(LOCAL_PORT);
    server.sin_addr.s_addr = inet_addr(LOCAL_IP);
    string name;
    cout<<"What is your name?\n";
    getline(cin,name);
    if(connect(sock,(struct sockaddr *)&server,sizeof(server)) < 0)
    {
        perror("connect");
        return 2;
    }
    write(sock,(char*)name.c_str(),name.length());

    printf("connect sucessfully!\n");
    printf("start to chat with friends\n");
    printf("********************************\n");

    void* temp=&sock;
    pthread_t th;
    pthread_create(&th,NULL,receive,temp);

    while(true){
        string s;
        getline(cin,s);
        write(sock,(char*)s.c_str(),s.length());
    }

    close(sock);
    return 0;
}

