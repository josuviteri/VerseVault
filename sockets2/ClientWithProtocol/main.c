// IMPORTANT: Winsock Library ("ws2_32") should be linked

#include <stdio.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000


#define NEGRITA "\e[1m" // Renombramos el código de los caracteres en negrita para que sea mas entendible
#define QUITAR_NEGRITA "\e[m" // Renombramos el codigo de quitar la negrita a los caracteres para que sea mas entendible
//"Sin dolor no hay gloria" - Proverbio Romano

char imprimirInicial1(){
	printf("\nProyecto Programacion IV | Grupo PVI-04\n\n");
    printf(NEGRITA"Bienvenido al Sistema de Libreria Virtual\nVerseVault\n\n" QUITAR_NEGRITA);

 	printf("\nSelecciona una opcion: \n");
    printf("1. Iniciar Sesion \n2. Registrar cuenta \n3. Continuar como invitado \n4. Salir\n\n");

	char opcion = getchar();

	char c; while ((c = getchar()) != '\n' && c != EOF); // Vacía el buffer de entrada

	return opcion;
}

char imprimirInicial2_1(){
	printf("\nIntroduce tus datos para iniciar sesion:\nIntroduce tu correo electronico:\n");

    //iniciarSesionMenu(email,pass);
	char opcion = getchar();

	char c; while ((c = getchar()) != '\n' && c != EOF); // Vacía el buffer de entrada

	return opcion;
}

char imprimirInicial2_2(){
    printf("\nIntroduce tu contrasenya:\n");
    
	//iniciarSesionMenu(email,pass);
	char opcion = getchar();

	char c; while ((c = getchar()) != '\n' && c != EOF); // Vacía el buffer de entrada

	return opcion;
}

void imprimirInicial(){
    char input[10];
    char nombre[31];
    char email[101];
    char pass[31];
    char sel;


    printf("\nProyecto Programacion IV | Grupo PVI-04\n\n");
    printf(NEGRITA"Bienvenido al Sistema de Libreria Virtual\nVerseVault\n\n" QUITAR_NEGRITA);

    do{
        printf("\nSelecciona una opcion: \n");
        printf("1. Iniciar Sesion \n2. Registrar cuenta \n3. Continuar como invitado \n4. Salir\n\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel);

        if(sel == '1'){
            printf("\nIntroduce tus datos para iniciar sesion:\nIntroduce tu correo electronico:\n");
            fgets(email, sizeof(email), stdin);

            printf("\nIntroduce tu contrasenya:\n");
            fgets(pass, sizeof(pass), stdin);

            //iniciarSesionMenu(email,pass);

        }else if(sel == '2'){
            printf("\nIntroduce tus datos para registrarte:\nIntroduce tu nombre:\n(30 caracteres como maximo)\n");
            fgets(nombre, sizeof(nombre), stdin);

            printf("\nIntroduce tu correo electronico:\n");
            fgets(email, sizeof(email), stdin);

            printf("\nIntroduce tu contrasenya:\n(30 caracteres como maximo)\n");
            fgets(pass, sizeof(pass), stdin);


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
	SOCKET s;
	struct sockaddr_in server;
	char sendBuff[512], recvBuff[512];

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
	char c;
	do
	{
		c = imprimirInicial1();
		if (c == '1')
		{
			char c2_1 = imprimirInicial2_1();
			char c2_2 = imprimirInicial2_2();

			// SENDING command SUMAR and parameters to the server
			strcpy(sendBuff, "INICIAR-SESION");
			send(s, sendBuff, sizeof(sendBuff), 0);
			//strcpy(sendBuff, c2_1);
			//send(s, sendBuff, sizeof(sendBuff), 0);
			//strcpy(sendBuff, c2_2);
			//send(s, sendBuff, sizeof(sendBuff), 0);
			strcpy(sendBuff, "INICIAR-SESION-END");
			send(s, sendBuff, sizeof(sendBuff), 0);

			// RECEIVING response to command INICIAR SESION from the server
			recv(s, recvBuff, sizeof(recvBuff), 0);
			printf("Suma = %s \n", recvBuff);
			//pasar al menu iniciar sesion menu
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

	// CLOSING the socket and cleaning Winsock...
	closesocket(s);
	WSACleanup();

	return 0;
}
