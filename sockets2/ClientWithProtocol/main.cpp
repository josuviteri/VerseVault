// IMPORTANT: Winsock Library ("ws2_32") should be linked

#include <stdio.h>
#include <winsock2.h>
#include <string.h>


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000


#define NEGRITA "\e[1m" // Renombramos el código de los caracteres en negrita para que sea mas entendible
#define QUITAR_NEGRITA "\e[m" // Renombramos el codigo de quitar la negrita a los caracteres para que sea mas entendible
//"Sin dolor no hay gloria" - Proverbio Romano
SOCKET s;
char sendBuff[512], recvBuff[512];

char imprimirInicial1(){
	printf("\nProyecto Programacion IV | Grupo PVI-04\n\n");
    printf(NEGRITA"Bienvenido al Sistema de Libreria Virtual\nVerseVault\n\n" QUITAR_NEGRITA);

 	printf("\nSelecciona una opcion: \n");
    printf("1. Iniciar Sesion \n2. Registrar cuenta \n3. Continuar como invitado \n4. Salir\n\n");

	char opcion = getchar();

	char c; while ((c = getchar()) != '\n' && c != EOF); // Vacía el buffer de entrada

	return opcion;
}


void menuMiLista() {
    char input[10];
    char sel;
    char titulo[30];
    char nom_autor[30];
    char idioma[10];
    char fecha_publicacion[11];
    char fecha_actual[10];
    printf(NEGRITA"Menu Mi Lista\n\n" QUITAR_NEGRITA);
    do{
        printf("\nSelecciona una opcion: \n");

        //Falta mostrar la lista actual del cliente

        printf("1.Agregar Libro a Mi Lista\n2.Eliminar Libro de Mi Lista\n3.Descargar Libro\n4.Leer Libro\n5.Volver\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel);


        if(sel == '1'){
            printf("\ncorrecto 1\n\n");
            printf("\nIntroduce el titulo del libro que desea guardar en tu lista: \n");
            fgets(titulo, sizeof(titulo), stdin);
            strtok(titulo, "\n"); // Elimina el carácter
            //aportarLibroMenu(titulo, actualTime);
        }else if(sel == '2'){
            printf("\ncorrecto 2\n\n");
            printf("\nIntroduce nombre del libro que quiera eliminar de su lista: \n");
            fgets(titulo, sizeof(titulo), stdin);
            strtok(titulo, "\n"); // Elimina el carácter
            //eliminarLibroMenu(titulo);
        }else if(sel == '3'){
            printf("\ncorrecto 3\n\n");
            printf("\nIntroduce nombre del libro que quieras descargar en su lista: \n");
            fgets(titulo, sizeof(titulo), stdin);
            strtok(titulo, "\n"); // Elimina el carácter
            //descargarLibro(titulo);

        }else if(sel == '4'){
            printf("\ncorrecto 4\n\n");
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

    printf(NEGRITA"Menu Principal\n\n" QUITAR_NEGRITA);


    do{
        printf("\nSelecciona una opcion: \n");
        printf("1. Mi Lista\n2.Buscar Libro\n3.Agregar Libro BD (admin only)\n4.Eliminar Libro BD (admin only)\n5.Volver\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel);

        if(sel == '1'){
            printf("\ncorrecto 1\n\n");
            menuMiLista();

        }else if(sel == '2'){
            printf("\ncorrecto 2\n\n");
            //buscarLibro();

        }else if(sel == '3'){
            printf("\ncorrecto 4\n\n");
            printf("\nIntroduce los datos del libro:\nIntroduce el nombre del libro:\n(30 caracteres como maximo)\n");
            fgets(titulo, sizeof(titulo), stdin);
            strtok(titulo, "\n"); // Elimina el carácter

            printf("\nIntroduce el nombre del autor:\n");
            fgets(nom_autor, sizeof(nom_autor), stdin);
            strtok(nom_autor, "\n"); // Elimina el carácter


            printf("\nIntroduce el idioma de una manera reducida:\n(ejemplo: es, en...)\n");
            fgets(idioma, sizeof(idioma), stdin);
            strtok(idioma, "\n"); // Elimina el carácter

            printf("\nIntroduce la fecha de publicacion del libro: \n(formato:aaaa-mm-dd)\n");
            fgets(fecha_publicacion, sizeof(fecha_publicacion), stdin);
            strtok(fecha_publicacion, "\n"); // Elimina el carácter

            //agregarLibroMenu(titulo, nom_autor, idioma, fecha_publicacion);
        }else if(sel == '4'){

        }else if(sel == '5'){
            printf("\ncerrando sesion...\n\n");
            //imprimirInicial();
        }else{
            printf("\nIntroduce un valor valido\n\n");
        }

    }while(sel != '5' && sel != '4' && sel != '3' && sel != '2' && sel != '1' && sel != '1');
}

void imprimirInicial() {
    char input[10];
    char nombre[31];
    char email[101];
    char pass[31];
    char sel;

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
            fgets(email, sizeof(email), stdin);
            strtok(email, "\n");
            send(s, email, strlen(email) + 1, 0);

            printf("\nIntroduce tu contrasenya:\n");
            fgets(pass, sizeof(pass), stdin);
            strtok(pass, "\n");
            send(s, pass, strlen(pass) + 1, 0);

            recv(s, recvBuff, sizeof(recvBuff), 0);
            printf("Usuario: %s \n\n", recvBuff);

            if (strcmp(recvBuff, "-1") != 0){ //ENTRA al siguiente menu, si el inicio de sesion ha sido corecto
                imprimirMenu();
            }

            strcpy(sendBuff, "INICIAR-SESION-END");
            send(s, sendBuff, strlen(sendBuff) + 1, 0);


        }else if(sel == '2'){
            strcpy(sendBuff, "REGISTRAR");
            send(s, sendBuff, strlen(sendBuff) + 1, 0);


            printf("\nIntroduce tus datos para registrarte:\nIntroduce tu nombre:\n(30 caracteres como maximo)\n");
            fgets(nombre, sizeof(nombre), stdin);
            strtok(nombre, "\n");
            send(s, nombre, strlen(nombre) + 1, 0);


            printf("\nIntroduce tu correo electronico:\n");
            fgets(email, sizeof(email), stdin);
            strtok(email, "\n");
            send(s, email, strlen(email) + 1, 0);



            printf("\nIntroduce tu contrasenya:\n(30 caracteres como maximo)\n");
            fgets(pass, sizeof(pass), stdin);
            strtok(pass, "\n");
            send(s, pass, strlen(pass) + 1, 0);

            recv(s, recvBuff, sizeof(recvBuff), 0);

            if (strcmp(recvBuff, "-1") == 0) {
                printf("Error: Registro fallido.\n");
            } else {
                printf("REGISTRO correcto");
            }
            //registrarClienteMenu(nombre,email,pass);

        }else if(sel == '3'){
           // imprimirMenuInvitado();
        }else if(sel == '4'){
            printf("\ncerrando...\n");
        }else{
            printf("\nIntroduce un valor valido\n\n");
    }

    }while(sel != '4' && sel != '3' && sel != '2' && sel != '1');
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


