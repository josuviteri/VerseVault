#include <stdio.h>
#include <winsock2.h>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "../../structs/cliente.h"
#include "../../structs/libro.h"
#include "../sqlite/sqlite3.h"
#include "../database/db2.h"
#include "../menu/menu.h"
#include "../database/dbBusqueda.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000
#define convPag 20 // Número de líneas por página, ajusta según sea necesario

using namespace std;

void leerLibro(SOCKET client_socket, const string& titulo);
void buscarLibro(SOCKET client_socket);



time_t rawtime;
struct tm* timeinfo;
char actualTime[80];

int main(int argc, char *argv[]) {
    WSADATA wsaData;
    SOCKET conn_socket;
    SOCKET comm_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char sendBuff[2048], recvBuff[2048];
    Cliente cl;
    Cliente clRegistro;


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
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(actualTime, sizeof(actualTime), "%Y-%m-%d", timeinfo);

        memset(recvBuff, 0, sizeof(recvBuff));
        int recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        if (recv_size <= 0) {
            perror("Error al recibir comando");
            break;
        }

        printf("Command received: %s \n", recvBuff);

        if (strcmp(recvBuff, "INICIAR-SESION") == 0) {
            printf("Received INICIAR-SESION command\n");
            commandLog("Received INICIAR-SESION command");

            memset(&cl, 0, sizeof(Cliente)); // Inicializa todo a cero

            recv_size = recv(comm_socket, cl.email_cl, sizeof(cl.email_cl) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir email");
                commandLog("Error al recibir email");
                continue;
            }
            cl.email_cl[recv_size] = '\0'; // Asegura terminador nulo
            printf("Email received: %s\n", cl.email_cl);
            commandLog("Email received");

            recv_size = recv(comm_socket, cl.passw, sizeof(cl.passw) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir contraseña");
                commandLog("Error al recibir contraseña");
                continue;
            }
            cl.passw[recv_size] = '\0'; // Asegura terminador nulo
            printf("Password received: %s\n", cl.passw);
            commandLog("Password received");

            printf("Calling iniciarSesionMenu\n");
            commandLog("Calling iniciarSesionMenu");
            cl.id_Cliente = iniciarSesionMenu(cl.email_cl, cl.passw);
            printf("ID Cliente: %d\n", cl.id_Cliente);

            memset(sendBuff, 0, sizeof(sendBuff));
            sprintf(sendBuff, "%d", cl.id_Cliente);
            if (send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0) == -1) {
                perror("Error al enviar ID de cliente");
                commandLog("Error al enviar ID de cliente");
            } else {
                printf("ID de cliente enviado correctamente\n");
                commandLog("ID de cliente enviado correctamente");
            }

            // Verificar si el correo electrónico termina con "@opendeusto.es"
            if (endsWith(cl.email_cl, "@opendeusto.es") || endsWith(cl.email_cl, "@deusto.es")) {
                cl.es_admin = 1;
            } else {
                cl.es_admin = 0;
            }
            if (esAdmin(cl.email_cl) == 1 ) {
                cl.es_admin = 1;
            } else {
                cl.es_admin = 0;
            }
            printf("Admin status: %d\n", cl.es_admin);

            memset(sendBuff, 0, sizeof(sendBuff));
            sprintf(sendBuff, "%d", cl.es_admin);
            if (send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0) == -1) {
                perror("Error al enviar si es admin el cliente");
                commandLog("Error al enviar si es admin el cliente");
            } else {
                printf("Admin status enviado correctamente\n");
                commandLog("Admin status enviado correctamente");
            }

            memset(recvBuff, 0, sizeof(recvBuff));
            recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            if (recv_size <= 0) {
                perror("Error al recibir INICIAR-SESION-END");
                commandLog("Error al recibir INICIAR-SESION-END");
                continue;
            }

            if (strcmp(recvBuff, "INICIAR-SESION-END") != 0) {
                printf("Error: Comando INICIAR-SESION-END no recibido correctamente\n");
                commandLog("Error: Comando INICIAR-SESION-END no recibido correctamente");
            } else {
                printf("INICIAR-SESION-END recibido correctamente\n");
                commandLog("INICIAR-SESION-END recibido correctamente");
            }
        }

        if (strcmp(recvBuff, "REGISTRAR") == 0) {

            memset(&clRegistro, 0, sizeof(Cliente)); // Inicializa todo a cero


            recv_size = recv(comm_socket, clRegistro.nom_Cliente, sizeof(clRegistro.nom_Cliente) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir nom_Cliente");
                commandLog("Error al recibir nom_Cliente");
                continue;
            }
            clRegistro.nom_Cliente[recv_size] = '\0'; // Asegura terminador nulo


            recv_size = recv(comm_socket, clRegistro.email_cl, sizeof(clRegistro.email_cl) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir email");
                commandLog("Error al recibir email");
                continue;
            }
            clRegistro.email_cl[recv_size] = '\0'; // Asegura terminador nulo

            recv_size = recv(comm_socket, clRegistro.passw, sizeof(clRegistro.passw) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir contraseña");
                commandLog("Error al recibir contraseña");
                continue;
            }
            clRegistro.passw[recv_size] = '\0'; // Asegura terminador nulo

            int registradoBien = 0;
            registradoBien = registrarClienteMenu(clRegistro.nom_Cliente,clRegistro.email_cl,clRegistro.passw);


            memset(sendBuff, 0, sizeof(sendBuff));
            sprintf(sendBuff, "%d", registradoBien);
            if (send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0) == -1) {
                perror("Error al enviar confiramcion Registro correcto");
                commandLog("Error al enviar confiramcion Registro correcto");
            }

            memset(recvBuff, 0, sizeof(recvBuff));
            recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            if (recv_size <= 0) {
                perror("Error al recibir REGISTRAR-END");
                commandLog("Error al recibir REGISTRAR-END");
                continue;
            }

            if (strcmp(recvBuff, "REGISTRAR-END") != 0) {
                printf("Error: Comando REGISTRAR-END no recibido correctamente\n");
                commandLog("Error: Comando REGISTRAR-END no recibido correctamente");
            }
        }

        if (strcmp(recvBuff, "INVITADO") == 0) {
            printf("Mensaje INVITADO recibido.\n");
            commandLog("Mensaje INVITADO recibido.");
            memset(recvBuff, 0, sizeof(recvBuff));
            recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir INVITADO-END");
                commandLog("Error al recibir INVITADO-END");
            } else {
                recvBuff[recv_size] = '\0';
                if (strcmp(recvBuff, "INVITADO-END") == 0) {
                    printf("Sesión de invitado manejada correctamente\n");
                    commandLog("Sesión de invitado manejada correctamente");
                } else {
                    printf("Error: Comando INVITADO-END no recibido correctamente\n");
                    commandLog("Error: Comando INVITADO-END no recibido correctamente");
                }
            }

        }

        if (strcmp(recvBuff, "AGREGAR-LIBRO-LISTA") == 0) {
            Libro libro;
            memset(&libro, 0, sizeof(Libro)); // Inicializa todo a cero

            // Recibir título del libro del cliente
            memset(recvBuff, 0, sizeof(recvBuff));
            recv_size = recv(comm_socket, libro.titulo, sizeof(libro.titulo) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir título");
                commandLog("Error al recibir título");
                continue;
            }
            libro.titulo[recv_size] = '\0'; // Asegura terminador nulo

            // Agregar libro a la lista del cliente
            int status;
            status = aportarLibroMenu(cl.id_Cliente, libro.titulo, actualTime);

            // Enviar confirmación al cliente
            memset(sendBuff, 0, sizeof(sendBuff));
            if (status == SQLITE_OK) {
                strcpy(sendBuff, "El libro se ha agregado correctamente\n");
                commandLog("El libro se ha agregado correctamente");
            } else {
                strcpy(sendBuff, "El libro no se ha agregado correctamente\n");
                commandLog("El libro no se ha agregado correctamente");
            }
            send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);

            // Esperar confirmación de fin del cliente
            memset(recvBuff, 0, sizeof(recvBuff));
            recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir AGREGAR-LIBRO-LISTA-END");
                commandLog("Error al recibir AGREGAR-LIBRO-LISTA-END");
                continue;
            }

            if (strcmp(recvBuff, "AGREGAR-LIBRO-LISTA-END") != 0) {
                printf("Error: Comando AGREGAR-LIBRO-LISTA-END no recibido correctamente\n");
                commandLog("Error: Comando AGREGAR-LIBRO-LISTA-END no recibido correctamente");
            }
        }
        if (strcmp(recvBuff, "MOSTRAR-RECOMENDACIONES") == 0) {

            char* recomendaciones = mostrarRecomendaciones();
            if (recomendaciones == NULL) {
                perror("Error al obtener la lista");
                commandLog("Error al obtener la lista");
                continue;
            }

            memset(sendBuff, 0, sizeof(sendBuff));
            strncpy(sendBuff, recomendaciones, sizeof(sendBuff) - 1);
            send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);

            free(recomendaciones);

            memset(recvBuff, 0, sizeof(recvBuff));
            recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir MOSTRAR-RECOMENDACIONES-END");
                commandLog("Error al recibir MOSTRAR-RECOMENDACIONES-END");
                continue;
            }

            if (strcmp(recvBuff, "MOSTRAR-RECOMENDACIONES-END") != 0) {
                printf("Error: Comando MOSTRAR-RECOMENDACIONES-END no recibido correctamente\n");
                commandLog("Error: Comando MOSTRAR-RECOMENDACIONES-END no recibido correctamente");
            }
        }

        if (strcmp(recvBuff, "ELIMINAR-LIBRO-LISTA") == 0) {
            Libro libro;
            memset(&libro, 0, sizeof(Libro)); // Inicializa todo a cero

            recv_size = recv(comm_socket, libro.titulo, sizeof(libro.titulo) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir titulo");
                commandLog("Error al recibir titulo");
                continue;
            }
            libro.titulo[recv_size] = '\0'; // Asegura terminador nulo

            int status;
            status = eliminarLibroMenu(cl.id_Cliente,  libro.titulo);

            memset(sendBuff, 0, sizeof(sendBuff));
            if(status == SQLITE_OK) {
                strcpy(sendBuff, "El libro se ha eliminado correctamente\n");
                commandLog("El libro se ha eliminado correctamente");
            } else {
                strcpy(sendBuff, "El libro no se ha eliminado correctamente\n");
                commandLog("El libro no se ha eliminado correctamente");
            }
            send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);


            memset(recvBuff, 0, sizeof(recvBuff));
            recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            if (recv_size <= 0) {
                perror("Error al recibir ELIMINAR-LIBRO-LISTA-END");
                commandLog("Error al recibir ELIMINAR-LIBRO-LISTA-END");
                continue;
            }

            if (strcmp(recvBuff, "ELIMINAR-LIBRO-LISTA-END") != 0) {
                printf("Error: Comando ELIMINAR-LIBRO-LISTA-END no recibido correctamente\n");
                commandLog("Error: Comando ELIMINAR-LIBRO-LISTA-END no recibido correctamente");
            }
        }

        if (strcmp(recvBuff, "DESCARGAR-LIBRO") == 0) {
            Libro libro;
            memset(&libro, 0, sizeof(Libro)); // Inicializa todo a cero

            recv_size = recv(comm_socket, libro.titulo, sizeof(libro.titulo) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir titulo");
                commandLog("Error al recibir titulo");
                continue;
            }
            libro.titulo[recv_size] = '\0'; // Asegura terminador nulo

            int status;
            status = descargarLibro(libro.titulo);

            memset(sendBuff, 0, sizeof(sendBuff));
            if(status == SQLITE_OK) {
                strcpy(sendBuff, "El libro se ha descargado correctamente\n");
                commandLog("El libro se ha descargado correctamente");
            } else {
                strcpy(sendBuff, "El libro no se ha descargado correctamente\n");
                commandLog("El libro no se ha descargado correctamente");
            }
            send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);


            memset(recvBuff, 0, sizeof(recvBuff));
            recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            if (recv_size <= 0) {
                perror("Error al recibir DESCARGAR-LIBRO-END");
                commandLog("Error al recibir DESCARGAR-LIBRO-END");
                continue;
            }

            if (strcmp(recvBuff, "DESCARGAR-LIBRO-END") != 0) {
                printf("Error: Comando DESCARGAR-LIBRO-END no recibido correctamente\n");
                commandLog("Error: Comando DESCARGAR-LIBRO-END no recibido correctamente");
            }
        }

        if (strcmp(recvBuff, "LEER-LIBRO") == 0) {
            // Código para manejar LEER-LIBRO
            Libro libro;
            memset(&libro, 0, sizeof(Libro)); // Inicializa todo a cero

            recv_size = recv(comm_socket, libro.titulo, sizeof(libro.titulo) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir título del libro");
                commandLog("Error al recibir título del libro");
                continue;
            }
            libro.titulo[recv_size] = '\0'; // Asegura terminador nulo

            leerLibro(comm_socket, libro.titulo);

            memset(recvBuff, 0, sizeof(recvBuff));
            recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            if (recv_size <= 0) {
                perror("Error al recibir LEER-LIBRO-END");
                commandLog("Error al recibir LEER-LIBRO-END");
                continue;
            }

            if (strcmp(recvBuff, "LEER-LIBRO-END") != 0) {
                printf("Error: Comando LEER-LIBRO-END no recibido correctamente\n");
                commandLog("Error: Comando LEER-LIBRO-END no recibido correctamente");
            }


        }

        if (strcmp(recvBuff, "MOSTRAR-LISTA") == 0) {
            // Código para manejar MOSTRAR-LISTA
            char* miLista = mostrarMiLista(cl.id_Cliente);
            if (miLista == NULL) {
                perror("Error al obtener la lista");
                commandLog("Error al obtener la lista");
                continue;
            }

            memset(sendBuff, 0, sizeof(sendBuff));
            strncpy(sendBuff, miLista, sizeof(sendBuff) - 1);
            send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);

            free(miLista);  // Liberar memoria asignada por mostrarMiLista

            memset(recvBuff, 0, sizeof(recvBuff));
            recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir MOSTRAR-LISTA-END");
                commandLog("Error al recibir MOSTRAR-LISTA-END");
                continue;
            }

            if (strcmp(recvBuff, "MOSTRAR-LISTA-END") != 0) {
                printf("Error: Comando MOSTRAR-LISTA-END no recibido correctamente\n");
                commandLog("Error: Comando MOSTRAR-LISTA-END no recibido correctamente");
            }
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


        if (strcmp(recvBuff, "BUSCAR-LIBRO") == 0) {
            memset(&cl, 0, sizeof(Cliente)); // Inicializa todo a cero


            buscarLibro(comm_socket);

            memset(recvBuff, 0, sizeof(recvBuff));
            recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            if (recv_size <= 0) {
                perror("Error al recibir BUSCAR-LIBRO-END");
                commandLog("Error al recibir BUSCAR-LISTA-END");
                continue;
            }

            if (strcmp(recvBuff, "BUSCAR-LIBRO-END") != 0) {
                printf("Error: Comando BUSCAR-LIBRO-END no recibido correctamente\n");
                commandLog("Error: Comando BUSCAR-LIBRO-END no recibido correctamente");
            }
        }

        if (strcmp(recvBuff, "AGREGAR-LIBRO-BD") == 0) {
            Libro libro;
            memset(&libro, 0, sizeof(Libro)); // Inicializa todo a cero

            recv_size = recv(comm_socket, libro.titulo, sizeof(libro.titulo) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir titulo");
                commandLog("Error al recibir titulo");
                continue;
            }
            libro.titulo[recv_size] = '\0'; // Asegura terminador nulo

            recv_size = recv(comm_socket, libro.autor_Libro, sizeof(libro.autor_Libro) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir autor");
                commandLog("Error al recibir autor");
                continue;
            }
            libro.autor_Libro[recv_size] = '\0'; // Asegura terminador nulo

            recv_size = recv(comm_socket, libro.idioma, sizeof(libro.idioma) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir idioma");
                commandLog("Error al recibir idioma");
                continue;
            }
            libro.idioma[recv_size] = '\0'; // Asegura terminador nulo

            recv_size = recv(comm_socket, libro.fecha_Publicacion, sizeof(libro.fecha_Publicacion) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir fecha publicacion");
                commandLog("Error al recibir fecha publicacion");
                continue;
            }
            libro.fecha_Publicacion[recv_size] = '\0'; // Asegura terminador nulo


            int status;
            status = agregarLibroMenu(libro.titulo,libro.autor_Libro, libro.idioma, libro.fecha_Publicacion);

            memset(sendBuff, 0, sizeof(sendBuff));
            if(status == SQLITE_OK) {
                strcpy(sendBuff, "El libro se ha agregado correctamente a la BD\n");
                commandLog("El libro se ha agregado correctamente a la BD");
            } else {
                strcpy(sendBuff, "El libro no se ha agregado correctamente a la BD\n");
                commandLog("El libro no se ha agregado correctamente a la BD");
            }
            send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);


            memset(recvBuff, 0, sizeof(recvBuff));
            if (strcmp(recvBuff, "AGREGAR-LIBRO-BD-END") != 0) {
                printf("Error: AGREGAR-LIBRO-BD-END no recibido correctamente\n");
                commandLog("Error: AGREGAR-LIBRO-BD-END no recibido correctamente");
            }
        }

        if (strcmp(recvBuff, "ELIMINAR-LIBRO-BD") == 0) {
            Libro libro;
            memset(&libro, 0, sizeof(Libro)); // Inicializa todo a cero

            recv_size = recv(comm_socket, libro.titulo, sizeof(libro.titulo) - 1, 0);
            if (recv_size <= 0) {
                perror("Error al recibir titulo");
                commandLog("Error al recibir titulo");
                continue;
            }
            libro.titulo[recv_size] = '\0'; // Asegura terminador nulo



            int status;
            status = eliminarLibroBD(libro.titulo);

            memset(sendBuff, 0, sizeof(sendBuff));
            if(status == SQLITE_OK) {
                strcpy(sendBuff, "El libro se ha eliminado correctamente de la BD\n");
                commandLog("El libro se ha eliminado correctamente de la BD");
            } else {
                strcpy(sendBuff, "El libro NO se ha eliminado correctamente de la BD\n");
                commandLog("El libro NO se ha eliminado correctamente de la BD");
            }
            send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);


            memset(recvBuff, 0, sizeof(recvBuff));
            recv_size = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            if (recv_size <= 0) {
                perror("Error al recibir ELIMINAR-LIBRO-BD-END");
                commandLog("Error al recibir ELIMINAR-LIBRO-BD-END");
                continue;
            }

            if (strcmp(recvBuff, "ELIMINAR-LIBRO-BD-END") != 0) {
                printf("Error: Comando ELIMINAR-LIBRO-BD-END no recibido correctamente\n");
                commandLog("Error: Comando ELIMINAR-LIBRO-BD-END no recibido correctamente");
            }
        }


        if (strcmp(recvBuff, "EXIT") == 0)
            break;
    } while (1);

    closesocket(comm_socket);
    WSACleanup();

    return 0;
}



void leerLibro(SOCKET client_socket, const string& titulo) {
    int id_titulo = peticionIdLibroPorTitulo(titulo);
    ifstream archivo("../libros/" + titulo + ".txt"); // Ajusta la ruta del archivo según sea necesario
    vector<string> lineas;
    string linea;

    archivo.clear();
    archivo.seekg(0);

    while (getline(archivo, linea)) {
        lineas.push_back(linea);
    }

    int inicio = cargarProgreso(id_titulo) * convPag;
    int opcion = 0;
    char recvBuff[2048];
    do {
        stringstream ss;
        int end = min(inicio + convPag, static_cast<int>(lineas.size()));
        for (int i = inicio; i < end; ++i) {
            ss << lineas[i] << endl;
        }
        string result = ss.str();
        send(client_socket, result.c_str(), result.size() + 1, 0);

        memset(recvBuff, 0, sizeof(recvBuff));
        int recv_size = recv(client_socket, recvBuff, sizeof(recvBuff), 0);
        if (recv_size <= 0) {
            perror("Error al recibir comando de lectura");
            commandLog("Error al recibir comando de lectura");
            break;
        }
        sscanf(recvBuff, "%d", &opcion);

        if (opcion == 2) { // Siguiente página
            inicio += convPag;
        } else if (opcion == 3) { // Página anterior
            inicio = max(inicio - convPag, 0);
        }
    } while (opcion != 4);

    archivo.close();
    actualizarProgreso(id_titulo,inicio/convPag, actualTime); // Guardar progreso antes de salir

}

void buscarLibro(SOCKET client_socket) {
    char opcion[10];
    char recvBuff[1024];
    char* resultados;

    // Recibir la opción seleccionada por el cliente
    int recv_size = recv(client_socket, recvBuff, sizeof(recvBuff) - 1, 0);
    if (recv_size <= 0) {
        perror("Error al recibir opción de búsqueda");
        commandLog("Error al recibir opción de búsqueda");
        return;
    }
    recvBuff[recv_size] = '\0';
    strcpy(opcion, recvBuff);

    if (strcmp(opcion, "1") == 0) {
        // Búsqueda por título
        char titulo[100];
        recv_size = recv(client_socket, recvBuff, sizeof(recvBuff) - 1, 0);
        if (recv_size <= 0) {
            perror("Error al recibir título");
            commandLog("Error al recibir título");
            return;
        }
        recvBuff[recv_size] = '\0';
        strcpy(titulo, recvBuff);
        resultados = peticionTitulo(titulo);
    } else if (strcmp(opcion, "2") == 0) {
        // Búsqueda por autor
        char autor[100];
        recv_size = recv(client_socket, recvBuff, sizeof(recvBuff) - 1, 0);
        if (recv_size <= 0) {
            perror("Error al recibir autor");
            commandLog("Error al recibir autor");
            return;
        }
        recvBuff[recv_size] = '\0';
        strcpy(autor, recvBuff);
        resultados = peticionAutor(autor);

    } else {
        strcpy(recvBuff, "Opcion no valida.");
        send(client_socket, recvBuff, strlen(recvBuff) + 1, 0);
        return;
    }
    // Enviar los resultados al cliente
    send(client_socket, resultados, strlen(resultados) + 1, 0);

    // Liberar memoria asignada para los resultados
    free(resultados);
}
