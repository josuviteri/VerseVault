#include <iostream>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

int main(int argc, char *argv[]) {

    WSADATA wsaData;
    SOCKET conn_socket;
    SOCKET comm_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char sendBuff[512], recvBuff[512];

    std::cout << "\nInitialising Winsock...\n";
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "Failed. Error Code : " << WSAGetLastError() << std::endl;
        return -1;
    }

    std::cout << "Initialised.\n";

    //SOCKET creation
    if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cout << "Could not create socket : " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    std::cout << "Socket created.\n";

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    //BIND (the IP/port with socket)
    if (bind(conn_socket, (struct sockaddr*) &server,
            sizeof(server)) == SOCKET_ERROR) {
        std::cout << "Bind failed with error code: " << WSAGetLastError() << std::endl;
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    std::cout << "Bind done.\n";

    //LISTEN to incoming connections (socket server moves to listening mode)
    if (listen(conn_socket, 1) == SOCKET_ERROR) {
        std::cout << "Listen failed with error code: " << WSAGetLastError() << std::endl;
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    //ACCEPT incoming connections (server keeps waiting for them)
    std::cout << "Waiting for incoming connections...\n";
    int stsize = sizeof(struct sockaddr);
    comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
    // Using comm_socket is able to send/receive data to/from connected client
    if (comm_socket == INVALID_SOCKET) {
        std::cout << "accept failed with error code : " << WSAGetLastError() << std::endl;
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }
    std::cout << "Incoming connection from: " << inet_ntoa(client.sin_addr) << " (" << ntohs(client.sin_port) << ")" << std::endl;

    // Closing the listening sockets (is not going to be used anymore)
    closesocket(conn_socket);

    //SEND and RECEIVE data
    std::cout << "Waiting for incoming messages from client... \n";
    do {
        // Enviar el mensaje del menú al cliente
        char menuMsg[512] = "1. Option 1\n2. Option 2\n3. Option 3\n0. Exit\nSelect an option: ";
        send(comm_socket, menuMsg, strlen(menuMsg), 0);

        // Recibir la selección del cliente
        int bytes = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        if (bytes > 0) {
            std::cout << "Receiving message... \n";
            std::cout << "Data received: " << recvBuff << std::endl;

            // Procesar la selección del cliente y enviar una respuesta
            int selection = atoi(recvBuff);
            switch (selection) {
                case 1:
                    strcpy(sendBuff, "You selected Option 1");
                    break;
                case 2:
                    strcpy(sendBuff, "You selected Option 2");
                    break;
                case 3:
                    strcpy(sendBuff, "You selected Option 3");
                    break;
                default:
                    strcpy(sendBuff, "Invalid selection");
                    break;
            }
            send(comm_socket, sendBuff, strlen(sendBuff), 0);
            std::cout << "Data sent: " << sendBuff << std::endl;

            if (selection == 0) // Si el cliente envía "0", salir del bucle
                break;
        }
    } while (1);

    // CLOSING the sockets and cleaning Winsock...
    closesocket(comm_socket);
    WSACleanup();

    return 0;
}
