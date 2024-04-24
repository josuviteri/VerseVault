#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../sqlite/sqlite3.h"
#include "../structs/autor.h"
#include <time.h>

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

int insertar_autores_distintos(sqlite3* db) {
    // Consultar autores distintos de la tabla Datos
    const char* sql_select_autores = "SELECT DISTINCT Autor FROM Datos;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql_select_autores, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparando la consulta SQL: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Insertar autores en la tabla Autor
    const char* sql_insert_autor = "INSERT INTO Autor (nom_autor) VALUES (?);";
    rc = sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al iniciar la transacción: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 1;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* autor = (const char*)sqlite3_column_text(stmt, 0);

        sqlite3_stmt* insert_stmt;
        rc = sqlite3_prepare_v2(db, sql_insert_autor, -1, &insert_stmt, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error preparando la inserción: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        rc = sqlite3_bind_text(insert_stmt, 1, autor, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error al enlazar parámetro: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(insert_stmt);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        rc = sqlite3_step(insert_stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Error al insertar el autor: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(insert_stmt);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        sqlite3_finalize(insert_stmt);
    }

    rc = sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al comprometer la transacción: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_finalize(stmt);

    printf("Autores insertados exitosamente en la tabla Autor.\n");

    return 0;
}

int limpiar_tabla_Autor(sqlite3* db) {
    char* errMsg = 0;
    int rc = sqlite3_exec(db, "DELETE FROM Autor;", 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        errorMsg("Error al limpiar la tabla.");
        fprintf(stderr, "Error al eliminar SQL: %s\n", errMsg);
        sqlite3_free(errMsg);
        return 1;
    }

    printf("Tabla limpia exitosamente.\n");
    return 0;
}

int insertar_generos_distintos(sqlite3* db) {
    // Consultar géneros distintos de la tabla Datos
    const char* sql_select_generos = "SELECT DISTINCT Genero FROM Datos;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql_select_generos, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparando la consulta SQL: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Insertar géneros en la tabla Genero
    const char* sql_insert_genero = "INSERT INTO Genero (nom_genero) VALUES (?);";
    rc = sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al iniciar la transacción: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 1;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* genero = (const char*)sqlite3_column_text(stmt, 0);

        sqlite3_stmt* insert_stmt;
        rc = sqlite3_prepare_v2(db, sql_insert_genero, -1, &insert_stmt, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error preparando la inserción: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        rc = sqlite3_bind_text(insert_stmt, 1, genero, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error al enlazar parámetro: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(insert_stmt);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        rc = sqlite3_step(insert_stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Error al insertar el género: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(insert_stmt);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        sqlite3_finalize(insert_stmt);
    }

    rc = sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al comprometer la transacción: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_finalize(stmt);

    printf("Géneros insertados exitosamente en la tabla Genero.\n");

    return 0;
}


int insertar_datos_en_libro(sqlite3* db) {
    const char* sql_select_datos = "SELECT id_libro, Titulo, Idioma, Autor FROM Datos;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql_select_datos, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparando la consulta SQL: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Preparar la consulta para buscar el ID del autor
    const char* sql_select_autor_id = "SELECT id_autor FROM Autor WHERE nom_autor = ?;";
    sqlite3_stmt* stmt_autor_id;

    // Insertar datos en la tabla Libro
    const char* sql_insert_libro = "INSERT INTO Libro (id_libro, titulo, ruta, id_autor, idioma) VALUES (?, ?, '', ?, ?);";
    rc = sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al iniciar la transacción: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 1;
    }

    // Variables para el resultado de la consulta
    int id_libro;
    const char* titulo;
    const char* idioma;
    const char* autor;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Obtener los datos de la consulta
        id_libro = sqlite3_column_int(stmt, 0);
        titulo = (const char*)sqlite3_column_text(stmt, 1);
        idioma = (const char*)sqlite3_column_text(stmt, 2);
        autor = (const char*)sqlite3_column_text(stmt, 3);

        // Preparar la consulta para obtener el ID del autor
        rc = sqlite3_prepare_v2(db, sql_select_autor_id, -1, &stmt_autor_id, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error preparando la consulta SQL para obtener el ID del autor: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        // Enlazar el nombre del autor como parámetro
        rc = sqlite3_bind_text(stmt_autor_id, 1, autor, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error al enlazar parámetro: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt_autor_id);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        // Ejecutar la consulta para obtener el ID del autor
        rc = sqlite3_step(stmt_autor_id);
        if (rc != SQLITE_ROW) {
            fprintf(stderr, "No se encontró el autor en la tabla Autor: %s\n", autor);
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt_autor_id);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        // Obtener el ID del autor
        int id_autor = sqlite3_column_int(stmt_autor_id, 0);

        // Insertar el libro en la tabla Libro
        sqlite3_stmt* insert_stmt;
        rc = sqlite3_prepare_v2(db, sql_insert_libro, -1, &insert_stmt, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error preparando la inserción: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt_autor_id);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        // Enlazar los parámetros para la inserción
        rc = sqlite3_bind_int(insert_stmt, 1, id_libro);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error al enlazar parámetro 'id_libro': %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt_autor_id);
            sqlite3_finalize(insert_stmt);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        rc = sqlite3_bind_text(insert_stmt, 2, titulo, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error al enlazar parámetro 'titulo': %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt_autor_id);
            sqlite3_finalize(insert_stmt);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        rc = sqlite3_bind_int(insert_stmt, 3, id_autor);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error al enlazar parámetro 'id_autor': %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt_autor_id);
            sqlite3_finalize(insert_stmt);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        rc = sqlite3_bind_text(insert_stmt, 4, idioma, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error al enlazar parámetro 'idioma': %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt_autor_id);
            sqlite3_finalize(insert_stmt);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        // Ejecutar la inserción
        rc = sqlite3_step(insert_stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Error al insertar el libro: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt_autor_id);
            sqlite3_finalize(insert_stmt);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        sqlite3_finalize(insert_stmt);
        sqlite3_finalize(stmt_autor_id);
    }

    rc = sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al comprometer la transacción: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_finalize(stmt);

    printf("Datos insertados exitosamente en la tabla Libro.\n");

    return 0;
}

int insertar_fecha_publicacion_en_libro(sqlite3* db) {
    const char* sql_select_datos = "SELECT id_libro, fecha_publicacion FROM Datos;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql_select_datos, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparando la consulta SQL: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Insertar la fecha de publicación en la tabla Libro
    const char* sql_update_libro = "UPDATE Libro SET fecha_publicacion = ? WHERE id_libro = ?;";
    
    // Variables para el resultado de la consulta
    int id_libro;
    const char* fecha_publicacion;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Obtener los datos de la consulta
        id_libro = sqlite3_column_int(stmt, 0);
        fecha_publicacion = (const char*)sqlite3_column_text(stmt, 1);

        // Preparar la consulta de actualización
        sqlite3_stmt* update_stmt;
        rc = sqlite3_prepare_v2(db, sql_update_libro, -1, &update_stmt, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error preparando la consulta de actualización: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return 1;
        }

        // Enlazar los parámetros para la actualización
        rc = sqlite3_bind_text(update_stmt, 1, fecha_publicacion, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error al enlazar parámetro 'fecha_publicacion': %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(update_stmt);
            return 1;
        }

        rc = sqlite3_bind_int(update_stmt, 2, id_libro);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error al enlazar parámetro 'id_libro': %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(update_stmt);
            return 1;
        }

        // Ejecutar la actualización
        rc = sqlite3_step(update_stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Error al actualizar la fecha de publicación: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(update_stmt);
            return 1;
        }

        sqlite3_finalize(update_stmt);
    }

    sqlite3_finalize(stmt);

    printf("Fecha de publicación insertada exitosamente en la tabla Libro.\n");

    return 0;
}

int actualizarTablaGeneros(sqlite3* db) {
    const char* sql_select_datos = "SELECT id_libro, Genero, Titulo FROM Datos;";
    const char* sql_select_genero_id = "SELECT id_genero FROM Genero WHERE nom_genero = ?";
    const char* sql_select_libro_id = "SELECT id_libro FROM Libro WHERE titulo = ?";
    const char* sql_insert_generos = "INSERT INTO Generos (id_libro, id_genero) VALUES (?, ?);";

    sqlite3_stmt* stmt;
    sqlite3_stmt* stmt_genero_id;
    sqlite3_stmt* stmt_libro_id;
    int rc;

    // Preparar consulta para seleccionar datos de la tabla Datos
    rc = sqlite3_prepare_v2(db, sql_select_datos, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparando la consulta SQL: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Variables para el resultado de la consulta
    int id_libro;
    const char* genero;
    const char* titulo;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Obtener los datos de la consulta
        id_libro = sqlite3_column_int(stmt, 0);
        genero = (const char*)sqlite3_column_text(stmt, 1);
        titulo = (const char*)sqlite3_column_text(stmt, 2);

        // Preparar consulta para obtener el id_genero
        rc = sqlite3_prepare_v2(db, sql_select_genero_id, -1, &stmt_genero_id, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error preparando la consulta SQL para obtener el id_genero: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return 1;
        }

        // Enlazar el nombre del genero como parametro
        rc = sqlite3_bind_text(stmt_genero_id, 1, genero, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error al enlazar parametro: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt_genero_id);
            return 1;
        }

        // Ejecutar la consulta para obtener el id_genero
        rc = sqlite3_step(stmt_genero_id);
        if (rc != SQLITE_ROW) {
            fprintf(stderr, "No se encontro el genero en la tabla Genero: %s\n", genero);
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt_genero_id);
            return 1;
        }

        // Obtener el id_genero
        int id_genero = sqlite3_column_int(stmt_genero_id, 0);

        // Preparar consulta para obtener el id_libro
        rc = sqlite3_prepare_v2(db, sql_select_libro_id, -1, &stmt_libro_id, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error preparando la consulta SQL para obtener el id_libro: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt_genero_id);
            return 1;
        }

        // Enlazar el titulo del libro como parametro
        rc = sqlite3_bind_text(stmt_libro_id, 1, titulo, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error al enlazar parametro: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt_genero_id);
            sqlite3_finalize(stmt_libro_id);
            return 1;
        }

        // Ejecutar la consulta para obtener el id_libro
        rc = sqlite3_step(stmt_libro_id);
        if (rc != SQLITE_ROW) {
            fprintf(stderr, "No se encontro el libro en la tabla Libro: %s\n", titulo);
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt_genero_id);
            sqlite3_finalize(stmt_libro_id);
            return 1;
        }

        // Obtener el id_libro
        int id_libro_obtenido = sqlite3_column_int(stmt_libro_id, 0);

        // Insertar los datos en la tabla Generos
        sqlite3_stmt* insert_stmt;
        rc = sqlite3_prepare_v2(db, sql_insert_generos, -1, &insert_stmt, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error preparando la insercion: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt_genero_id);
            sqlite3_finalize(stmt_libro_id);
            return 1;
        }

        // Enlazar los parametros para la insercion
        rc = sqlite3_bind_int(insert_stmt, 1, id_libro_obtenido);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error al enlazar parametro 'id_libro': %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt_genero_id);
            sqlite3_finalize(stmt_libro_id);
            sqlite3_finalize(insert_stmt);
            return 1;
        }

        rc = sqlite3_bind_int(insert_stmt, 2, id_genero);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error al enlazar parametro 'id_genero': %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt_genero_id);
            sqlite3_finalize(stmt_libro_id);
            sqlite3_finalize(insert_stmt);
            return 1;
        }

        // Ejecutar la insercion
        rc = sqlite3_step(insert_stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Error al insertar en la tabla Generos: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt_genero_id);
            sqlite3_finalize(stmt_libro_id);
            sqlite3_finalize(insert_stmt);
            return 1;
        }

        sqlite3_finalize(insert_stmt);
        sqlite3_finalize(stmt_libro_id);
        sqlite3_finalize(stmt_genero_id);
    }

    sqlite3_finalize(stmt);

    printf("Datos insertados exitosamente en la tabla Generos.\n");

    return 0;
}



int main() {
    sqlite3* db;
    int rc = sqlite3_open("libreria.db", &db);
    if (rc) {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // rc = insertar_autores_distintos(db);
    // if (rc != 0) {
    //     fprintf(stderr, "Error al insertar autores en la tabla Autor.\n");
    // }

    // rc = insertar_generos_distintos(db);
    // if (rc != 0) {
    //     fprintf(stderr, "Error al insertar géneros en la tabla Genero.\n");
    // }

    // rc = insertar_datos_en_libro(db);
    // if (rc != 0) {
    //     fprintf(stderr, "Error al insertar datos en la tabla Libro.\n");
    // }

    // rc = insertar_fecha_publicacion_en_libro(db);
    // if (rc != 0) {
    //     fprintf(stderr, "Error al insertar fecha de publicación en la tabla Libro.\n");
    // }

    
    // rc = limpiar_tabla_Autor(db);
    // if (rc != 0) {
    //     fprintf(stderr, "Error al insertar géneros en la tabla Genero.\n");
    // }

     rc = actualizarTablaGeneros(db);
     if (rc != 0) {
         fprintf(stderr, "Error al insertar géneros en la tabla Genero.\n");
     }

    sqlite3_close(db);

    return 0;
}