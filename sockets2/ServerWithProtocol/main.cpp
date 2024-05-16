#include <stdio.h>
#include <winsock2.h>
#include <math.h>
#include "../../structs/cliente.h"
#include "../sqlite/sqlite3.h"
#include "../database/db2.h"
#include "../menu/menu.h"

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
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        return -1;
    }

    printf("Initialised.\n");

    if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if (bind(conn_socket, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d\n", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    printf("Bind done.\n");

    if (listen(conn_socket, 1) == SOCKET_ERROR) {
        printf("Listen failed with error code: %d\n", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    printf("Waiting for incoming connections...\n");
    int stsize = sizeof(struct sockaddr);
    comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
    if (comm_socket == INVALID_SOCKET) {
        printf("accept failed with error code : %d\n", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }
    printf("Incoming connection from: %s (%d)\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    closesocket(conn_socket);

    printf("Waiting for incoming commands from client... \n");
    do {
        memset(recvBuff, 0, sizeof(recvBuff));
        int recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        if (recv_size <= 0) {
            perror("Error al recibir comando");
            break;
        }

        printf("Command received: %s \n", recvBuff);

        if (strcmp(recvBuff, "INICIAR-SESION") == 0) {
            Cliente cl;
            memset(&cl, 0, sizeof(Cliente)); // Inicializa todo a cero

            recv_size = recv(comm_socket, cl.email_cl, sizeof(cl.email_cl) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir email");
                continue;
            }
            cl.email_cl[recv_size] = '\0'; // Asegura terminador nulo

            recv_size = recv(comm_socket, cl.passw, sizeof(cl.passw) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir contraseña");
                continue;
            }
            cl.passw[recv_size] = '\0'; // Asegura terminador nulo

            cl.id_Cliente = iniciarSesionMenu(cl.email_cl, cl.passw);

            /*printf("\nhola soy : %i\n", cl.id_Cliente);
            printf("\nCorreo soy : %s\n", cl.email_cl);
            printf("\npass soy : %s\n", cl.passw);
            */

            memset(sendBuff, 0, sizeof(sendBuff));
            sprintf(sendBuff, "%d", cl.id_Cliente);
            if (send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0) == -1) {
                perror("Error al enviar ID de cliente");
            }

            memset(recvBuff, 0, sizeof(recvBuff));
            recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            if (recv_size <= 0) {
                perror("Error al recibir INICIAR-SESION-END");
                continue;
            }

            if (strcmp(recvBuff, "INICIAR-SESION-END") != 0) {
                printf("Error: Comando INICIAR-SESION-END no recibido correctamente\n");
            }
        }
        if (strcmp(recvBuff, "REGISTRAR") == 0) {
            Cliente cl;
            memset(&cl, 0, sizeof(Cliente)); // Inicializa todo a cero


            recv_size = recv(comm_socket, cl.nom_Cliente, sizeof(cl.nom_Cliente) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir nom_Cliente");
                continue;
            }
            cl.nom_Cliente[recv_size] = '\0'; // Asegura terminador nulo


            recv_size = recv(comm_socket, cl.email_cl, sizeof(cl.email_cl) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir email");
                continue;
            }
            cl.email_cl[recv_size] = '\0'; // Asegura terminador nulo

            recv_size = recv(comm_socket, cl.passw, sizeof(cl.passw) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir contraseña");
                continue;
            }
            cl.passw[recv_size] = '\0'; // Asegura terminador nulo

            int registradoBien = 0;
            registradoBien = registrarClienteMenu(cl.nom_Cliente,cl.email_cl,cl.passw);

            /*printf("\nhola soy : %i\n", cl.id_Cliente);
            printf("\nCorreo soy : %s\n", cl.email_cl);
            printf("\npass soy : %s\n", cl.passw);
            */

            memset(sendBuff, 0, sizeof(sendBuff));
            sprintf(sendBuff, "%d", registradoBien);
            if (send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0) == -1) {
                perror("Error al enviar confiramcion Registro correcto");
            }

            memset(recvBuff, 0, sizeof(recvBuff));
            recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            if (recv_size <= 0) {
                perror("Error al recibir REGISTRAR-END");
                continue;
            }

            if (strcmp(recvBuff, "REGISTRAR-END") != 0) {
                printf("Error: Comando REGISTRAR-END no recibido correctamente\n");
            }
        }

        if (strcmp(recvBuff, "RAIZ") == 0) {
            memset(recvBuff, 0, sizeof(recvBuff));
            recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            if (recv_size <= 0) {
                perror("Error al recibir número para calcular raíz");
                continue;
            }
            int n = atoi(recvBuff);
            float raiz = sqrt(n);

            memset(recvBuff, 0, sizeof(recvBuff));
            recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            if (recv_size <= 0 || strcmp(recvBuff, "RAIZ-END") != 0) {
                perror("Error al recibir comando RAIZ-END");
                continue;
            }

            memset(sendBuff, 0, sizeof(sendBuff));
            sprintf(sendBuff, "%f", raiz);
            if (send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0) == -1) {
                perror("Error al enviar resultado de raíz");
            }
            printf("Response sent: %s \n", sendBuff);
        }

        if (strcmp(recvBuff, "IP") == 0) {
            memset(recvBuff, 0, sizeof(recvBuff));
            recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            if (recv_size <= 0 || strcmp(recvBuff, "IP-END") != 0) {
                perror("Error al recibir comando IP-END");
                continue;
            }

            memset(sendBuff, 0, sizeof(sendBuff));
            strcpy(sendBuff, inet_ntoa(server.sin_addr));
            if (send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0) == -1) {
                perror("Error al enviar IP");
            }
            printf("Response sent: %s \n", sendBuff);
        }

        if (strcmp(recvBuff, "EXIT") == 0)
            break;
    } while (1);

    closesocket(comm_socket);
    WSACleanup();

    return 0;
}
