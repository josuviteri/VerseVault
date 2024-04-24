#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../sqlite/sqlite3.h"

typedef struct Libreria {
    char nom_libreria[100];
    char ciudad_libreria[100];
    char direccion_libreria[100];
    char horario_libreria[100];
} Libreria;

void insertar_libreria_en_db(sqlite3 *db, const Libreria *libreria) {
    char *error_message = 0;
    char sql_query[500]; 

    // Construir la consulta SQL para insertar la librería
    sprintf(sql_query, "INSERT INTO Libreria (nom_libre, ciudad_libre, dir_libre, horario_libre) VALUES ('%s', '%s', '%s', '%s')",
            libreria->nom_libreria, libreria->ciudad_libreria, libreria->direccion_libreria, libreria->horario_libreria);

    // Ejecutar la consulta
    int result = sqlite3_exec(db, sql_query, 0, 0, &error_message);
    if (result != SQLITE_OK) {
        printf("Error al insertar la librería: %s\n", error_message);
        sqlite3_free(error_message);
    }
}

void leer_librerias_desde_csv(const char *nombre_archivo, sqlite3 *db) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombre_archivo);
        return;
    }

    char linea[200];
    while (fgets(linea, sizeof(linea), archivo)) {
        Libreria libreria;
        sscanf(linea, "%[^;];%[^;];%[^;];%[^;];", 
               libreria.nom_libreria,
               libreria.ciudad_libreria,
               libreria.direccion_libreria,
               libreria.horario_libreria);
        insertar_libreria_en_db(db, &libreria);
    }

    fclose(archivo);
}

int main() {
    const char* csvPath = "./ficheros/librerias-1.csv";

    sqlite3 *db;
    int result = sqlite3_open("libreria.db", &db);
    if (result) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    leer_librerias_desde_csv(csvPath, db);

    sqlite3_close(db);

    return 0;
}
