#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "../sqlite/sqlite3.h"
#include "../database/db2.h"
#include "../database/dbBusqueda.h"
#include "../curly/descargaArchivos.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#define convPag 20

#define NEGRITA "\e[1m" // Renombramos el código de los caracteres en negrita para que sea mas entendible
#define QUITAR_NEGRITA "\e[m" // Renombramos el codigo de quitar la negrita a los caracteres para que sea mas entendible
//"Sin dolor no hay gloria" - Proverbio Romano
int id_cliente_actual = -1;
time_t rawtime;
struct tm* timeinfo;
char actualTime[80];
int es_admin = 0;

void imprimirInicial(){
    char input[10];
    char nombre[31];
    char email[101];
    char pass[31];
    char sel;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(actualTime, sizeof(actualTime), "%Y-%m-%d", timeinfo);

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
            strtok(email, "\n"); // Elimina el carácter

            // Verificar si el correo electrónico termina con "@opendeusto.es"
            if (endsWith(email, "@opendeusto.es") || endsWith(email, "@deusto.es")) {
                es_admin = 1;
            }else {
                es_admin = 0;
            }
            printf("\nIntroduce tu contrasenya:\n");
            fgets(pass, sizeof(pass), stdin);
            strtok(pass, "\n"); // Elimina el carácter

            iniciarSesionMenu(email,pass);

        }else if(sel == '2'){
            printf("\nIntroduce tus datos para registrarte:\nIntroduce tu nombre:\n(30 caracteres como maximo)\n");
            fgets(nombre, sizeof(nombre), stdin);
            strtok(nombre, "\n"); // Elimina el carácter

            printf("\nIntroduce tu correo electronico:\n");
            fgets(email, sizeof(email), stdin);
            strtok(email, "\n"); // Elimina el carácter

            printf("\nIntroduce tu contrasenya:\n(30 caracteres como maximo)\n");
            fgets(pass, sizeof(pass), stdin);
            strtok(pass, "\n"); // Elimina el carácter

            registrarClienteMenu(nombre,email,pass);

        }else if(sel == '3'){
            imprimirMenuInvitado();
        }else if(sel == '4'){
            printf("\ncerrando...\n");
        }else{
            printf("\nIntroduce un valor valido\n\n");
    }

    }while(sel != '4');
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

    if(es_admin == 1) {
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
                buscarLibro();

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

                agregarLibroMenu(titulo, nom_autor, idioma, fecha_publicacion);
            }else if(sel == '4'){

            }else if(sel == '5'){
                printf("\ncerrando sesion...\n\n");
                imprimirInicial();
            }else{
                printf("\nIntroduce un valor valido\n\n");
            }

        }while(sel != '5');
    }else {
        do{
            printf("\nSelecciona una opcion: \n");
            printf("1. Mi Lista\n2.Buscar Libro\n3.Volver\n");

            fgets(input, sizeof(input), stdin);
            sscanf(input, " %c", &sel);

            if(sel == '1'){
                printf("\ncorrecto 1\n\n");
                menuMiLista();

            }else if(sel == '2'){
                printf("\ncorrecto 2\n\n");
                buscarLibro();

            }else if(sel == '3'){
                printf("\ncerrando sesion...\n\n");
                imprimirInicial();
            }else{
                printf("\nIntroduce un valor valido\n\n");
            }

        }while(sel != '3');
    }

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

        mostrarMiLista(id_cliente_actual);
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
            aportarLibroMenu(titulo, actualTime);
        }else if(sel == '2'){
            printf("\ncorrecto 2\n\n");
            printf("\nIntroduce nombre del libro que quiera eliminar de su lista: \n");
            fgets(titulo, sizeof(titulo), stdin);
            strtok(titulo, "\n"); // Elimina el carácter
            eliminarLibroMenu(titulo);
        }else if(sel == '3'){
            printf("\ncorrecto 3\n\n");
            printf("\nIntroduce nombre del libro que quieras descargar en su lista: \n");
            fgets(titulo, sizeof(titulo), stdin);
            strtok(titulo, "\n"); // Elimina el carácter
            descargarLibro(titulo);

        }else if(sel == '4'){
            printf("\ncorrecto 4\n\n");
            printf("\nIntroduce nombre del libro que quieras leer: \n");
            fgets(titulo, sizeof(titulo), stdin);
            strtok(titulo, "\n"); // Elimina el carácter
            leerLibro(titulo);
            menuMiLista();
            //leer libro
        }else if(sel == '5'){
            printf("\nvolviendo...\n\n");
            imprimirMenu();
        }else{
            printf("\nIntroduce un valor valido\n\n");
        }

    }while(sel != '5');
}

void imprimirMenuInvitado(){
    char input[10];
    char sel;

    printf(NEGRITA"Menu Principal\nSesion de invitado\n\n" QUITAR_NEGRITA);


    do{
        printf("\nSelecciona una opcion: \n");
        printf("1.Gestionar Contenido\n2. Cerrar Sesion\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel);

        if(sel == '1'){
            printf("\ncorrecto 2\n\n");
            imprimirGestionInvitado();
        }else if(sel == '2'){
            printf("\ncerrando sesion...\n\n");
            imprimirInicial();
        }else{
            printf("\nIntroduce un valor valido\n\n");
    }

    }while(sel != '2' && sel != '1');


}

/*void imprimirGestion(){
    char input[10];
    char sel;
    char titulo[30];
    char nom_autor[30];
    char idioma[10];
    char fecha_publicacion[11];
    char fecha_actual[10];

    printf(NEGRITA"Menu de Gestion de Contenido\nUsuario: \n\n" QUITAR_NEGRITA);


    do{
        printf("\nSelecciona una opcion: \n");
        printf("1. Agregar Libro a la BD\n2. Agregar Libro a nuestra Lista\n3. Eliminar Libro de nuestra Lista\n4. Descargar Libro\n5. Buscar Libro\n6. Volver\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel);

        if(sel == '1'){
            printf("\ncorrecto 1\n\n");
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

            agregarLibroMenu(titulo, nom_autor, idioma, fecha_publicacion);

        }else if(sel == '2'){
            printf("\ncorrecto 2\n\n");
            printf("\nIntroduce el titulo del libro que desea guardar en tu lista: \n");
            fgets(titulo, sizeof(titulo), stdin);
            strtok(titulo, "\n"); // Elimina el carácter
            aportarLibroMenu(titulo, actualTime);

        }else if(sel == '3'){
            printf("\ncorrecto 3\n\n");
            printf("\nIntroduce nombre del libro que quiera eliminar de su lista: \n");
            fgets(titulo, sizeof(titulo), stdin);
            strtok(titulo, "\n"); // Elimina el carácter
            eliminarLibroMenu(titulo);

        }else if(sel == '4'){
            printf("\ncorrecto 4\n\n");
            descargarLibro();
        }else if(sel == '5'){
            printf("\nvolviendo...\n\n");
            buscarLibro();
        }else if(sel == '6'){
            printf("\nvolviendo...\n\n");
            imprimirMenu();
        }
        else{
            printf("\nIntroduce un valor valido\n\n");
    }

    }while(sel != '3' && sel != '2' && sel != '1');

}
*/

void imprimirGestionInvitado(){
    char input[10];
    char sel;
    char titulo[30];
    char fecha_actual[30];

    printf(NEGRITA"Menu de Gestion de Contenido\nSesion de invitado\n\n" QUITAR_NEGRITA);


    do{
        printf("\nSelecciona una opcion: \n");
        printf("1. Buscar Libro\n2. Aportar Libro\n3. Volver\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel);

        if(sel == '1'){
            printf("\ncorrecto 1\n\n");
            buscarLibro();
        }else if(sel == '2'){
            printf("\ncorrecto 2\n\n");
            printf("\nIntroduce el titulo del libro que desea guardar en tu lista: \n");
            fgets(titulo, sizeof(titulo), stdin);
            strtok(titulo, "\n"); // Elimina el carácter
            aportarLibroMenu(titulo, actualTime);
        }else if(sel == '3'){
            printf("\nvolviendo...\n\n");
            imprimirMenuInvitado();
        }
        else{
            printf("\nIntroduce un valor valido\n\n");
    }

    }while(sel != '3' && sel != '2' && sel != '1');

}



//apartado gestion usuarios
void iniciarSesionMenu(char email_cl[], char pass_cl[]){
    sqlite3 *db;
    int rc = sqlite3_open("libreria.db", &db);
    if (rc != SQLITE_OK) {
        //errorMsgg("Error opening database\n");
        printf("Error opening database\n");
        return;
    }

    id_cliente_actual = iniciarSesion(db, email_cl, pass_cl);
    if (id_cliente_actual != -1) {
        printf("Inicio de sesion exitoso. Bienvenido\n");
        imprimirMenu();
    } else {
        printf("Inicio de sesion fallido. Verifica tus credenciales.\n");
    }

    rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        printf("Error opening database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }
}




void registrarClienteMenu(char nom_cl[], char email_cl[], char pass_cl[]){
    sqlite3 *db;
    int rc = sqlite3_open("libreria.db", &db);
    if (rc != SQLITE_OK) {
        //////errorMsgg("Error opening database\n");
        printf("Error opening database\n");
        return;
    }

    rc = registrarCliente(db, nom_cl, email_cl, pass_cl);
    if (rc != SQLITE_OK) {
        printf("Error inserting new data\n");
        printf("%s\n", sqlite3_errmsg(db));
    } else {
        printf("Cliente registrado exitosamente\n");
    }

    rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        printf("Error closing database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }
}

//apartado gestion de contenido de la db
void agregarLibroMenu(char titulo[], char nom_autor[], char idioma[], char fecha_publicacion[]){
    //debria contectarse y agregar un libro a la db
    sqlite3 *db;
    int rl = sqlite3_open("libreria.db", &db);
    if (rl != SQLITE_OK) {
        //////errorMsgg("Error opening database\n");
        printf("Error opening database\n");
        return;
    }
    rl = agregarLibro(db, titulo, nom_autor, idioma, fecha_publicacion);
    if (rl != SQLITE_OK) {
        printf("Error inserting new data\n");
        printf("%s\n", sqlite3_errmsg(db));
    } else {
        printf("Libro agregado exitosamente\n");
    }

    rl = sqlite3_close(db);
    if (rl != SQLITE_OK) {
        printf("Error closing database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }
}
void eliminarLibroMenu(char titulo[]){
    //debria contectarse a la db y eliminar un libro de la lista personal
    sqlite3 *db;
    int al = sqlite3_open("libreria.db", &db);
    if (al != SQLITE_OK) {
        //////errorMsgg("Error opening database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }

    // Intenta agregar el libro
    al = eliminarLibro(db, titulo);

    if (al != SQLITE_OK) {
        printf("Error inserting new datan");
        printf("%s\n", sqlite3_errmsg(db));
    } else {
        printf("Libro eliminado exitosamente\n");
    }

    // Cierra la base de datos
    al = sqlite3_close(db);
    if (al != SQLITE_OK) {
        printf("Error closing database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }
}
void aportarLibroMenu(char titulo[], char fecha_lec[]){
    //debria contectarse y agregar un libro de los disponibles en la db a la lista personal
    sqlite3 *db;
        int al = sqlite3_open("libreria.db", &db);
    if (al != SQLITE_OK) {
        //////errorMsgg("Error opening database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }

    // Intenta agregar el libro
    al = agregarLibroMiLista(db, id_cliente_actual ,titulo, fecha_lec);

    if (al != SQLITE_OK) {
        printf("Error inserting new data\n");
        printf("%s\n", sqlite3_errmsg(db));
    } else {
        printf("Libro agregado exitosamente\n");
    }

    // Cierra la base de datos
    al = sqlite3_close(db);
    if (al != SQLITE_OK) {
        printf("Error closing database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }
}
void guardarProgresoListaPersonal(int id_cliente, char titulo[], char fecha_lec[], int pag_actual) {
    sqlite3 *db;
    int al = sqlite3_open("libreria.db", &db);
    if (al != SQLITE_OK) {
        //////errorMsg("Error opening database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }

    al = guardarProgreso(db, id_cliente, titulo, fecha_lec, 0);
    if (al != SQLITE_OK) {
        printf("Error inserting new data\n");
        printf("%s\n", sqlite3_errmsg(db));
    } else {
        printf("Progreso del libro guardado exitosamente\n");
    }

    al = sqlite3_close(db);
    if (al != SQLITE_OK) {
        printf("Error closing database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }

    return;
}
void descargarLibro(char *titulo){

    peticionAutorPorTitulo(titulo);
}

void buscarLibro(){
   char opcion[10]; // Opción seleccionada por el usuario

    // Solicitar al usuario que ingrese el tipo de búsqueda
    printf("Selecciona el tipo de busqueda:\n");
    printf("1. Por titulo\n");
    printf("2. Por autor\n");
    printf("Opcion: ");
    fgets(opcion, sizeof(opcion), stdin);
    opcion[strcspn(opcion, "\n")] = 0; // Eliminar el salto de línea

    if (strcmp(opcion, "1") == 0) {
        // Búsqueda por título
        char titulo[100];
        printf("Ingresa el titulo del libro a buscar: ");
        fgets(titulo, sizeof(titulo), stdin);
        titulo[strcspn(titulo, "\n")] = 0; // Eliminar el salto de línea
        peticionTitulo(titulo);
    } else if (strcmp(opcion, "2") == 0) {
        // Búsqueda por autor
        char autor[100];
        printf("Ingresa el nombre del autor a buscar: ");
        fgets(autor, sizeof(autor), stdin);
        autor[strcspn(autor, "\n")] = 0; // Eliminar el salto de línea
        peticionAutor(autor);
    } else {
        printf("Opcion no válida.\n");
    }
}


void imprimirLineas(const vector<string>& lines, int start) {
    int end = min(start + 20, static_cast<int>(lines.size()));
    for (int i = start; i < end; ++i) {
        cout << lines[i] << endl;
    }
}
void leerLibro(string titulo) {
    ifstream archivo("../libros/"+ titulo +".txt");
    if (!archivo) {
        cerr << "No se pudo abrir el archivo." << endl;
        return;
    }
    int id_titulo = peticionIdLibroPorTitulo(titulo);
    vector<string> lineas;
    string linea;
    while (getline(archivo, linea)) {
        lineas.push_back(linea);
    }

    int inicio = cargarProgreso(id_titulo) * convPag; // Cargar progreso almacenado
    int opcion;
    do {
        imprimirLineas(lineas, inicio);
        cout << "Opciones: 1. Continuar, 2. Retroceder, 3. Salir: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                inicio += convPag;
                break;
            case 2:
                inicio = max(0, inicio - convPag);
                break;
            case 3:
                cout << "Guardando progreso..." << endl;
                actualizarProgreso(id_titulo,inicio/convPag, actualTime); // Guardar progreso antes de salir
                cout << "Saliendo del programa." << endl;
                break;
            default:
                cout << "Opción inválida. Inténtalo de nuevo." << endl;
        }
    } while (opcion != 3);
}



