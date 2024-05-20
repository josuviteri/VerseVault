//
// Created by asier on 20/05/2024.
//

#ifndef CLIENT_H
#define CLIENT_H

void leerLibro(SOCKET client_socket, const char* titulo);
void buscarLibro(SOCKET client_socket);
void imprimirInicial();
void imprimirGestionInvitado();

#endif //CLIENT_H
