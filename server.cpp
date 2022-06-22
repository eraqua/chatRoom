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
#include<vector>
using namespace std;

#define LOCAL_IP "127.0.0.1" //looback ip;
#define LOCAL_PORT 1234  //port
#define MAX 10

pthread_mutex_t mutex;      //Define a mutex
string lastmessage;         //string to save the last message
struct user{int sock;string name;}; //struct to diliver the parameters of clients
vector<int> socks;          //save the clients' socks
//
int start_up(const char* local_ip,int local_port)
{

    cout<<"*************************"<<endl;
	int sock = socket(AF_INET,SOCK_STREAM,0);//1.build socket
	if(sock < 0){
		perror("socket");
		exit(1);
	}
	struct sockaddr_in local;//2.bind the socket to IP and PORT
	local.sin_family = AF_INET;
	local.sin_port = htons(local_port);
	local.sin_addr.s_addr = inet_addr(local_ip);
	const int on=1;
	
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));//set bind options to avoid error
	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)//3.bind socket
	{
		perror("bind");
		exit(2);
	}
	else{cout<<"Bind success.\n";}
	if(listen(sock,MAX) < 0)
	{
		perror("listen");
		exit(3);
	}
	else{cout<<"Listenning.\nstart successfully!\n"<<"ip:\t"<<LOCAL_IP<<"\nport:\t"<<LOCAL_PORT<<endl;}
    cout<<"*************************"<<endl;

	return sock;
}
void * HandlerRequest(void *arg)
{
    pthread_mutex_lock(&mutex);//lock
    user* t=(user*)arg;
    int new_sock=t->sock;
    string idname=t->name;
    while(true){
        char recvBuf[4024] = {};
        int rRes=recv(new_sock, recvBuf, 4024, 0);
        if(rRes>0)
            cout<<idname<<"(socketID-"<<new_sock<<")  :"<<recvBuf<<"\n";
        else{
            cout<<idname<<"(socketID: "<<new_sock<<")"<<" is log out;current online:"<<socks.size()-1<<endl;
            //bug fixed:remove expired sock;

            lastmessage=idname+" is log out";
        for(auto i:socks){//send last message to non-current socket
            if(i!=new_sock){
                send(i,(char*)lastmessage.c_str(),lastmessage.length(),0);
            }
        }
            for(auto it = socks.begin();it!=socks.end();){
                if(*it == new_sock)
                socks.erase(it);
                else{
                    it++;
                }
            }
            break;
        }
        lastmessage=idname+": "+recvBuf;
        
        for(auto i:socks){//send last message to non-current socket
            if(i!=new_sock){
                send(i,(char*)lastmessage.c_str(),lastmessage.length(),0);
            }
        }
    }
    close(new_sock);
    pthread_mutex_unlock(&mutex);//unlock
	pthread_exit(NULL);
}
int main()
{
	int sock = start_up(LOCAL_IP,LOCAL_PORT);
	
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	while(true){
        pthread_mutex_init(&mutex, NULL);
        int new_sock = accept(sock,(struct sockaddr *)&client,&len);

        char recvBuf[4024] = {};
        if(new_sock>0){
            recv(new_sock, recvBuf, 4024, 0);

            cout<<"----------------------"<<endl;
            cout<<recvBuf<<" is connected."<<"( socket: "<<new_sock<<" )\n";
            socks.push_back(new_sock);
            cout<<socks.size()<<" clients connected."<<endl;
            cout<<"----------------------"<<endl;
        }
        else{cout<<"Failed to accept."<<endl;}
        user t;
        t.name=recvBuf;t.sock=new_sock;
        pthread_t id;
        pthread_create(&id,NULL,HandlerRequest,&t);
        pthread_mutex_destroy(&mutex);
	}
    close(sock);
	return 0;
}

