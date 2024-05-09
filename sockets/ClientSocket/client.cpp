#include <iostream>
#include <winsock2.h>
#include <cstdlib>
#include <cstdio>
#include "./menu/menu.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

int main() {
    WSADATA wsaData;
    SOCKET s;
    struct sockaddr_in server;
    char sendBuff[512], recvBuff[512];

    std::cout << "\nInitialising Winsock...\n";
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed. Error Code : " << WSAGetLastError() << std::endl;
        return -1;
    }

    std::cout << "Initialised.\n";

    // SOCKET creation
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Could not create socket : " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    std::cout << "Socket created.\n";

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    // CONNECT to remote server
    if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Connection error: " << WSAGetLastError() << std::endl;
        closesocket(s);
        WSACleanup();
        return -1;
    }

    std::cout << "Connection established with: " << inet_ntoa(server.sin_addr)
              << " (" << ntohs(server.sin_port) << ")\n";

    // RECEIVE and SEND data
    do {
        // RECEIVE menu message from server
        int bytes = recv(s, recvBuff, sizeof(recvBuff), 0);
        if (bytes > 0) {
            // Show menu to user
            std::cout << "Menu received from server: \n" << recvBuff << std::endl;

            // Get user selection
            std::cout << "Enter your selection: ";
            std::cin.getline(sendBuff, sizeof(sendBuff));

            // Send user selection to server
            send(s, sendBuff, strlen(sendBuff), 0);

            // Receive and display server response
            bytes = recv(s, recvBuff, sizeof(recvBuff), 0);
            if (bytes > 0) {
                std::cout << "Server response: " << recvBuff << std::endl;
            }
        }
    } while (strcmp(sendBuff, "0") != 0); // Exit loop if user enters "0"

    // CLOSING the socket and cleaning Winsock...
    closesocket(s);
    WSACleanup();

    return 0;
}
