#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../sqlite/sqlite3.h"
#include "../structs/autor.h"
#include <time.h>

#define MAX_BUFFER_SIZE 1024

void errorMsg(char mensaje[]) {
    FILE* f;
    time_t rawtime;
    struct tm* timeinfo;
    char timestamp[80];

    // Obtener la hora actual
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Formatear la fecha y hora
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

    f = fopen("ficheros/db.log", "a");

    //fprintf(f, "%s\n", mensaje);
    fprintf(f, "[%s] %s\n", timestamp, mensaje);

    fclose(f);
}

void procesar_csv(const char* csvPath, sqlite3* db) {
    FILE* csvFile = fopen(csvPath, "r");
    if (!csvFile) {
        printf("No se pudo abrir el archivo CSV.\n");
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    // Leer la primera línea del archivo CSV (encabezado)
    if (fgets(buffer, MAX_BUFFER_SIZE, csvFile) != NULL) {
        // Leer y procesar el resto del archivo línea por línea
        while (fgets(buffer, MAX_BUFFER_SIZE, csvFile) != NULL) {
            // Eliminar los saltos de línea al final de la línea
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }

            // Tokenizar la línea por el delimitador '^p'
            char* id_libro = strtok(buffer, "@");
            char* Tipo = strtok(NULL, "@");
            char* fecha_publicacion = strtok(NULL, "@");
            char* Titulo = strtok(NULL, "@");
            char* Idioma = strtok(NULL, "@");
            char* Genero = strtok(NULL, "@");
            char* Autor = strtok(NULL, "@");

            // Insertar los datos en la base de datos SQLite
            char sql[MAX_BUFFER_SIZE];
            sprintf(sql, "INSERT INTO Datos (id_libro, Tipo, fecha_publicacion, Titulo, idioma, Genero, Autor) VALUES ('%d', '%s', '%s', '%s', '%s', '%s', '%s');",
                atoi(id_libro), Tipo, fecha_publicacion, Titulo, Idioma, Genero, Autor);
            
            char* errMsg = 0;
            int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "Error SQL: %s\n", errMsg);
                sqlite3_free(errMsg);
            }
        }
    }

    // Cerrar el archivo CSV
    fclose(csvFile);
}


int limpiar_tabla(sqlite3* db) {
    char* errMsg = 0;
    int rc = sqlite3_exec(db, "DELETE FROM Datos;", 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        errorMsg("Error al limpiar la tabla.");
        fprintf(stderr, "Error al eliminar SQL: %s\n", errMsg);
        sqlite3_free(errMsg);
        return 1;
    }

    printf("Tabla limpia exitosamente.\n");
    return 0;
}

int main() {
    const char* csvPath = "./ficheros/Libros_Data_Limpia.csv";

    sqlite3* db;
    int rc = sqlite3_open("libreria.db", &db);
    if (rc) {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    procesar_csv(csvPath, db);

    //limpiar_tabla(db);


    sqlite3_close(db);

    return 0;
}
