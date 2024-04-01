#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../sqlite/sqlite3.h"
#include "../structs/libreria.h" 

// Función para insertar datos de una librería en la base de datos
void insertar_libreria_en_db(sqlite3 *db, Libreria libreria) {
    char *error_message = 0;
    char sql_query[500]; 

    // Construir la consulta SQL para insertar la librería
    sprintf(sql_query, "INSERT INTO Libreria (nom_libre, ciudad_libre, dir_libre, horario_libre) VALUES ('%s', '%s', '%s', '%s')",
            libreria.nom_libreria, libreria.ciudad_libreria, libreria.direccion_libreria, libreria.horario_libreria);

    // Ejecutar la consulta
    int result = sqlite3_exec(db, sql_query, 0, 0, &error_message);
    if (result != SQLITE_OK) {
        printf("Error al insertar la librería: %s\n", error_message);
        sqlite3_free(error_message);
    }
}

// Función para insertar datos de librerías desde un archivo CSV en la base de datos
void insertar_librerias_desde_csv_en_db(const char *nombre_archivo, sqlite3 *db) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombre_archivo);
        return;
    }

    char linea[200]; // Asumiendo que cada línea tiene una longitud máxima de 200 caracteres

    while (fgets(linea, sizeof(linea), archivo)) {
        Libreria libreria;

        sscanf(linea, "%[^;];%[^;];%[^;];%[^;];", 
               libreria.nom_libreria,
               libreria.ciudad_libreria,
               libreria.direccion_libreria,
               libreria.horario_libreria);

        // Insertar la librería en la base de datos
        insertar_libreria_en_db(db, libreria);
    }

    fclose(archivo);
}

int main() {
    const char* csv2Path = "./ficheros/librerias-1.csv";
    sqlite3 *db;
    int result = sqlite3_open("libreria.db", &db);
    if (result) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 0;
    }


    // Insertar los datos de las librerías desde el archivo CSV en la base de datos
    insertar_librerias_desde_csv_en_db("csv2Path", db);

    // Cerrar la conexión con la base de datos
    sqlite3_close(db);

    return 0;
}
