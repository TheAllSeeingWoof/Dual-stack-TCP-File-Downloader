// Heeeeyoo this shoud be the project eeeeooo

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "conio.h"
#include "Encription.h"

#if defined(_MSC_VER) && _MSC_VER >= 1900 && !defined(_CRT_NO_TIME_T)
#define _TIMESPEC_DEFINED
#endif
#include <pthread.h>
#pragma comment(lib,"pthreadVC2.lib")

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define SERVER_PORT 27015	// Port number of server that will be used for communication with clients
#define SERVER_PORT2 27016
#define BUFFER_SIZE 512		// Size of buffer that will be used for sending and receiving messages to clients
#define NAME_SIZE 100
#define SIZE 1024

// Checks if ip address belongs to IPv4 address family
bool is_ipV4_address(sockaddr_in6 address);

pthread_mutex_t lock;

void SendFile(FILE *fp, int sockfd) {
	char data[SIZE] = { 0 };

	while (fgets(data, SIZE, fp) != NULL) {
		if (send(sockfd, data, sizeof(data), 0) == -1) {
			perror("[-]Error in sending file.");
			exit(1);
		}
		memset(data, 0, SIZE);
	}
	return;
}
//this should be where it gets decided how many connections for a file / sending the ports for them
void SendPort(void* client_socket) {
	
	int sock = *(int *)client_socket;
	int iResult;
	int portNum = SERVER_PORT2;

	// itt kellene hozzatenni h recv hany port and for(send as many) :C
	iResult = send(sock, (char*)&portNum, sizeof(int), 0);
	if (iResult == SOCKET_ERROR)
	{
		puts("Send failed");
	}


	//close socket
	printf("closing connection/socket\n");
	iResult = closesocket(sock);

	if (iResult == SOCKET_ERROR)
	{
		printf("closesocket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
	}

	return;
}

// the thread where the file gets sent
void StartDownload(void* client_socket) {
	
	int sock = *(int *)client_socket;
	int iResult;
	char *filename = "send1.txt";
	char *encripted = "encripted.txt";
	FILE *fp;

	MonoalphabeticCipher(filename, encripted);

	fp = fopen(encripted, "r");
	if (fp == NULL) {
		perror("Error in reading file.");
		exit(1);
	}

	SendFile(fp, sock);
	fclose(fp);
	printf("File sent! \n\n");

	if (remove(encripted) == 0) {
		printf("The file is deleted successfully.");
	}
	else {
		printf("The file is not deleted.");
	}

	printf("closing connection/socket\n");
	iResult = closesocket(sock);

	if (iResult == SOCKET_ERROR)
	{
		printf("closesocket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
	}

	return;
}

void* socketThread(void* clientSocket) {
	//Receive a message from client
	int sock = *(int *)clientSocket;
	int iResult;
	char request = '0';

	iResult = recv(sock, &request, sizeof(char), 0);
	if (iResult == SOCKET_ERROR)
	{
		puts("recieve failed");
	}
	else
	{
		switch (request)
		{
		case '1':
			SendPort(&sock);
			return NULL;

		case '2':
			StartDownload(&sock);
			return NULL;

		default:
			break;
		}
	}
	printf("closing connection/socket\n");
	iResult = closesocket(sock);

	if (iResult == SOCKET_ERROR)
	{
		printf("closesocket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
	}

	return NULL;
}

SOCKET NetworkAcceptAny(SOCKET fds[], unsigned int count, struct sockaddr *addr, socklen_t *addrlen)
{
	fd_set readfds;
	SOCKET maxfd, fd;
	unsigned int i;
	int status;

	FD_ZERO(&readfds);
	maxfd = -1;
	for (i = 0; i < count; i++) {
		FD_SET(fds[i], &readfds);
		if (fds[i] > maxfd)
			maxfd = fds[i];
	}
	status = select(maxfd + 1, &readfds, NULL, NULL, NULL);
	if (status < 0)
		return INVALID_SOCKET;
	fd = INVALID_SOCKET;
	for (i = 0; i < count; i++)
		if (FD_ISSET(fds[i], &readfds)) {
			fd = fds[i];
			break;
		}
	if (fd == INVALID_SOCKET)
		return INVALID_SOCKET;
	else
		return accept(fd, addr, addrlen);
}


int main() {

	int c;
	unsigned int count = 2;
	SOCKET socketDesc[2], clientSocket;
	//sockaddr_in server[2], client;

	sockaddr_in6  serverAddress[2], client;

	int iResult;

	// WSADATA data structure that is to receive details of the Windows Sockets implementation
	WSADATA wsaData;

	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return 1;
	}


	///////////////////////////SOCKETS/////////////////////////////////
	//Create socket
	socketDesc[0] = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if (socketDesc[0] == INVALID_SOCKET)
	{
		printf("Creating socket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	puts("Socket 1 created\n");

	socketDesc[1] = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if (socketDesc[1] == INVALID_SOCKET)
	{
		printf("Creating socket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	puts("Socket 2 created\n");

	///////////////////////////////////////////////////////

	char no[4] = { 0 };
	int ip6Result = setsockopt(socketDesc[0], IPPROTO_IPV6, IPV6_V6ONLY, (const char*)no, sizeof(no));

	if (ip6Result == SOCKET_ERROR)
	{
		printf("failed setsock 1 with error: %u\n", WSAGetLastError());
	}

	ip6Result = setsockopt(socketDesc[1], IPPROTO_IPV6, IPV6_V6ONLY, (const char*)no, sizeof(no));

	if (ip6Result == SOCKET_ERROR)
	{
		printf("failed setsock 2 with error: %u\n", WSAGetLastError());
	}

	//////////////////////////////////////////////////////////////////

	//Prepare the sockaddr_in structure
	memset((char*)&serverAddress[0], 0, sizeof(serverAddress[0]));
	serverAddress[0].sin6_family = AF_INET6;
	serverAddress[0].sin6_addr = in6addr_any;
	serverAddress[0].sin6_port = htons(SERVER_PORT);
	serverAddress[0].sin6_flowinfo = 0;				// flow info


	//Prepare the sockaddr_in structure
	memset((char*)&serverAddress[1], 0, sizeof(serverAddress[1]));
	serverAddress[1].sin6_family = AF_INET6;
	serverAddress[1].sin6_addr = in6addr_any;
	serverAddress[1].sin6_port = htons(SERVER_PORT2);
	serverAddress[1].sin6_flowinfo = 0;				// flow info


	//Bind
	iResult = bind(socketDesc[0], (SOCKADDR*)&(serverAddress[0]), sizeof(serverAddress[0]));
	if (iResult == SOCKET_ERROR)
	{
		printf("Socket 1 bind failed with error: %d\n", WSAGetLastError());
		closesocket(socketDesc[0]);
		WSACleanup();
		return 1;
	}
	puts("Bind socket 1 done\n");

	//Bind
	iResult = bind(socketDesc[1], (SOCKADDR*)&(serverAddress[1]), sizeof(serverAddress[1]));
	if (iResult == SOCKET_ERROR)
	{
		printf("Socket 2 bind failed with error: %d\n", WSAGetLastError());
		closesocket(socketDesc[1]);
		WSACleanup();
		return 1;
	}
	puts("Bind socket 2 done\n");

	//Listen
	if (listen(socketDesc[0], 80) == 0)
	{
		printf("Listening 1\n");
	}
	else
	{
		printf("Listening 1 error\n");
	}

	//Listen
	if (listen(socketDesc[1], 80) == 0)
	{
		printf("Listening 2\n");
	}
	else
	{
		printf("Listening 2 error\n");
	}



	pthread_t clientTh[100];
	int i = 0;
	c = sizeof(sockaddr_in6);

	while (1)
	{
		//Accept and incoming connection
		puts("Waiting for incoming connections...");

		clientSocket = NetworkAcceptAny(socketDesc, count, (SOCKADDR *)&client, (socklen_t*)&c);

		//accept connection from an incoming client
		if (clientSocket < 0)
		{
			perror("Accept failed");
			return 1;
		}
		puts("Connection accepted");

		if (pthread_create(&clientTh[i++], NULL, socketThread, &clientSocket) != 0)
		{
			printf("Failed to create thread. \n");
		}

		if (i >= 99)
		{
			i = 0;
			while (i < 100)
			{
				pthread_join(clientTh[i++], NULL);
			}
			i = 0;
		}
		
	}

	printf("closing connection/socket\n");
	iResult = closesocket(clientSocket);

	if (iResult == SOCKET_ERROR)
	{
		printf("closesocket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Close Winsock library
	WSACleanup();
	_getch();

	pthread_mutex_destroy(&lock);

	return 0;
}

