// IMPORTANT: Winsock Library ("ws2_32") should be linked

#include <stdio.h>
#include <winsock2.h>
#include <math.h>
#include "../../structs/cliente.h"
#include "../sqlite/sqlite3.h"
#include "../database/db2.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000






int main(int argc, char *argv[]) {

	WSADATA wsaData;
	SOCKET conn_socket;
	SOCKET comm_socket;
	struct sockaddr_in server;
	struct sockaddr_in client;
	char sendBuff[512], recvBuff[512];

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		return -1;
	}

	printf("Initialised.\n");

	//SOCKET creation
	if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//BIND (the IP/port with socket)
	if (bind(conn_socket, (struct sockaddr*) &server,
			sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code: %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	printf("Bind done.\n");

	//LISTEN to incoming connections (socket server moves to listening mode)
	if (listen(conn_socket, 1) == SOCKET_ERROR) {
		printf("Listen failed with error code: %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	//ACCEPT incoming connections (server keeps waiting for them)
	printf("Waiting for incoming connections...\n");
	int stsize = sizeof(struct sockaddr);
	comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
	// Using comm_socket is able to send/receive data to/from connected client
	if (comm_socket == INVALID_SOCKET) {
		printf("accept failed with error code : %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}
	printf("Incomming connection from: %s (%d)\n", inet_ntoa(client.sin_addr),
			ntohs(client.sin_port));

	// Closing the listening sockets (is not going to be used anymore)
	closesocket(conn_socket);

	//SEND and RECEIVE data (CLIENT/SERVER PROTOCOL)
	printf("Waiting for incoming commands from client... \n");
	do
	{
		recv(comm_socket, recvBuff, sizeof(recvBuff), 0);

		printf("Command received: %s \n", recvBuff);


		if (strcmp(recvBuff, "INICIAR-SESION") == 0){
		sqlite3 *db;
		int rc = sqlite3_open("libreria.db", &db);
		if (rc != SQLITE_OK) {
			printf("Error opening database\n");
			return;
		}


		Cliente cl;
		
		// Recibir email como cadena
		recv(comm_socket, cl.email_cl, sizeof(cl.email_cl), 0);
		
		// Recibir contraseña como cadena
		recv(comm_socket, cl.passw, sizeof(cl.passw), 0);
		
		// Recibir INICIAR-SESION-END
		recv(comm_socket, recvBuff, sizeof(recvBuff), 0);

		// Llamar a la función iniciarSesion con el email y la contraseña
		cl.id_Cliente = iniciarSesion(db, cl.email_cl, cl.passw);
		
		// Enviar el resultado al cliente
		sprintf(sendBuff, "%d", cl.id_Cliente);
		send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);
		printf("Response sent: %s \n", sendBuff);

		rc = sqlite3_close(db);
		if (rc != SQLITE_OK) {
			printf("Error closing database\n");
			printf("%s\n", sqlite3_errmsg(db));
			return;
		}
	}

		if (strcmp(recvBuff, "RAIZ") == 0)
		{
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			int n = atoi(recvBuff);
			float raiz = sqrt(n);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			if (strcmp(recvBuff, "RAIZ-END") == 0); // Nada que hacer

			sprintf(sendBuff, "%f", raiz);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %s \n", sendBuff);
		}

		if (strcmp(recvBuff, "IP") == 0)
		{
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			if (strcmp(recvBuff, "IP-END") == 0); // Nada que hacer

			strcpy(sendBuff, inet_ntoa(server.sin_addr));
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %s \n", sendBuff);
		}

		if (strcmp(recvBuff, "EXIT") == 0)
			break;

	} while (1);

	// CLOSING the sockets and cleaning Winsock...
	closesocket(comm_socket);
	WSACleanup();

	return 0;
}

