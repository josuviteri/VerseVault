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

//"Sin dolor no hay gloria" - Proverbio Romano



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
int agregarLibroMenu(char titulo[], char nom_autor[], char idioma[], char fecha_publicacion[]){
    //debria contectarse y agregar un libro a la db
    sqlite3 *db;
    int rl = sqlite3_open("libreria.db", &db);
    if (rl != SQLITE_OK) {
        //////errorMsgg("Error opening database\n");
        printf("Error opening database\n");
        return -1;
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
        return -1;
    }
    return rl;
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

int eliminarLibroBD(char titulo[]){
    //debria contectarse a la db y eliminar un libro de la lista personal
    sqlite3 *db;
    int al = sqlite3_open("libreria.db", &db);
    if (al != SQLITE_OK) {
        //////errorMsgg("Error opening database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return -1;
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
        return -1;
    }
    return al;
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
int descargarLibro(char *titulo){
// Verificar si el archivo del libro ya existe en la carpeta
    ifstream archivo("../libros/" + string(titulo) + ".txt");
    if (archivo.good()) {
        cout << "El libro '" << titulo << "' ya esta descargado." << endl;
        return SQLITE_OK; // Si el libro ya está en la carpeta, salir de la función
    }
    int status = peticionAutorPorTitulo(titulo);
    return status;
}

/*void buscarLibro(){
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
}*/


void imprimirLineas(const vector<string>& lines, int start) {
    int end = min(start + convPag, static_cast<int>(lines.size()));
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
            //leer(titulo, archivoNuevo);
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
                //actualizarProgreso(id_titulo,inicio/convPag, actualTime); // Guardar progreso antes de salir
                cout << "Saliendo del programa." << endl;
                break;
            default:
                cout << "Opcion invalida. Intentalo de nuevo." << endl;
        }
    } while (opcion != 3);
}


