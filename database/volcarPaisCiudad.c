#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../sqlite/sqlite3.h"

typedef struct Pais {
    char nombre[100];
} Pais;

void insertar_pais_en_db(sqlite3 *db, const Pais *pais) {
    char *error_message = 0;
    char sql_query[200]; 

    // Construir la consulta SQL para insertar el país
    sprintf(sql_query, "INSERT INTO Pais (nom_pais) VALUES ('%s')", pais->nombre);

    // Ejecutar la consulta
    int result = sqlite3_exec(db, sql_query, 0, 0, &error_message);
    if (result != SQLITE_OK) {
        printf("Error al insertar el país: %s\n", error_message);
        sqlite3_free(error_message);
    }
}

void leer_paises_desde_csv(const char *nombre_archivo, sqlite3 *db) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombre_archivo);
        return;
    }

    char linea[200];
    while (fgets(linea, sizeof(linea), archivo)) {
        // Eliminar el caracter de nueva línea al final de la línea si existe
        if (linea[strlen(linea) - 1] == '\n')
            linea[strlen(linea) - 1] = '\0';
        
        Pais pais;
        sscanf(linea, "%[^;];", pais.nombre);
        insertar_pais_en_db(db, &pais);
    }

    fclose(archivo);
}


typedef struct Ciudad {
    char nombre[100];
    int id_pais; // Clave foránea para el país
} Ciudad;

void insertar_ciudad_en_db(sqlite3 *db, const Ciudad *ciudad) {
    char *error_message = 0;
    char sql_query[200]; 

    // Obtener el id del país España
    int id_espana;
    char *sql_query_pais = "SELECT id_pais FROM Pais WHERE nom_pais = 'Espana'";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql_query_pais, -1, &stmt, NULL);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        id_espana = sqlite3_column_int(stmt, 0);
    } else {
        printf("Error: no se encontro el país Espana en la base de datos.\n");
        return;
    }
    sqlite3_finalize(stmt);

    // Construir la consulta SQL para insertar la ciudad
    sprintf(sql_query, "INSERT INTO Ciudad (nom_ciudad, id_pais) VALUES ('%s', %d)", ciudad->nombre, id_espana);

    // Ejecutar la consulta
    int result = sqlite3_exec(db, sql_query, 0, 0, &error_message);
    if (result != SQLITE_OK) {
        printf("Error al insertar la ciudad: %s\n", error_message);
        sqlite3_free(error_message);
    }
}
void leer_ciudades_desde_csv(const char *nombre_archivo, sqlite3 *db) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombre_archivo);
        return;
    }

    char linea[200];
    // Saltar la primera fila (encabezado)
    fgets(linea, sizeof(linea), archivo);

    while (fgets(linea, sizeof(linea), archivo)) {
        // Eliminar el caracter de nueva línea al final de la línea si existe
        if (linea[strlen(linea) - 1] == '\n')
            linea[strlen(linea) - 1] = '\0';
        
        Ciudad ciudad;
        sscanf(linea, "%[^;];", ciudad.nombre);
        insertar_ciudad_en_db(db, &ciudad);
    }

    fclose(archivo);
}

int main() {
    const char* csvPath = "./ficheros/Paises_Limpios.csv";

    sqlite3 *db;
    int result = sqlite3_open("libreria.db", &db);
    if (result) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    //leer_paises_desde_csv(csvPath, db);

    leer_ciudades_desde_csv("./ficheros/Municipios_Limpios.csv", db);


    sqlite3_close(db);

    return 0;
}