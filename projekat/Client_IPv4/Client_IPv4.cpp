// TCP CLIENT da project BOIII

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "conio.h"
#include "Decription.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define SERVER_IP_ADDRESS "127.0.0.1"		// IPv4 address of server
#define SERVER_PORT 27015					// Port number of server that will be used for communication with clients
#define BUFFER_SIZE 512						// Size of buffer that will be used for sending and receiving messages to client
#define SIZE 1024							//buffer size for sending, recieving data



void write_file(char* filename, int sockfd) {
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
	sockaddr_in serverAddress, serverAddress1, serverAddress2;

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

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	// Check if socket creation succeeded
	if (clientSocket == INVALID_SOCKET)
	{
		printf("Creating socket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Initialize memory for address structure
	memset((char*)&serverAddress, 0, sizeof(serverAddress));

	serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(SERVER_PORT);

	//Connect to remote server
	if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
	{
		perror("connect failed. Error");
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	puts("Connected to Server\n");

	int PORT1 = -1;
	char request = '1'; //want the port numb thx

	iResult = send(clientSocket, &request, sizeof(char), 0);
	if (iResult == SOCKET_ERROR)
	{
		puts("Send failed");
		return 1;
	}

	//recv port
	iResult = recv(clientSocket, (char*)&PORT1, sizeof(int), 0);
	if (iResult == SOCKET_ERROR)
	{
		puts("recieve failed\n");
	}

	printf("WE RECIEVED A PORT %i\n", PORT1);

	// Close client application
	iResult = closesocket(clientSocket);
	if (iResult == SOCKET_ERROR)
	{
		printf("clientSocket2 failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	printf("first socket closed\n");

	///////////////////////////////LETS CONNECT TO NEW CHANNEL BBY///////////////////////////////
	// Initialize memory for address structure

	SOCKET clientSocket1 = socket(AF_INET, SOCK_STREAM, 0);

	// Check if socket creation succeeded
	if (clientSocket1 == INVALID_SOCKET)
	{
		printf("Creating socket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	memset((char*)&serverAddress1, 0, sizeof(serverAddress1));

	serverAddress1.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
	serverAddress1.sin_family = AF_INET;
	serverAddress1.sin_port = htons(PORT1);

	//Connect to remote server
	if (connect(clientSocket1, (struct sockaddr*)&serverAddress1, sizeof(serverAddress1)) < 0)
	{
		perror("connect failed. Error");
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

	write_file(encripted, clientSocket1);
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
		printf("clientSocket2 failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Close Winsock library
	WSACleanup();

	// Client has succesfully sent a message
	return 0;
}
