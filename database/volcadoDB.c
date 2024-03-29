#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../sqlite/sqlite3.h"
#include "../structs/autor.h"

void selectTopAuthors(sqlite3 *db) {
    int rc;
    sqlite3_stmt *stmt;
    
    // Query para seleccionar los primeros 10 autores
    const char *sql_select_author = "SELECT id_autor, nom_autor FROM Autor LIMIT 10";

    // Preparar la sentencia SQL
    rc = sqlite3_prepare_v2(db, sql_select_author, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
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
        fprintf(stderr, "Error al abrir el archivo CSV\n");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    char *sql_insert_author = "INSERT INTO Autor (nom_autor) VALUES (?)";
    sqlite3_stmt *stmt_insert_author;
    int rc = sqlite3_prepare_v2(db, sql_insert_author, -1, &stmt_insert_author, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparando la consulta SQL: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    int author_count = 0;
    while (fgets(line, sizeof(line), csv_file) && author_count < MAX_AUTHORS) {
        char *author = strtok(line, ",");
        sqlite3_bind_text(stmt_insert_author, 1, author, -1, SQLITE_STATIC);
        rc = sqlite3_step(stmt_insert_author);
        if (rc != SQLITE_DONE) {
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
        fprintf(stderr, "Error preparando la consulta SQL: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // Ejecutar la consulta SQL
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error eliminando datos de la tabla Autor: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Datos eliminados correctamente de la tabla Autor\n");
    }

    // Finalizar la consulta y liberar recursos
    sqlite3_finalize(stmt);
    return rc;
}

int main(int argc, char* argv[]) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    // Abrir la base de datos
    rc = sqlite3_open("libreria.db", &db);
    if(rc) {
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
    // rc = deleteAllAuthors(db);
    // if (rc != SQLITE_OK) {
    //     fprintf(stderr, "Error al eliminar datos de la tabla Autor\n");
    // }

    printf("\nLos primeros 10 autores son:\n");
     selectTopAuthors(db);
    
    // Cerrar la base de datos
    sqlite3_close(db);
    return 0;
}
