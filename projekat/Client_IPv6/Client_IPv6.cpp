// UDP client that uses blocking sockets

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "conio.h"
#include "Decription.h"

#if defined(_MSC_VER) && _MSC_VER >= 1900 && !defined(_CRT_NO_TIME_T)
#define _TIMESPEC_DEFINED
#endif
#include <pthread.h>
#pragma comment(lib,"pthreadVC2.lib")

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define SERVER_IP_ADDRESS "0:0:0:0:0:0:0:1"	// IPv6 address of server in localhost
#define SERVER_PORT 27015					// Port number of server that will be used for communication with clients
#define BUFFER_SIZE 512						// Size of buffer that will be used for sending and receiving messages to client
#define SIZE 1024


void WriteFile(char* filename, int sockfd) {
	int n;
	FILE *fp;
	char buffer[SIZE];

	fp = fopen(filename, "w");
	while (1) {
		n = recv(sockfd, buffer, SIZE, 0);
		if (n <= 0) {
			break;
			return;
		}
		fprintf(fp, "%s", buffer);
		memset(buffer, 0, SIZE);
	}
	fclose(fp);
	return;
}

int main()
{
	// Server address structure
	sockaddr_in6 serverAddress, serverAddress2;

	// Size of server address structure
	int sockAddrLen = sizeof(serverAddress);

	// WSADATA data structure that is used to receive details of the Windows Sockets implementation
	WSADATA wsaData;

	// Initialize windows sockets for this process
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	// Check if library is succesfully initialized
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	// Initialize memory for address structure
	memset((char*)&serverAddress, 0, sizeof(serverAddress));

	// Initialize address structure of server
	serverAddress.sin6_family = AF_INET6;								// IPv6 address famly
	inet_pton(AF_INET6, SERVER_IP_ADDRESS, &serverAddress.sin6_addr);	// Set server IP address using string
	serverAddress.sin6_port = htons(SERVER_PORT);						// Set server port
	serverAddress.sin6_flowinfo = 0;									// flow info


	// Create a socket

	SOCKET clientSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	// Check if socket creation succeeded
	if (clientSocket == INVALID_SOCKET)
	{
		printf("Creating socket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) < 0)
	{
		perror("connect failed. Error");
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	puts("Connected to Server\n");

	int PORT = -1;
	char request = '1'; //want the port numb thx
	//int numbOports = 2;

	iResult = send(clientSocket, &request, sizeof(char), 0);
	if (iResult == SOCKET_ERROR)
	{
		puts("Send failed");
		return 1;
	}

	//recv port
	iResult = recv(clientSocket, (char*)&PORT, sizeof(int), 0);
	if (iResult == SOCKET_ERROR)
	{
		puts("recieve failed\n");
	}

	printf("WE RECIEVED A PORT %i\n", PORT);

	// Close client application
	iResult = closesocket(clientSocket);
	if (iResult == SOCKET_ERROR)
	{
		printf("closing clientSocket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	printf("first socket closed\n");

	///////////////////////////////LETS CONNECT TO NEW CHANNEL BBY///////////////////////////////

	SOCKET clientSocket1 = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);

	// Check if socket creation succeeded
	if (clientSocket1 == INVALID_SOCKET)
	{
		printf("Creating socket 2 failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Initialize memory for address structure
	memset((char*)&serverAddress2, 0, sizeof(serverAddress2));

	// Initialize address structure of server
	serverAddress2.sin6_family = AF_INET6;								// IPv6 address famly
	inet_pton(AF_INET6, SERVER_IP_ADDRESS, &serverAddress2.sin6_addr);	// Set server IP address using string
	serverAddress2.sin6_port = htons(PORT);						// Set server port
	serverAddress2.sin6_flowinfo = 0;									// flow info

	if (connect(clientSocket1, (SOCKADDR*)&serverAddress2, sizeof(serverAddress2)) < 0)
	{
		perror("connect 2 failed. Error");
		closesocket(clientSocket1);
		WSACleanup();
		return 1;
	}
	puts("Connected to Server\n");

	request = '2'; // DOWNLOAD BIACH

	iResult = send(clientSocket1, &request, sizeof(char), 0);
	if (iResult == SOCKET_ERROR)
	{
		puts("Send failed");
		return 1;
	}

	/////////////////////////RETRIEVE ZE FILE///////////////////////////////


	char *encripted = "recvEncripted.txt";
	char *filename = "recv.txt";

	WriteFile(encripted, clientSocket1);
	printf("Download complete\n\n");

	printf("Starting decription!\n");
	MonoalphabeticDecipher(encripted, filename);
	printf("Decription DONE!\n");

	if (remove(encripted) == 0) {
		printf("The file is deleted successfully.\n");
	}
	else {
		printf("The file is not deleted.\n");
	}

	// Close client application
	iResult = closesocket(clientSocket1);
	if (iResult == SOCKET_ERROR)
	{
		printf("clientSocket1 failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Close Winsock library
	WSACleanup();

	// Client has succesfully sent a message
	return 0;
}

