//Basic code from Microsoft Developer Network instructions on TCP connections
//Modifications made to what is being returned to the client, (adding "Polly says"
//as a way of verifying that no sort of echoing is occurring, the use of the port
//numbers to be a little easier to connect with, and the output -- namely that
//the server now prints out the address it has connected to as well as what
//specific data it was sent by the client upon connection.
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

struct SocketInfo{
	SOCKET ClientOn;
	IN_ADDR address;
	USHORT port;
	int performance;
};

struct DV{
	char *address;
};

int iResult;
SOCKET ListenSocket;
SocketInfo sockets[10];
struct sockaddr_in connection; //Added to track what the connection was made to
int conlen;
int numSockets = 0;

int __cdecl main(void) 
{
    WSADATA wsaData;
    
	
	int option = -1;
	int socket;
	for(int i = 0;i < 10;i++)
	{
		sockets[i].ClientOn = INVALID_SOCKET;
		sockets[i].performance = 9999;
	}
    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }
	do{
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }
	
    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);
	do{
		printf("Please select an option:\n");
		printf("[0] Contact another server\n");
		printf("[1] Listen for another server\n");
		printf("[2] Show Distance Vector Table\n");
		printf("[3] Update Distance Vector Table\n");
		printf("[4] Quit\n");
		printf("(%d active connections\n", numSockets);
		scanf("%i", &option);
		switch(option){
		case 0:
				socket = MakeConnection();
				 // Send an initial buffer
				sprintf(sendbuf, "M %s %d\n", inet_ntoa(ip4->sin_addr), ip4->sin_port);
				iResult = send( sockets[socket].ClientOn, sendbuf, (int)strlen(sendbuf), 0 );
				if (iResult == SOCKET_ERROR) {
					printf("send failed with error: %d\n", WSAGetLastError());
					closesocket(sockets[socket].ClientOn);
					WSACleanup();
					return 1;
				}
				printf("Sent: %s\n", sendbuf);
				iResult = recvfrom(sockets[numSockets].ClientOn, recvbuf, recvbuflen, 0, (sockaddr *)&connection, &conlen);
				if (iResult > 0) {
					recvbuf[iResult] = '\0';
					printf("Received: %s\n", recvbuf);
					// Echo the buffer back to the sender with an additional "Polly says: "
					sprintf(sendbuf, "I %s", recvbuf);
					iSendResult = send( sockets[numSockets].ClientOn, sendbuf, (int)strlen(sendbuf), 0 );
					if (iSendResult == SOCKET_ERROR) {
						printf("send failed with error: %d\n", WSAGetLastError());
						closesocket(sockets[numSockets].ClientOn);
						WSACleanup();
						return 1;
					}
					printf("Sent: %s\n", sendbuf);
					printf("Bytes sent: %d\n", iSendResult);
				}
				else if (iResult == 0)
					printf("Connection closing...\n");
				else  {
					printf("recv failed with error: %d\n", WSAGetLastError());
					closesocket(sockets[numSockets].ClientOn);
					WSACleanup();
					return 1;
				}
				break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			printf("That is an unacceptable input.");
			break;
			}
			
		}
	}while(true);

    // No longer need server socket
    closesocket(ListenSocket);

    // Receive until the peer shuts down the connection
    do {
        iResult = recvfrom(ClientSocket, recvbuf, recvbuflen, 0, (sockaddr *)&connection, &conlen);
        if (iResult > 0) {
            recvbuf[iResult] = '\0';
			printf("Received: %s\n", recvbuf);
			printf("Bytes received: %d\n", iResult);
			// Echo the buffer back to the sender with an additional "Polly says: "
			sprintf(sendbuf, "Polly says: %s", recvbuf);
            iSendResult = send( ClientSocket, sendbuf, (int)strlen(sendbuf), 0 );
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
			printf("Sent: %s\n", sendbuf);
			printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}

int MakeConnection()
{
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	conlen = sizeof(connection);
	// Accept a client socket
	sockets[numSockets].ClientOn = accept(ListenSocket, (sockaddr *)&connection, &conlen);
	if (sockets[numSockets].ClientOn == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return -1;
	}else{
		printf("Accepting Connection From: %s:%d\n", inet_ntoa(connection.sin_addr), connection.sin_port);
		numSockets++;
		return numSockets-1;
	}
}