// IMPORTANT: Winsock Library ("ws2_32") should be linked

#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono> // Para utilizar std::this_thread::sleep_for
#include <thread> // Para utilizar std::chrono::milliseconds


#include "../../structs/cliente.h"
#include "../../structs/libro.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000
#define convPag 20
#include "client.h"
using namespace std;

#define NEGRITA "\e[1m" // Renombramos el código de los caracteres en negrita para que sea mas entendible
#define QUITAR_NEGRITA "\e[m" // Renombramos el codigo de quitar la negrita a los caracteres para que sea mas entendible
//"Sin dolor no hay gloria" - Proverbio Romano
SOCKET s;
char sendBuff[2048], recvBuff[2048];
int es_admin = 0;
Cliente cliente;


void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
char imprimirInicial1(){
	printf("\nProyecto Programacion IV | Grupo PVI-04\n\n");
    printf(NEGRITA"Bienvenido al Sistema de Libreria Virtual\nVerseVault\n\n" QUITAR_NEGRITA);

 	printf("\nSelecciona una opcion: \n");
    printf("1. Iniciar Sesion \n2. Registrar cuenta \n3. Continuar como invitado \n4. Salir\n\n");

	char opcion = getchar();

	char c; while ((c = getchar()) != '\n' && c != EOF); // Vacía el buffer de entrada

	return opcion;
}

void buscarLibro(SOCKET client_socket) {

    char opcion[10];

    std::cout << "Selecciona el tipo de busqueda:\n";
    std::cout << "1. Por titulo\n";
    std::cout << "2. Por autor\n";
    std::cout << "Opcion: ";
    std::cin.getline(opcion, sizeof(opcion));

    send(client_socket, opcion, strlen(opcion) + 1, 0);
    if (strcmp(opcion, "1") == 0) {
        char titulo[100];
        std::cout << "Ingresa el titulo del libro a buscar: ";
        std::cin.getline(titulo, sizeof(titulo));
        send(client_socket, titulo, strlen(titulo) + 1, 0);
    } else if (strcmp(opcion, "2") == 0) {
        char autor[100];
        std::cout << "Ingresa el nombre del autor a buscar: ";
        std::cin.getline(autor, sizeof(autor));
        send(client_socket, autor, strlen(autor) + 1, 0);
    } else {
        std::cerr << "Opcion no valida." << std::endl;
        return;
    }

    memset(recvBuff, 0, sizeof(recvBuff));
    int recv_size = recv(client_socket, recvBuff, sizeof(recvBuff) - 1, 0);
    if (recv_size <= 0) {
        perror("Error al recibir resultado de busqueda");
    }

    std::cout << "Resultados de la busqueda:\n" << recvBuff << std::endl;
}

void leerLibro(SOCKET client_socket, const char* titulo) {
    char sendBuff[2048], recvBuff[2048];
    int opcion;

    do {
        memset(recvBuff, 0, sizeof(recvBuff));
        int recv_size = recv(client_socket, recvBuff, sizeof(recvBuff) - 1, 0);
        if (recv_size <= 0) {
            perror("Error al recibir contenido del libro");
            break;
        }

        std::cout << recvBuff << std::endl;

        std::cout << "\nOpciones: \n";
        std::cout << "2. Siguiente pagina\n";
        std::cout << "3. Pagina anterior\n";
        std::cout << "4. Salir de la lectura\n";
        std::cout << "Selecciona una opcion: ";
        std::cin >> opcion;

        sprintf(sendBuff, "%d", opcion);
        send(client_socket, sendBuff, strlen(sendBuff) + 1, 0);
    } while (opcion != 4);
}

void menuMiLista() {
    char input[10];
    char sel;
    char titulo[30];
    char nom_autor[30];
    char idioma[10];
    char fecha_publicacion[11];
    char fecha_actual[10];
    Libro libro;

    std::string lista_completa;
    int recv_size = 0;


    do {


        printf(NEGRITA "Menu Mi Lista\n\n" QUITAR_NEGRITA);

        printf("\nSelecciona una opcion: \n");
        printf("1. Agregar Libro a Mi Lista\n2. Eliminar Libro de Mi Lista\n3. Descargar Libro\n4. Leer Libro\n5. Volver\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel);
        std::string lista_recomendacioens;

        switch (sel) {
            case '1':
                printf("\ncorrecto 1\n\n");


                strcpy(sendBuff, "AGREGAR-LIBRO-LISTA");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);


                // Pedir al usuario el título del libro
                printf("\nIntroduce el titulo del libro que desea guardar en tu lista:\n");
                fgets(libro.titulo, sizeof(libro.titulo), stdin);
                strtok(libro.titulo, "\n");

                // Enviar el título del libro al servidor
                send(s, libro.titulo, strlen(libro.titulo) + 1, 0);

                // Recibir confirmación del servidor
                memset(recvBuff, 0, sizeof(recvBuff));
                recv(s, recvBuff, sizeof(recvBuff) - 1, 0);
                printf("%s", recvBuff);

                // Enviar señal de fin al servidor
                strcpy(sendBuff, "AGREGAR-LIBRO-LISTA-END");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);

                // Mostrar lista actualizada
                strcpy(sendBuff, "MOSTRAR-LISTA");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);

                memset(recvBuff, 0, sizeof(recvBuff));
                recv(s, recvBuff, sizeof(recvBuff) - 1, 0);
                std::cout << "Mi Lista:\n" << recvBuff << std::endl;

                strcpy(sendBuff, "MOSTRAR-LISTA-END");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);
                break;

            case '2':
                printf("\ncorrecto 2\n\n");
                strcpy(sendBuff, "ELIMINAR-LIBRO-LISTA");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);

                printf("\nIntroduce el titulo del libro que desea eliminar de tu lista:\n");
                fgets(libro.titulo, sizeof(libro.titulo), stdin);
                strtok(libro.titulo, "\n");
                send(s, libro.titulo, strlen(libro.titulo) + 1, 0);

                recv(s, recvBuff, sizeof(recvBuff), 0);
                printf("%s", recvBuff);
                Sleep(1000);
                strcpy(sendBuff, "ELIMINAR-LIBRO-LISTA-END");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);



                strcpy(sendBuff, "MOSTRAR-LISTA");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);
                memset(recvBuff, 0, sizeof(recvBuff));

                recv(s, recvBuff, sizeof(recvBuff) - 1, 0);
                std::cout << "Mi Lista:\n" << recvBuff << std::endl;

                strcpy(sendBuff, "MOSTRAR-LISTA-END");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);
                break;

            case '3':
                printf("\ncorrecto 3\n\n");
                strcpy(sendBuff, "DESCARGAR-LIBRO");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);

                printf("\nIntroduce nombre del libro que quieras descargar en su lista: \n");
                fgets(libro.titulo, sizeof(libro.titulo), stdin);
                strtok(libro.titulo, "\n");
                send(s, libro.titulo, strlen(libro.titulo) + 1, 0);

                strcpy(sendBuff, "DESCARGAR-LIBRO-END");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);

                recv(s, recvBuff, sizeof(recvBuff), 0);
                printf("%s", recvBuff);

                Sleep(1000);

                strcpy(sendBuff, "MOSTRAR-LISTA");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);
                memset(recvBuff, 0, sizeof(recvBuff));

                recv(s, recvBuff, sizeof(recvBuff) - 1, 0);
                std::cout << "Mi Lista:\n" << recvBuff << std::endl;

                strcpy(sendBuff, "MOSTRAR-LISTA-END");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);
                break;

            case '4':
                printf("\ncorrecto 4\n\n");
                strcpy(sendBuff, "LEER-LIBRO");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);

                printf("\nIntroduce nombre del libro que quieras leer: \n");
                fgets(libro.titulo, sizeof(libro.titulo), stdin);
                strtok(libro.titulo, "\n");
                send(s, libro.titulo, strlen(libro.titulo) + 1, 0);

                leerLibro(s, libro.titulo);

                strcpy(sendBuff, "LEER-LIBRO-END");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);
                sel = -1;

                break;

            case '5':
                printf("\nvolviendo...\n\n");
                break;

            default:
                printf("\nIntroduce un valor valido\n\n");
                break;
        }
    } while (sel != '5');
}

void imprimirMenu(){
    char input[10];
    char sel;
    char titulo[30];
    char nom_autor[30];
    char idioma[10];
    char fecha_publicacion[11];
    char fecha_actual[10];
    bool salir = false; // Bandera para salir del bucle
    Libro libro;

    int recv_size = 0;

    printf(NEGRITA"Menu Principal\n\n" QUITAR_NEGRITA);

    if (cliente.es_admin == 1) {
        do{printf("\nSelecciona una opcion: \n");
            printf("1. Mi Lista\n2. Buscar Libro\n3. Agregar Libro BD (admin only)\n4. Eliminar Libro BD (admin only)\n5. Volver\n");

            fgets(input, sizeof(input), stdin);
            sscanf(input, " %c", &sel);
            std::string lista_completa;
            switch(sel) {
                case '1':

                    printf("\ncorrecto 1\n\n");
                    strcpy(sendBuff, "MOSTRAR-LISTA");
                    send(s, sendBuff, strlen(sendBuff) + 1, 0);
                    memset(recvBuff, 0, sizeof(recvBuff));

                    recv(s, recvBuff, sizeof(recvBuff) - 1, 0);
                    std::cout << "Mi Lista:\n" << recvBuff << std::endl;

                    strcpy(sendBuff, "MOSTRAR-LISTA-END");
                    send(s, sendBuff, strlen(sendBuff) + 1, 0);
                    menuMiLista();
                    break;

                case '2':
                    printf("\ncorrecto 2\n\n");
                    strcpy(sendBuff, "BUSCAR-LIBRO");
                    send(s, sendBuff, strlen(sendBuff) + 1, 0);

                    buscarLibro(s);

                    strcpy(sendBuff, "BUSCAR-LIBRO-END");
                    send(s, sendBuff, strlen(sendBuff) + 1, 0);
                    //imprimirMenu();
                    break;

                case '3':
                    printf("\ncorrecto 3\n\n");
                    strcpy(sendBuff, "AGREGAR-LIBRO-BD");
                    send(s, sendBuff, strlen(sendBuff) + 1, 0);
                    printf("\nIntroduce los datos del libro:\nIntroduce el nombre del libro:\n(30 caracteres como maximo)\n");

                    fgets(libro.titulo, sizeof(libro.titulo), stdin);
                    strtok(libro.titulo, "\n");
                    send(s, libro.titulo, strlen(libro.titulo) + 1, 0);

                    printf("\nIntroduce el nombre del autor:\n");
                    fgets(libro.autor_Libro, sizeof(libro.autor_Libro), stdin);
                    strtok(libro.autor_Libro, "\n");
                    send(s, libro.autor_Libro, strlen(libro.autor_Libro) + 1, 0);

                    printf("\nIntroduce el idioma de una manera reducida:\n(ejemplo: es, en...)\n");
                    fgets(libro.idioma, sizeof(idioma), stdin);
                    strtok(libro.idioma, "\n");
                    send(s, libro.idioma, strlen(libro.idioma) + 1, 0);

                    printf("\nIntroduce la fecha de publicacion del libro: \n(formato:aaaa-mm-dd)\n");
                    fgets(libro.fecha_Publicacion, sizeof(fecha_publicacion), stdin);
                    strtok(libro.fecha_Publicacion, "\n");
                    send(s, libro.fecha_Publicacion, strlen(libro.fecha_Publicacion) + 1, 0);

                    recv(s, recvBuff, sizeof(recvBuff), 0);
                    printf("%s \n\n", recvBuff);


                    memset(sendBuff, 0, sizeof(sendBuff));
                    strcpy(sendBuff, "AGREGAR-LIBRO-BD-END");
                    send(s, sendBuff, strlen(sendBuff) + 1, 0);
                    //imprimirMenu();

                    break;

                case '4':
                    printf("\ncorrecto 4\n\n");
                    strcpy(sendBuff, "ELIMINAR-LIBRO-BD");
                    send(s, sendBuff, strlen(sendBuff) + 1, 0);

                    printf("\nIntroduce nombre del libro que quiera eliminar de la BD: \n");
                    fgets(libro.titulo, sizeof(libro.titulo), stdin);
                    strtok(libro.titulo, "\n"); // Elimina el carácter
                    send(s, libro.titulo, strlen(libro.titulo) + 1, 0);

                    recv(s, recvBuff, sizeof(recvBuff), 0);
                    printf("%s \n\n", recvBuff);


                    //eliminarLibroBD(titulo);
                    break;

                case '5':
                    printf("\ncerrando sesion...\n\n");
                    salir = true;
                    break;

                default:
                    printf("\nIntroduce un valor valido\n\n");
                    break;
            }
            limpiarBuffer();
        } while(!salir);
    } else {
        do {
            printf("\nSelecciona una opcion: \n");
            printf("1. Mi Lista\n2. Buscar Libro\n3. Volver\n");

            fgets(input, sizeof(input), stdin);
            sscanf(input, " %c", &sel);
            std::string lista_completa;
            switch (sel) {
                case '1':
                    printf("\ncorrecto 1\n\n");
                    strcpy(sendBuff, "MOSTRAR-LISTA");
                    send(s, sendBuff, strlen(sendBuff) + 1, 0);
                    memset(recvBuff, 0, sizeof(recvBuff));

                    recv(s, recvBuff, sizeof(recvBuff) - 1, 0);
                    std::cout << "Mi Lista:\n" << recvBuff << std::endl;

                    strcpy(sendBuff, "MOSTRAR-LISTA-END");
                    send(s, sendBuff, strlen(sendBuff) + 1, 0);
                    menuMiLista();
                    break;
                case '2':
                    printf("\ncorrecto 2\n\n");

                    strcpy(sendBuff, "BUSCAR-LIBRO");
                    send(s, sendBuff, strlen(sendBuff) + 1, 0);

                    buscarLibro(s);


                    strcpy(sendBuff, "BUSCAR-LIBRO-END");
                    send(s, sendBuff, strlen(sendBuff) + 1, 0);

                    //buscarLibro();

                    break;
                case '3':
                    printf("\ncerrando sesion...\n\n");
                    salir = true;
                    break;
                default:
                    printf("\nIntroduce un valor valido\n\n");
                    break;
            }

        } while (!salir);
    }
}

void imprimirMenuInvitado() {
    char input[10];
    char sel;

    printf(NEGRITA "Menu Principal\nSesion de invitado\n\n" QUITAR_NEGRITA);

    do {
        printf("\nSelecciona una opcion: \n");
        printf("1. Gestionar Contenido\n2. Cerrar Sesion\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel);

        if (sel == '1') {
            printf("\ncorrecto 2\n\n");
            imprimirGestionInvitado();
        } else if (sel == '2') {
            printf("\ncerrando sesion...\n\n");
            return; // Salir del menú de invitado
        } else {
            printf("\nIntroduce un valor valido\n\n");
        }

    } while (sel != '2' && sel != '1');
}

void imprimirGestionInvitado() {
    char input[10];
    char sel;
    char titulo[30];
    char nom_autor[30];
    char idioma[10];
    char fecha_publicacion[11];
    Libro libro;

    printf(NEGRITA "Menu de Gestion de Contenido\nSesion de invitado\n\n" QUITAR_NEGRITA);

    do {
        printf("\nSelecciona una opcion: \n");
        printf("1. Buscar Libro\n2. Aportar Libro a la BD\n3. Volver\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel);

        switch (sel) {
            case '1':
                printf("\ncorrecto 1\n\n");

                strcpy(sendBuff, "BUSCAR-LIBRO");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);

                buscarLibro(s);


                strcpy(sendBuff, "BUSCAR-LIBRO-END");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);
                //buscarLibro();
                break;
            case '2':
                printf("\ncorrecto 2\n\n");

                strcpy(sendBuff, "AGREGAR-LIBRO-BD");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);
                printf("\nIntroduce los datos del libro:\nIntroduce el nombre del libro:\n(30 caracteres como maximo)\n");

                fgets(libro.titulo, sizeof(libro.titulo), stdin);
                strtok(libro.titulo, "\n");
                send(s, libro.titulo, strlen(libro.titulo) + 1, 0);

                printf("\nIntroduce el nombre del autor:\n");
                fgets(libro.autor_Libro, sizeof(libro.autor_Libro), stdin);
                strtok(libro.autor_Libro, "\n");
                send(s, libro.autor_Libro, strlen(libro.autor_Libro) + 1, 0);

                printf("\nIntroduce el idioma de una manera reducida:\n(ejemplo: es, en...)\n");
                fgets(libro.idioma, sizeof(idioma), stdin);
                strtok(libro.idioma, "\n");
                send(s, libro.idioma, strlen(libro.idioma) + 1, 0);

                printf("\nIntroduce la fecha de publicacion del libro: \n(formato:aaaa-mm-dd)\n");
                fgets(libro.fecha_Publicacion, sizeof(fecha_publicacion), stdin);
                strtok(libro.fecha_Publicacion, "\n");
                send(s, libro.fecha_Publicacion, strlen(libro.fecha_Publicacion) + 1, 0);

                recv(s, recvBuff, sizeof(recvBuff), 0);
                printf("%s \n\n", recvBuff);


                memset(sendBuff, 0, sizeof(sendBuff));
                strcpy(sendBuff, "AGREGAR-LIBRO-BD-END");
                send(s, sendBuff, strlen(sendBuff) + 1, 0);

                break;
            case '3':
                printf("\nvolviendo...\n\n");
                imprimirMenuInvitado();
                break;
            default:
                printf("\nIntroduce un valor valido\n\n");
                break;
        }
        limpiarBuffer();
    } while (sel != '3');
}

void imprimirInicial() {
    char input[10];
    char nombre[31];
    char email[101];
    char pass[31];
    char sel;
    bool salir = false;

	Cliente clienteRegistro;
    printf("\nProyecto Programacion IV | Grupo PVI-04\n\n");
    printf(NEGRITA"Bienvenido al Sistema de Libreria Virtual\nVerseVault\n\n" QUITAR_NEGRITA);

    do {
        printf("\nSelecciona una opcion: \n");
        printf("1. Iniciar Sesion \n2. Registrar cuenta \n3. Continuar como invitado \n4. Salir\n\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel);

        if (sel == '1') {
            strcpy(sendBuff, "INICIAR-SESION");
            send(s, sendBuff, strlen(sendBuff) + 1, 0);

            printf("\nIntroduce tus datos para iniciar sesion:\nIntroduce tu correo electronico:\n");
            fgets(cliente.email_cl, sizeof(cliente.email_cl), stdin);
            strtok(cliente.email_cl, "\n");
            send(s, cliente.email_cl, strlen(cliente.email_cl) + 1, 0);

            printf("\nIntroduce tu contrasenya:\n");
            fgets(cliente.passw, sizeof(cliente.passw), stdin);
            strtok(cliente.passw, "\n");
            send(s, cliente.passw, strlen(cliente.passw) + 1, 0);

            recv(s, recvBuff, sizeof(recvBuff), 0);
            printf("Usuario ID: %s \n\n", recvBuff);

            recv(s, recvBuff, sizeof(recvBuff), 0);
            //printf("Soy: %s \n\n", recvBuff);

            if (strcmp(recvBuff, "1") == 0) {
                cliente.es_admin = 1;
            } else {
                cliente.es_admin = 0;
            }
            strcpy(sendBuff, "INICIAR-SESION-END");
            send(s, sendBuff, strlen(sendBuff) + 1, 0);

            if (strcmp(recvBuff, "-1") != 0){ //ENTRA al siguiente menu, si el inicio de sesion ha sido corecto
                imprimirMenu();
            }


        }if(sel == '2'){
            strcpy(sendBuff, "REGISTRAR");
            send(s, sendBuff, strlen(sendBuff) + 1, 0);

            // Envío del nombre del cliente
            printf("\nIntroduce tus datos para registrarte:\nIntroduce tu nombre:\n(30 caracteres como maximo)\n");
            fgets(clienteRegistro.nom_Cliente, sizeof(clienteRegistro.nom_Cliente), stdin);
            strtok(clienteRegistro.nom_Cliente, "\n");
            send(s, clienteRegistro.nom_Cliente, strlen(clienteRegistro.nom_Cliente) + 1, 0);

            // Envío del correo electrónico
            printf("\nIntroduce tu correo electronico:\n");
            fgets(clienteRegistro.email_cl, sizeof(clienteRegistro.email_cl), stdin);
            strtok(clienteRegistro.email_cl, "\n");
            send(s, clienteRegistro.email_cl, strlen(clienteRegistro.email_cl) + 1, 0);

            // Envío de la contraseña
            printf("\nIntroduce tu contrasenya:\n(30 caracteres como maximo)\n");
            fgets(clienteRegistro.passw, sizeof(clienteRegistro.passw), stdin);
            strtok(clienteRegistro.passw, "\n");
            send(s, clienteRegistro.passw, strlen(clienteRegistro.passw) + 1, 0);

            // Recibir respuesta del servidor
            recv(s, recvBuff, sizeof(recvBuff), 0);

            if (strcmp(recvBuff, "-1") == 0) {
                printf("Error: Registro fallido.\n");
            } else {
                printf("REGISTRO correcto");
            }

            // Envío del comando "REGISTRAR-END"
            strcpy(sendBuff, "REGISTRAR-END");
            send(s, sendBuff, strlen(sendBuff) + 1, 0);


        }else if(sel == '3'){
            strcpy(sendBuff, "INVITADO");
            send(s, sendBuff, strlen(sendBuff) + 1, 0);

            imprimirMenuInvitado();

            strcpy(sendBuff, "INVITADO-END");
            send(s, sendBuff, strlen(sendBuff) + 1, 0);

        }else if(sel == '4'){
            printf("\ncerrando...\n");
            salir = true;
        }else{
            printf("\nIntroduce un valor valido\n\n");
    }

    }while(!salir);
}




char menu()
{
	printf("\n");
	printf("Elige la OPCION que desees: \n");
	printf("1. Sumar (2 + 3 + 5) \n");
	printf("2. Raiz cuadrada (9) \n");
	printf("3. Obtener IP del servidor \n");
	printf("4. Salir \n\n");
	printf("Opcion: ");
	char opcion = getchar();

	char c; while ((c = getchar()) != '\n' && c != EOF); // Vac�a el buffer de entrada

	return opcion;
}


int main(int argc, char *argv[])
{

	WSADATA wsaData;
	struct sockaddr_in server;

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return -1;
	}

	printf("Initialised.\n");

	//SOCKET creation
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//CONNECT to remote server
	if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Connection error: %d", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return -1;
	}

	printf("Connection stablished with: %s (%d)\n", inet_ntoa(server.sin_addr),
			ntohs(server.sin_port));

	//SEND and RECEIVE data (CLIENT/SERVER PROTOCOL)
    imprimirInicial();
	closesocket(s);
	WSACleanup();

	return 0;
}


