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
#include <chrono> // Para utilizar std::this_thread::sleep_for
#include <thread> // Para utilizar std::chrono::milliseconds


using namespace std;

#define convPag 20

#define NEGRITA ""//"\e[1m" // Renombramos el código de los caracteres en negrita para que sea mas entendible
#define QUITAR_NEGRITA "" //"\e[m" // Renombramos el codigo de quitar la negrita a los caracteres para que sea mas entendible
//"Sin dolor no hay gloria" - Proverbio Romano
int id_cliente_actual = -1;
time_t rawtime;
struct tm* timeinfo;
char actualTime[80];
int es_admin = 0;
bool salir = false; // Bandera para salir del programa


void imprimirInicial() {
    char input[10];
    char nombre[31];
    char email[101];
    char pass[31];
    char sel;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(actualTime, sizeof(actualTime), "%Y-%m-%d", timeinfo);

    cout << "\nProyecto Programacion IV | Grupo PVI-04\n\n";
    cout << NEGRITA << "Bienvenido al Sistema de Libreria Virtual\nVerseVault\n\n" << QUITAR_NEGRITA;

    do {
        cout << "\nSelecciona una opcion: \n";
        cout << "1. Iniciar Sesion \n2. Registrar cuenta \n3. Continuar como invitado \n4. Salir\n\n";

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel);

        if (sel == '1') {
            cout << "\nIntroduce tus datos para iniciar sesion:\nIntroduce tu correo electronico:\n";
            fgets(email, sizeof(email), stdin);
            strtok(email, "\n"); // Elimina el carácter

            // Verificar si el correo electrónico termina con "@opendeusto.es"
            if (endsWith(email, "@opendeusto.es") || endsWith(email, "@deusto.es")) {
                es_admin = 1;
            } else {
                es_admin = 0;
            }
            cout << "\nIntroduce tu contrasenya:\n";
            fgets(pass, sizeof(pass), stdin);
            strtok(pass, "\n"); // Elimina el carácter

            iniciarSesionMenu(email, pass);

        } else if (sel == '2') {
            cout << "\nIntroduce tus datos para registrarte:\nIntroduce tu nombre:\n(30 caracteres como maximo)\n";
            fgets(nombre, sizeof(nombre), stdin);
            strtok(nombre, "\n"); // Elimina el carácter

            cout << "\nIntroduce tu correo electronico:\n";
            fgets(email, sizeof(email), stdin);
            strtok(email, "\n"); // Elimina el carácter

            cout << "\nIntroduce tu contrasenya:\n(30 caracteres como maximo)\n";
            fgets(pass, sizeof(pass), stdin);
            strtok(pass, "\n"); // Elimina el carácter

            registrarClienteMenu(nombre, email, pass);

        } else if (sel == '3') {
            imprimirMenuInvitado();
        } else if (sel == '4') {
            cout << "\ncerrando...\n";
            salir = true; // Establecer la bandera para salir del programa
        } else {
            cout << "\nIntroduce un valor valido\n\n";
        }

    } while (sel != '4' && !salir);
}
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void imprimirMenu() {
    char input[10];
    char sel;
    char titulo[30];
    char nom_autor[30];
    char idioma[10];
    char fecha_publicacion[11];

    bool salir = false; // Bandera para salir del bucle

    printf(NEGRITA "Menu Principal\n\n" QUITAR_NEGRITA);

    if (es_admin == 1) {
        do {
            printf("\nSelecciona una opcion: \n");
            printf("1. Mi Lista\n2. Buscar Libro\n3. Agregar Libro BD (admin only)\n4. Eliminar Libro BD (admin only)\n5. Volver\n");

            fgets(input, sizeof(input), stdin);
            sscanf(input, " %c", &sel);

            switch (sel) {
                case '1':
                    printf("\ncorrecto 1\n\n");
                    menuMiLista();
                    break;
                case '2':
                    printf("\ncorrecto 2\n\n");
                    buscarLibro();
                    break;
                case '3':
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
                    break;
                case '4':
                    // Agregar código para eliminar libro BD
                    printf("\nIntroduce nombre del libro que quiera eliminar de la BD: \n");
                    fgets(titulo, sizeof(titulo), stdin);
                    strtok(titulo, "\n"); // Elimina el carácter
                    eliminarLibroBD(titulo);
                    imprimirMenu();
                    break;
                case '5':
                    printf("\ncerrando sesion...\n\n");
                    salir = true;
                    imprimirInicial();
                    break;
                default:
                    printf("\nIntroduce un valor valido\n\n");
                    break;
            }
            limpiarBuffer();
        } while (!salir);
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
                    buscarLibro();
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

void menuMiLista() {
    char input[10];
    char sel;
    char titulo[200];
    char nom_autor[30];
    char idioma[10];
    char fecha_publicacion[11];
    char fecha_actual[10];

    printf(NEGRITA"Menu Mi Lista\n\n" QUITAR_NEGRITA);

    bool salir = false; // Bandera para salir del bucle

    do {
        mostrarMiLista(id_cliente_actual);
        printf("\nSelecciona una opcion: \n");
        printf("1. Agregar Libro a Mi Lista\n2. Eliminar Libro de Mi Lista\n3. Descargar Libro\n4. Leer Libro\n5. Volver\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel);

        switch (sel) {
            case '1':
                printf("\ncorrecto 1\n\n");
                mostrarRecomendaciones();
                printf("\nIntroduce el titulo del libro que desea guardar en tu lista: \n");
                fgets(titulo, sizeof(titulo), stdin);
                strtok(titulo, "\n"); // Elimina el carácter
                //aportarLibroMenu(titulo, actualTime);
                break;
            case '2':
                printf("\ncorrecto 2\n\n");
                printf("\nIntroduce nombre del libro que quiera eliminar de su lista: \n");
                fgets(titulo, sizeof(titulo), stdin);
                strtok(titulo, "\n"); // Elimina el carácter
                //eliminarLibroMenu(titulo);
                break;
            case '3':
                printf("\ncorrecto 3\n\n");
                printf("\nIntroduce nombre del libro que quieras descargar en su lista: \n");
                fgets(titulo, sizeof(titulo), stdin);
                strtok(titulo, "\n"); // Elimina el carácter
                descargarLibro(titulo);
                break;
            case '4':
                printf("\ncorrecto 4\n\n");
                printf("\nIntroduce nombre del libro que quieras leer: \n");
                fgets(titulo, sizeof(titulo), stdin);
                strtok(titulo, "\n"); // Elimina el carácter
                if (CheckleerLibro(titulo)) {
                    // Si la lectura del libro fue exitosa, salimos del bucle del menú
                    salir = true;
                }
                // No hay necesidad de llamar a menuMiLista() aquí
                break;
            case '5':
                printf("\nvolviendo...\n\n");
                salir = true;
                break;
            default:
                printf("\nIntroduce un valor valido\n\n");
                break;
        }

    } while (!salir);
}

void imprimirMenuInvitado() {
    char input[10];
    char sel;

    cout << NEGRITA << "Menu Principal\nSesion de invitado\n\n" << QUITAR_NEGRITA;

    do {
        cout << "\nSelecciona una opcion: \n";
        cout << "1. Gestionar Contenido\n2. Cerrar Sesion\n";

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel);

        if (sel == '1') {
            cout << "\ncorrecto 2\n\n";
            imprimirGestionInvitado();
        } else if (sel == '2') {
            cout << "\ncerrando sesion...\n\n";
            return; // Salir del menú de invitado
        } else {
            cout << "\nIntroduce un valor valido\n\n";
        }

    } while (sel != '2' && sel != '1');
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

void imprimirGestionInvitado() {
    char input[10];
    char sel;
    char titulo[30];
    char nom_autor[30];
    char idioma[10];
    char fecha_publicacion[11];

    printf(NEGRITA "Menu de Gestion de Contenido\nSesion de invitado\n\n" QUITAR_NEGRITA);

    do {
        printf("\nSelecciona una opcion: \n");
        printf("1. Buscar Libro\n2. Aportar Libro a la BD\n3. Volver\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel);

        switch (sel) {
            case '1':
                printf("\ncorrecto 1\n\n");
                buscarLibro();
                break;
            case '2':
                printf("\ncorrecto 2\n\n");
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
    } while (sel != '3' && sel != '2' && sel != '1');
}




//apartado gestion usuarios
int iniciarSesionMenu(char email_cl[], char pass_cl[]){
    sqlite3 *db;
    int rc = sqlite3_open("libreria.db", &db);
    if (rc != SQLITE_OK) {
        //errorMsgg("Error opening database\n");
        printf("Error opening database\n");
        return -1;
    }
    int id_cliente_actu = -1;
    id_cliente_actu = iniciarSesion(db, email_cl, pass_cl);
    if (id_cliente_actu != -1) {
        printf("Inicio de sesion exitoso. Bienvenido\n");
        //imprimirMenu();
    } else {
        printf("Inicio de sesion fallido. Verifica tus credenciales.\n");
    }

    rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        printf("Error opening database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return -1;
    }
    return id_cliente_actu;
}




int registrarClienteMenu(char nom_cl[], char email_cl[], char pass_cl[]){
    sqlite3 *db;
    int rc = sqlite3_open("libreria.db", &db);
    if (rc != SQLITE_OK) {
        //////errorMsgg("Error opening database\n");
        printf("Error opening database\n");
        return -1;
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
        return -1;
    }
    return rc;
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
int eliminarLibroMenu(int id_cliente, char titulo[]){
    //debria contectarse a la db y eliminar un libro de la lista personal
    sqlite3 *db;
    int al = sqlite3_open("libreria.db", &db);
    if (al != SQLITE_OK) {
        //////errorMsgg("Error opening database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return -1;
    }

    // Intenta agregar el libro
    al = eliminarLibro(db, id_cliente, titulo);

    if (al != SQLITE_OK) {
        printf("Error deleting data");
        printf("%s\n", sqlite3_errmsg(db));
    } else {
        printf("Libro eliminado exitosamente\n");
    }

    // Cierra la base de datos
    al = sqlite3_close(db);
    if (al != SQLITE_OK) {
        printf("Error closing database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return -1;
    }
    return al;
}
int aportarLibroMenu(int id_cliente, char titulo[], char fecha_lec[]){
    //debria contectarse y agregar un libro de los disponibles en la db a la lista personal
    sqlite3 *db;
        int al = sqlite3_open("libreria.db", &db);
    if (al != SQLITE_OK) {
        //////errorMsgg("Error opening database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return -1;
    }

    // Intenta agregar el libro
    al = agregarLibroMiLista(db, id_cliente ,titulo, fecha_lec);

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
        return -1;
    }
    return al;
}

void eliminarLibroBD(char titulo[]){
    //debria contectarse a la db y eliminar un libro de la lista personal
    sqlite3 *db;
    int al = sqlite3_open("libreria.db", &db);
    if (al != SQLITE_OK) {
        //////errorMsgg("Error opening database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }

    // Intenta agregar el libro
    al = eliminarLibroBD1(db, titulo);

    if (al != SQLITE_OK) {
        printf("Error deleting data");
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
// Verificar si el archivo del libro ya existe en la carpeta
    ifstream archivo("../libros/" + string(titulo) + ".txt");
    if (archivo.good()) {
        cout << "El libro '" << titulo << "' ya esta descargado." << endl;
        return; // Si el libro ya está en la carpeta, salir de la función
    }
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
        printf("Opcion no valida.\n");
    }
}


void imprimirLineas(const vector<string>& lines, int start) {
    int end = min(start + 20, static_cast<int>(lines.size()));
    for (int i = start; i < end; ++i) {
        cout << lines[i] << endl;
    }
}

bool CheckleerLibro(string titulo) {
    ifstream archivo("../libros/" + titulo + ".txt");
    if (!archivo) {
        //cerr << "El archivo del libro no esta disponible localmente." << endl;
        cout << "Deseas descargarlo? (Si = 1, No = 0): \n";
        int opcion;
        cin >> opcion;
        char tituloChar[titulo.length() + 1]; // +1 para el carácter nulo al final
        strcpy(tituloChar, titulo.c_str());
        if (opcion == 1) {
            descargarLibro(tituloChar);
            // Crear un nuevo objeto ifstream para el archivo recién descargado
            ifstream archivoNuevo("../libros/" + titulo + ".txt");
            leer(titulo, archivoNuevo);
        } else {
            cout << "No se puede leer el libro sin descargarlo." << endl;
            return true;
        }
    } else {
        leer(titulo, archivo);
    }
    return true;
}

void leer(string titulo, ifstream& archivo){
    // Reiniciamos el puntero de lectura al principio del archivo
    archivo.clear(); // Limpiamos cualquier error o bandera de fin de archivo
    archivo.seekg(0); // Nos movemos al principio del archivo


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
        cout << "\nOpciones: 1. Continuar, 2. Retroceder, 3. Salir: ";
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
                cout << "Opcion invalida. Intentalo de nuevo." << endl;
        }
    } while (opcion != 3);
}


