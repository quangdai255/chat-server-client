//============================================================================
// Name        : ChatServer.cpp
// Author      : Duy Nguyen
// Version     :
// Copyright   : Open Source
// Description : Hello World in C++, Ansi-style
//============================================================================

//Tham khao: https://github.com/robelsharma/MultiClientChatRoom/blob/master/Server/Server/ServerManager.cpp

//----Bo thu vien C/C++----
#include <iostream>
#include <string>

#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <pthread.h>
//----------------------------------------

//----Using namespace----
using namespace std;
//----------------------------------------

//----Cac gia tri define----
#define BACKLOG 10
//----------------------------------------

//----Global Variables----
char *hostIP = "127.0.0.1";
int hostPort = 9001;
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
//----------------------------------------



void* requestHandler(void* sockfd)
{
	// KICH BAN
	//Xu ly cu the cho tung client
	// Nhan thong diep HELO
	// Goi loai ma hoa CRYP
	// Xac nhan ket noi
	// Goi danh sach client (10s/lan)
	// Nhan yeu cau ket noi toi 1 client khac de chat (1 - 1)
		// Viet class chat session

		// Thiet lap ket noi giua 2 client
		// Goi yeu cau ket noi toi tu client 1 -> client 2
			//Nhan phan hoi: OK hoac Failed
		// Goi ket qua phan hoi ve cho client 1 (gia su la ok)
		// Tao 1 ket noi trung gian, truyen tin giua 2 client nay
		//	MESS - Send Message.
		//		Cú pháp: MESS <hexa_text>
		//		Ví dụ: MESS <a1cd3e76b>

		// Dong ket noi neu nhan dc yeu cau exit


	int byte_sent = -1;
	int byte_recv = -1;
	int len;
	int socket = (int)sockfd;
	char *cmdBuffer[128];
	char *dataBuffer[8192];
	printf("Handle_request() with socket: %d\n",socket);

	//Nhan thong diep hello, neu ko co thong diep nay thi se tu tat
	//GHi log lai
	recv(socket,cmdBuffer,sizeof(cmdBuffer),0);
	cout << "Cmd from Client: " + cmdBuffer;

	//cmd: HELO
	if((cmdBuffer != "HELO"))
	{
		//GHi log lai
		goto EXIT;
	}

	cmdBuffer = "CRYP AES";
	//cmd: CRYP
	send(socket,cmdBuffer,sizeof(cmdBuffer),0);

	//Them khuc check loi o day
	recv(socket,cmdBuffer,sizeof(cmdBuffer),0);

	//cmd: ACK xac nhan ket noi nay
	if(cmdBuffer != "ACK")
	{
		//GHi log lai
		goto EXIT;
	}

	//else -> ok ngon lanh, cho nhan command tu client
	// "Giu nguyen ket noi nay"
	while(true)
	{

	}

	EXIT: cout << "Client exit\n";
	close(socket);
}

int acceptorLoop(char *hostIP, int hostPort)
{
	//Nguyen cai nay se chay trong 1 process!!!
	// fork(); !!!
	//while(true)
	//Ma gia
	//Tao socket(IP,Port)
	int retcode;
	int server_socket;
	struct addrinfo hints, *res;
	memset(&hints,0,sizeof hints);

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	char tempStr[10];
	sprintf(tempStr,"%ld",hostPort);
	cout << tempStr;
	getaddrinfo(hostIP,tempStr,&hints,&res);

	if( (retcode = getaddrinfo(hostIP,tempStr,&hints,&res)) != 0 )
	{
		//Add log
		//fprintf
		cerr << "Failed at getaddrinfo(): %s\n", gai_strerror(retcode);
		return -1;
	}

	server_socket = socket(res->ai_family,res->ai_socktype, res->ai_protocol);
	if(server_socket == -1)
	{
		//Add log
		//fprintf
		cerr << "Failed to create socket\n";
		return -1;
	}

	if ( (retcode = bind(server_socket,res->ai_addr, res->ai_addrlen)) == -1)
	{
		//Add log
		//fprintf
		cerr << "Failed at bind(): %s\n";
		close(server);
		return -1;
	}

	listen(server_socket, BACKLOG);
	int client_socket;

	pthread_t thread_id;
	//Dem so luong ket noi
	int count = 0;
	//Accept ket noi tu day
	while(true)
	{
		struct sockaddr_storage client_addr;
		socklen_t len;
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &len);

		if(client_socket < 0)
		{
			printf("Failed at accept()\n");
			exit(-1);

		}
		printf("Client #%d connected\n",count++);

		printf("Ticket client socket: %d\n",client_socket);

		//Moi Connection moi se la 1 socket/thread/connection
			// => viet ham xu ly rieng tung socket (acceptHandler!!)
			// Ham xu ly rieng tung socket

		//If nhan dc signal thi se out vong lap, dong tat ca cac ket noi

		//Thread -> handle_request
		if (pthread_create(&thread_id, NULL, requestHandler, (void*) client_socket) < 0)
		{
			printf("Failed when created thread\n");
			exit(1);
		}
		//Join thread
		pthread_join(thread_id, NULL);
	};





}


int main() {
	cout << "---Chat Server---" << endl;
	cout << "Chat server started...";

	return 0;
}
