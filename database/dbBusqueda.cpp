#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../curly/descargaArchivos.h"
#include "../sqlite/sqlite3.h"

char* peticionTitulo(const char* titulo) {
    sqlite3* db;
    char query[200];
    sqlite3_stmt* stmt;
    int rc;
    char* resultados = NULL;
    int resultados_size = 0;

    rc = sqlite3_open("libreria.db", &db);
    if (rc) {
        fprintf(stderr, "Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    snprintf(query, sizeof(query), "SELECT * FROM Libro WHERE titulo LIKE '%%%s%%'", titulo);
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    resultados = (char*)malloc(1024);
    if (resultados == NULL) {
        fprintf(stderr, "Error al asignar memoria\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return NULL;
    }
    resultados[0] = '\0';

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "ID: %d, Titulo: %s\n", sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1));
        resultados_size += strlen(buffer) + 1;
        resultados = (char*)realloc(resultados, resultados_size);
        if (resultados == NULL) {
            fprintf(stderr, "Error al reasignar memoria\n");
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return NULL;
        }
        strcat(resultados, buffer);
    }

    if (strlen(resultados) == 0) {
        strcpy(resultados, "Libro no encontrado.\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return resultados;
}


char* peticionAutor(const char* autor) {
    sqlite3* db;
    char query[200];
    sqlite3_stmt* stmt;
    int rc;
    char* resultados = NULL;
    int resultados_size = 0;

    rc = sqlite3_open("libreria.db", &db);
    if (rc) {
        fprintf(stderr, "Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    snprintf(query, sizeof(query), "SELECT l.id_libro, l.titulo, a.nom_autor FROM Libro l INNER JOIN Autor a ON l.id_autor = a.id_autor WHERE a.nom_autor LIKE '%%%s%%'", autor);
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    resultados = (char*)malloc(1024);
    if (resultados == NULL) {
        fprintf(stderr, "Error al asignar memoria\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return NULL;
    }
    resultados[0] = '\0';

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "ID: %d, Titulo: %s, Autor: %s\n", sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2));
        resultados_size += strlen(buffer) + 1;
        resultados = (char*)realloc(resultados, resultados_size);
        if (resultados == NULL) {
            fprintf(stderr, "Error al reasignar memoria\n");
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return NULL;
        }
        strcat(resultados, buffer);
    }

    if (strlen(resultados) == 0) {
        strcpy(resultados, "No se encontraron libros para el autor proporcionado.\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return resultados;
}


int peticionAutorPorTitulo(char *titulo){
    sqlite3 *db;
    char query[200]; // Ajusta el tamaño según tus necesidades
    sqlite3_stmt *stmt;
    int rc;

    // Abrir la base de datos
    rc = sqlite3_open("libreria.db", &db); // Cambiado a "libreria.db"
    if (rc) {
        fprintf(stderr, "Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    // Construir la consulta SQL para buscar el libro por título
    snprintf(query, sizeof(query), "SELECT nom_autor FROM Autor WHERE id_autor = (SELECT id_autor FROM Libro WHERE titulo = '%s')", titulo);

    // Preparar la consulta SQL
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // Ejecutar la consulta SQL y manejar los resultados
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        char* nombreAutor;
        nombreAutor = (char *)sqlite3_column_text(stmt, 0);
        printf("%s\n",titulo);
        printf("%s\n",nombreAutor);
        descargaArchivos(titulo, nombreAutor); //Llamamos a la funcion para descargar
    } else {
        printf("\nLibro no encontrado.\n");
    }

    // Finalizar la consulta y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return rc;
}

int peticionIdLibroPorTitulo(const std::string& titulo) {
    sqlite3 *db;
    char query[200]; // Ajusta el tamaño según tus necesidades
    sqlite3_stmt *stmt;
    int rc;
    int idLibro = -1; // Valor predeterminado en caso de que no se encuentre el libro

    // Abrir la base de datos
    rc = sqlite3_open("libreria.db", &db);
    if (rc) {
        fprintf(stderr, "Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return idLibro;
    }

    // Construir la consulta SQL para buscar el ID del libro por título
    snprintf(query, sizeof(query), "SELECT id_libro FROM Libro WHERE titulo = '%s'", titulo.c_str());

    // Preparar la consulta SQL
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return idLibro;
    }

    // Ejecutar la consulta SQL y manejar los resultados
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        idLibro = sqlite3_column_int(stmt, 0);
    }

    // Finalizar la consulta y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return idLibro;
}



