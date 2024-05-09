// IMPORTANT: Winsock Library ("ws2_32") should be linked

#include <stdio.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

int main(int argc, char *argv[]) {

	WSADATA wsaData;
	SOCKET s;
	struct sockaddr_in server;
	char sendBuff[512], recvBuff[512];

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		return -1;
	}

	printf("Initialised.\n");

	//SOCKET creation
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//CONNECT to remote server
	if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
		printf("Connection error: %d", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return -1;
	}

	printf("Connection stablished with: %s (%d)\n", inet_ntoa(server.sin_addr),
			ntohs(server.sin_port));

// RECEIVE and SEND data
    do {
        // RECEIVE menu message from server
        int bytes = recv(s, recvBuff, sizeof(recvBuff), 0);
        if (bytes > 0) {
            // Show menu to user
            printf("Menu received from server: \n%s\n", recvBuff);

            // Get user selection
            printf("Enter your selection: ");
            fgets(sendBuff, sizeof(sendBuff), stdin);
            sendBuff[strcspn(sendBuff, "\n")] = '\0'; // Remove newline character

            // Send user selection to server
            send(s, sendBuff, strlen(sendBuff), 0);

            // Receive and display server response
            bytes = recv(s, recvBuff, sizeof(recvBuff), 0);
            if (bytes > 0) {
                printf("Server response: %s\n", recvBuff);
            }
        }
    } while (strcmp(sendBuff, "0") != 0); // Exit loop if user enters "0"

	// CLOSING the socket and cleaning Winsock...
	closesocket(s);
	WSACleanup();

	return 0;
}
