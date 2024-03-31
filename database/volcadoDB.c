#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../sqlite/sqlite3.h"
#include "../structs/autor.h"
#include <time.h>

void errorMsg(char mensaje[]){
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


// Función para cargar los autores desde un archivo CSV a una base de datos SQLite
int cargar_autores_desde_csv(sqlite3 *db, const char *nombre_archivo){
    // Abrir el archivo CSV
    FILE *file = fopen(nombre_archivo, "r");
    if (!file) {
        errorMsg("No se pudo abrir el archivo CSV\n");
        fprintf(stderr, "No se pudo abrir el archivo CSV\n");
        return 1;
    }

    // Preparar la consulta SQL para insertar autores
    const char *sql = "INSERT INTO autores (nom_autor) VALUES (?)";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        errorMsg("Error al preparar la consulta SQL\n");
        fprintf(stderr, "Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
        fclose(file);
        return 1;
    }

    // Leer y procesar cada línea del archivo CSV
    char line[1024];
    char *autor;
    char *token;
    const char delimiter[2] = ",";
    while (fgets(line, sizeof(line), file)) {
        // Utilizar strtok para dividir la línea en tokens
        token = strtok(line, delimiter);
        // Ignorar las primeras 6 columnas
        for (int i = 0; i < 6; ++i) {
            token = strtok(NULL, delimiter);
        }
        // Tomar la séptima columna como el nombre del autor
        autor = token;
        // Eliminar el carácter de nueva línea al final del autor
        autor[strcspn(autor, "\n")] = 0;

        // Insertar el autor en la base de datos
        rc = sqlite3_bind_text(stmt, 1, autor, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            errorMsg("Error al enlazar el nombre del autor\n");
            fprintf(stderr, "Error al enlazar el nombre del autor: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            fclose(file);
            return 1;
        }

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            errorMsg("Error al ejecutar la consulta SQL\n");
            fprintf(stderr, "Error al ejecutar la consulta SQL: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            fclose(file);
            return 1;
        }

        // Reiniciar la consulta para el próximo autor
        sqlite3_reset(stmt);
    }

    // Finalizar la consulta y cerrar el archivo
    sqlite3_finalize(stmt);
    fclose(file);

    return 0;
}


void selectTopAuthors(sqlite3 *db) {
    int rc;
    sqlite3_stmt *stmt;
    
    // Query para seleccionar los primeros 10 autores
    const char *sql_select_author = "SELECT id_autor, nom_autor FROM Autor LIMIT 10";

    // Preparar la sentencia SQL
    rc = sqlite3_prepare_v2(db, sql_select_author, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        errorMsg("Error preparando la consulta SQL\n");
        fprintf(stderr, "Error preparando la consulta SQL: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Ejecutar la consulta SQL y mostrar los resultados
    printf("ID_Autor  Nombre_Autor\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id_autor = sqlite3_column_int(stmt, 0);
        const unsigned char *nombre_autor = sqlite3_column_text(stmt, 1);
        printf("%-9d %s\n", id_autor, nombre_autor);
    }

    // Finalizar la consulta y liberar recursos
    sqlite3_finalize(stmt);
}
#define MAX_LINE_LENGTH 1024
#define MAX_AUTHORS 30

int insertAuthorsFromCSV(sqlite3 *db) {
    FILE *csv_file = fopen("ficheros/Libros_Data_Limpia.csv", "r");
    if (!csv_file) {
        errorMsg("Error al abrir el archivo CSV\n");
        fprintf(stderr, "Error al abrir el archivo CSV\n");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    char *sql_insert_author = "INSERT INTO Autor (nom_autor) VALUES (?)";
    sqlite3_stmt *stmt_insert_author;
    int rc = sqlite3_prepare_v2(db, sql_insert_author, -1, &stmt_insert_author, 0);
    if (rc != SQLITE_OK) {
        errorMsg("Error preparando la consulta SQL\n");
        fprintf(stderr, "Error preparando la consulta SQL: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    int author_count = 0;
    while (fgets(line, sizeof(line), csv_file) && author_count < MAX_AUTHORS) {
        char *author = strtok(line, ",");
        sqlite3_bind_text(stmt_insert_author, 1, author, -1, SQLITE_STATIC);
        rc = sqlite3_step(stmt_insert_author);
        if (rc != SQLITE_DONE) {
            errorMsg("Error insertando autor\n");
            fprintf(stderr, "Error insertando autor: %s\n", sqlite3_errmsg(db));
        } else {
            printf("Autor insertado correctamente: %s\n", author);
        }
        sqlite3_reset(stmt_insert_author);
        author_count++;
    }

    fclose(csv_file);
    sqlite3_finalize(stmt_insert_author);
    return SQLITE_OK;
}
// void insertAuthorsFromCSV(sqlite3 *db, const char *csv_file_path) {
//     int rc;
//     FILE *csv_file = fopen(csv_file_path, "r");
//     if (!csv_file) {
//         fprintf(stderr, "Error al abrir el archivo CSV\n");
//         return;
//     }

//     // Ignorar la primera línea si contiene encabezados
//     char line[1024];
//     fgets(line, sizeof(line), csv_file);

//     // Query para insertar autores
//     const char *sql_insert_author = "INSERT INTO Autor (nom_autor) VALUES (?)";

//     // Preparar la sentencia SQL
//     sqlite3_stmt *stmt_insert_author;
//     rc = sqlite3_prepare_v2(db, sql_insert_author, -1, &stmt_insert_author, 0);
//     if (rc != SQLITE_OK) {
//         fprintf(stderr, "Error preparando la consulta SQL: %s\n", sqlite3_errmsg(db));
//         fclose(csv_file);
//         return;
//     }

//     // Leer y procesar cada línea del archivo CSV
//     while (fgets(line, sizeof(line), csv_file)) {
//         char *autor = strtok(line, ",");
//         // Insertar autor en la base de datos
//         sqlite3_bind_text(stmt_insert_author, 1, autor, -1, SQLITE_STATIC);
//         rc = sqlite3_step(stmt_insert_author);
//         if (rc != SQLITE_DONE) {
//             fprintf(stderr, "Error insertando autor: %s\n", sqlite3_errmsg(db));
//         } else {
//             fprintf(stderr, "Autor insertado correctamente: %s\n", autor);
//         }
//         // Reiniciar la sentencia preparada para el siguiente autor
//         sqlite3_reset(stmt_insert_author);
//     }

//     // Liberar recursos
//     fclose(csv_file);
//     sqlite3_finalize(stmt_insert_author);
// }

int deleteAllAuthors(sqlite3 *db) {
    int rc;
    char *sql = "DELETE FROM Autor";

    // Preparar la sentencia SQL
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        errorMsg("Error preparando la consulta SQL\n");
        fprintf(stderr, "Error preparando la consulta SQL: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // Ejecutar la consulta SQL
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        errorMsg("Error eliminando datos de la tabla Autor\n");
        fprintf(stderr, "Error eliminando datos de la tabla Autor: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Datos eliminados correctamente de la tabla Autor\n");
    }

    // Finalizar la consulta y liberar recursos
    sqlite3_finalize(stmt);
    return rc;
}


// Función para cargar los autores desde un archivo CSV a una base de datos SQLite
int cargar_10autores_desde_csv(sqlite3 *db, const char *nombre_archivo){
    // Abrir el archivo CSV
    FILE *file = fopen(nombre_archivo, "r");
    if (!file) {
        errorMsg("No se pudo abrir el archivo CSV\n");
        fprintf(stderr, "No se pudo abrir el archivo CSV\n");
        return 1;
    }

    // Descartar la primera línea que contiene los encabezados
    char line[1024];
    if (!fgets(line, sizeof(line), file)) {
        errorMsg("No se pudo leer la primera línea del archivo CSV\n");
        fprintf(stderr, "No se pudo leer la primera línea del archivo CSV\n");
        fclose(file);
        return 1;
    }

    // Preparar la consulta SQL para insertar autores
    const char *sql = "INSERT INTO Autor (nom_autor) VALUES (?)";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        errorMsg("Error al preparar la consulta SQL");
        fprintf(stderr, "Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
        fclose(file);
        return 1;
    }

    // Leer y procesar cada línea del archivo CSV
    char *autor;
    char *token;
    const char delimiter[3] = "^p"; // Cambiado el delimitador
    int count = 0; // Contador de autores insertados
    while (fgets(line, sizeof(line), file)) {
        // Verificar si se ha alcanzado el límite de 10 autores
        if (count >= 10) {
            break;
        }

        // Copiar la línea para evitar modificar la original
        char line_copy[1024];
        strcpy(line_copy, line);

        // Utilizar strtok para dividir la línea en tokens
        token = strtok(line_copy, delimiter);
        // Ignorar las primeras 6 columnas
        for (int i = 0; i < 6; ++i) {
            token = strtok(NULL, delimiter);
        }
        // Tomar la séptima columna como el nombre del autor
        autor = token;
        // Eliminar el carácter de nueva línea al final del autor
        autor[strcspn(autor, "\n")] = 0;

        // Insertar el autor en la base de datos
        rc = sqlite3_bind_text(stmt, 1, autor, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            errorMsg("Error al enlazar el nombre del autor\n");
            fprintf(stderr, "Error al enlazar el nombre del autor: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            fclose(file);
            return 1;
        }

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            errorMsg("Error al ejecutar la consulta SQL\n");
            fprintf(stderr, "Error al ejecutar la consulta SQL: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            fclose(file);
            return 1;
        }

        // Incrementar el contador de autores
        count++;

        // Reiniciar la consulta para el próximo autor
        sqlite3_reset(stmt);
    }

    // Finalizar la consulta y cerrar el archivo
    sqlite3_finalize(stmt);
    fclose(file);

    return 0;
}
int main(int argc, char* argv[]) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    // Abrir la base de datos
    rc = sqlite3_open("libreria.db", &db);
    if(rc) {
        errorMsg("Error, No se puede abrir la base de datos\n");
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 0;
    } else {
        fprintf(stderr, "Base de datos abierta exitosamente\n");
    }

    // Insertar autores desde el archivo CSV
    //insertAuthorsFromCSV(db, "./ficheros/Libros_Data_Limpia.csv");
    //insertAuthorsFromCSV(db);

    // // Seleccionar y mostrar los primeros 10 autores
    // printf("\nLos primeros 10 autores son:\n");
    // selectTopAuthors(db);

    // Borrar todos los datos de la tabla Autor
     //rc = deleteAllAuthors(db);
     //if (rc != SQLITE_OK) {
     //   errorMsg("Error al eliminar datos de la tabla Autor\n");
     //   fprintf(stderr, "Error al eliminar datos de la tabla Autor\n");
     //}

    if (cargar_10autores_desde_csv(db, "./ficheros/Libros_Data_Limpia.csv") != 0) {
        errorMsg("Error al cargar los autores desde el archivo CSV\n");
        fprintf(stderr, "Error al cargar los autores desde el archivo CSV\n");
        sqlite3_close(db);
        return 1;
    }

    printf("\nLos primeros 10 autores son:\n");
     selectTopAuthors(db);
    
    // Cerrar la base de datos
    sqlite3_close(db);
    return 0;
}
