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

using namespace std;

#define NEGRITA "\e[1m" // Renombramos el código de los caracteres en negrita para que sea mas entendible
#define QUITAR_NEGRITA "\e[m" // Renombramos el codigo de quitar la negrita a los caracteres para que sea mas entendible
//"Sin dolor no hay gloria" - Proverbio Romano
SOCKET s;
char sendBuff[512], recvBuff[512];
int es_admin = 0;
Cliente cliente;

void leerLibro(SOCKET client_socket, const char* titulo);
void buscarLibro(SOCKET client_socket);
void imprimirInicial();


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
    char sendBuff[512], recvBuff[2048];
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

    printf(NEGRITA"Menu Mi Lista\n\n" QUITAR_NEGRITA);
    do{
        printf("\nSelecciona una opcion: \n");

        //Falta mostrar la lista actual del cliente

        printf("1. Agregar Libro a Mi Lista\n2. Eliminar Libro de Mi Lista\n3. Descargar Libro\n4. Leer Libro\n5. Volver\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel);


        if(sel == '1'){
            printf("\ncorrecto 1\n\n");

        	strcpy(sendBuff, "AGREGAR-LIBRO-LISTA");
        	send(s, sendBuff, strlen(sendBuff) + 1, 0);

        	printf("\nIntroduce el titulo del libro que desea guardar en tu lista:\n");
        	fgets(libro.titulo, sizeof(libro.titulo), stdin);
        	strtok(libro.titulo, "\n");
        	send(s, libro.titulo, strlen(libro.titulo) + 1, 0);


        	recv(s, recvBuff, sizeof(recvBuff), 0);
			printf("%s", recvBuff);

        	strcpy(sendBuff, "AGREGAR-LIBRO-LISTA-END");
        	send(s, sendBuff, strlen(sendBuff) + 1, 0);

        	menuMiLista();

            //aportarLibroMenu(titulo, actualTime);
        }else if(sel == '2'){
        	printf("\ncorrecto 2\n\n");
        	strcpy(sendBuff, "ELIMINAR-LIBRO-LISTA");
        	send(s, sendBuff, strlen(sendBuff) + 1, 0);

        	printf("\nIntroduce el titulo del libro que desea eliminar de tu lista:\n");
        	fgets(libro.titulo, sizeof(libro.titulo), stdin);
        	strtok(libro.titulo, "\n");
        	send(s, libro.titulo, strlen(libro.titulo) + 1, 0);


        	recv(s, recvBuff, sizeof(recvBuff), 0);
        	printf("%s", recvBuff);

        	strcpy(sendBuff, "ELIMINAR-LIBRO-LISTA-END");
        	send(s, sendBuff, strlen(sendBuff) + 1, 0);

        	menuMiLista();




        }else if(sel == '3'){
            printf("\ncorrecto 3\n\n");
            strcpy(sendBuff, "DESCARGAR-LIBRO");
            send(s, sendBuff, strlen(sendBuff) + 1, 0);

            printf("\nIntroduce nombre del libro que quieras descargar en su lista: \n");
            fgets(libro.titulo, sizeof(libro.titulo), stdin);
            strtok(libro.titulo, "\n"); // Elimina el carácter
            send(s, libro.titulo, strlen(libro.titulo) + 1, 0);

            strcpy(sendBuff, "DESCARGAR-LIBRO-END");
            send(s, sendBuff, strlen(sendBuff) + 1, 0);

            recv(s, recvBuff, sizeof(recvBuff), 0);
            printf("%s", recvBuff);

            //descargarLibro(titulo);

        }else if(sel == '4'){
            printf("\ncorrecto 4\n\n");
            strcpy(sendBuff, "LEER-LIBRO");
            send(s, sendBuff, strlen(sendBuff) + 1, 0);

            printf("\nIntroduce nombre del libro que quieras leer: \n");
            fgets(libro.titulo, sizeof(libro.titulo), stdin);
            strtok(libro.titulo, "\n"); // Elimina el carácter
            send(s, libro.titulo, strlen(libro.titulo) + 1, 0);

            leerLibro(s, libro.titulo);


            strcpy(sendBuff, "LEER-LIBRO-END");
            send(s, sendBuff, strlen(sendBuff) + 1, 0);



            //leer libro
        }else if(sel == '5'){
            printf("\nvolviendo...\n\n");
            //imprimirMenu();
        }else{
            printf("\nIntroduce un valor valido\n\n");
        }

    }while(sel != '5' && sel != '4' && sel != '3' && sel != '2' && sel != '1');
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

    printf(NEGRITA"Menu Principal\n\n" QUITAR_NEGRITA);

    if (cliente.es_admin == 1) {
        do{printf("\nSelecciona una opcion: \n");
            printf("1. Mi Lista\n2. Buscar Libro\n3. Agregar Libro BD (admin only)\n4. Eliminar Libro BD (admin only)\n5. Volver\n");

            fgets(input, sizeof(input), stdin);
            sscanf(input, " %c", &sel);

            switch(sel) {
                case '1':
                    printf("\ncorrecto 1\n\n");
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
                    printf("\ncorrecto 4\n\n");
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

                    strcpy(sendBuff, "AGREGAR-LIBRO-BD-END");
                    send(s, sendBuff, strlen(sendBuff) + 1, 0);
                    imprimirMenu();
                    break;

                case '4':
                    // Coloca aquí la lógica para la opción 4
                    break;

                case '5':
                    printf("\ncerrando sesion...\n\n");
                    salir = true;
                    break;

                default:
                    printf("\nIntroduce un valor valido\n\n");
                    break;
            }
        } while(!salir);
    } else {
        do {
            printf("\nSelecciona una opcion: \n");
            printf("1. Mi Lista\n2. Buscar Libro\n3. Volver\n");

            fgets(input, sizeof(input), stdin);
            sscanf(input, " %c", &sel);

            switch (sel) {
                case '1':
                    printf("\ncorrecto 1\n\n");
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
            // imprimirGestionInvitado();
        } else if (sel == '2') {
            printf("\ncerrando sesion...\n\n");
            return; // Salir del menú de invitado
        } else {
            printf("\nIntroduce un valor valido\n\n");
        }

    } while (sel != '2' && sel != '1');
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
            printf("Usuario: %s \n\n", recvBuff);

            recv(s, recvBuff, sizeof(recvBuff), 0);
            printf("Soy: %s \n\n", recvBuff);

            if (strcmp(recvBuff, "1") == 0) {
                cliente.es_admin = 1;
            } else {
                cliente.es_admin = 0;
            }

            if (strcmp(recvBuff, "-1") != 0){ //ENTRA al siguiente menu, si el inicio de sesion ha sido corecto
                imprimirMenu();
            }

            strcpy(sendBuff, "INICIAR-SESION-END");
            send(s, sendBuff, strlen(sendBuff) + 1, 0);


        }else if(sel == '2'){
            strcpy(sendBuff, "REGISTRAR");
            send(s, sendBuff, strlen(sendBuff) + 1, 0);


            printf("\nIntroduce tus datos para registrarte:\nIntroduce tu nombre:\n(30 caracteres como maximo)\n");
            fgets(clienteRegistro.nom_Cliente, sizeof(clienteRegistro.nom_Cliente), stdin);
            strtok(clienteRegistro.nom_Cliente, "\n");
            send(s, clienteRegistro.nom_Cliente, strlen(clienteRegistro.nom_Cliente) + 1, 0);


            printf("\nIntroduce tu correo electronico:\n");
            fgets(clienteRegistro.email_cl, sizeof(clienteRegistro.email_cl), stdin);
            strtok(clienteRegistro.email_cl, "\n");
            send(s, clienteRegistro.email_cl, strlen(clienteRegistro.email_cl) + 1, 0);



            printf("\nIntroduce tu contrasenya:\n(30 caracteres como maximo)\n");
            fgets(clienteRegistro.passw, sizeof(clienteRegistro.passw), stdin);
            strtok(clienteRegistro.passw, "\n");
            send(s, clienteRegistro.passw, strlen(clienteRegistro.passw) + 1, 0);

            recv(s, recvBuff, sizeof(recvBuff), 0);

            if (strcmp(recvBuff, "-1") == 0) {
                printf("Error: Registro fallido.\n");
            } else {
                printf("REGISTRO correcto");
            }
            //registrarClienteMenu(nombre,email,pass);

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


	/*char c;
	do
	{
		c = imprimirInicial1();
		if (c == '1')
		{
            // Enviar comando INICIAR-SESION
            strcpy(sendBuff, "INICIAR-SESION");
            send(s, sendBuff, strlen(sendBuff) + 1, 0); // +1 para incluir el terminador nulo

            printf("\nIntroduce tus datos para iniciar sesion:\nIntroduce tu correo electronico:\n");

            //iniciarSesionMenu(email,pass);
            char* correo;
            fgets(correo, sizeof(correo), stdin);
            strtok(correo, "\n"); // Elimina el carácter

            printf("\nIntroduce tu contrasenya:\n");

            //iniciarSesionMenu(email,pass);
            char* contra;
            fgets(contra, sizeof(contra), stdin);
            strtok(contra, "\n"); // Elimina el carácter


            // Enviar usuario como carácter
			send(s, correo, sizeof(correo), 0);

			// Enviar contraseña como carácter
			send(s, contra, sizeof(contra), 0);

			// Enviar comando INICIAR-SESION-END
			strcpy(sendBuff, "INICIAR-SESION-END");
			send(s, sendBuff, strlen(sendBuff) + 1, 0);

			// Recibir respuesta del servidor
			recv(s, recvBuff, sizeof(recvBuff), 0);
			printf("Usuario: %s \n", recvBuff);

			// Pasar al menú de iniciar sesión
		}

		if (c == '2')
		{
			// SENDING command RAIZ and parameter to the server
			strcpy(sendBuff, "RAIZ");
			send(s, sendBuff, sizeof(sendBuff), 0);
			strcpy(sendBuff, "9");
			send(s, sendBuff, sizeof(sendBuff), 0);
			strcpy(sendBuff, "RAIZ-END");
			send(s, sendBuff, sizeof(sendBuff), 0);

			// RECEIVING response to command RAIZ from the server
			recv(s, recvBuff, sizeof(recvBuff), 0);
			printf("Raiz cuadrada = %s \n", recvBuff);
		}

		if (c == '3')
		{
			// SENDING command IP
			strcpy(sendBuff, "IP");
			send(s, sendBuff, sizeof(sendBuff), 0);
			strcpy(sendBuff, "IP-END");
			send(s, sendBuff, sizeof(sendBuff), 0);

			// RECEIVING response to command IP from the server
			recv(s, recvBuff, sizeof(recvBuff), 0);
			printf("IP del servidor = %s \n", recvBuff);
		}

		if (c == '4')
		{
			// SENDING command EXIT
			strcpy(sendBuff, "EXIT");
			send(s, sendBuff, sizeof(sendBuff), 0);
		}
	}while(c != '4');
*/
	// CLOSING the socket and cleaning Winsock...
	closesocket(s);
	WSACleanup();

	return 0;
}


