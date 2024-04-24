#include <stdio.h>
#include "../sqlite/sqlite3.h"
#include <string.h>



int main(int argc, char* argv[]) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    // Abrir la base de datos
    rc = sqlite3_open("libreria.db", &db);
    if(rc) {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return(0);
    } else {
        fprintf(stderr, "Base de datos abierta exitosamente\n");
    }

    // Query para insertar un nuevo usuario
    char *sql = "INSERT INTO Cliente (id_cl, nom_cl, email_cl, contra_cl, fecha_n_cl, es_admin, id_ciudad) VALUES (?, ?, ?, ?, ?, ?, ?)";
    
    // Ejemplo de datos para un nuevo usuario
    int id_cl = 1;
    char *nom_cl = "Usuario1";
    char *email_cl = "usuario1@example.com";
    char *contra_cl = "contraseña";
    char *fecha_n_cl = "2024-03-14";
    int es_admin = 0;
    int id_ciudad = 1; // ID de la ciudad a la que pertenece el usuario
    
    // Preparar la sentencia SQL
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error preparando la consulta SQL: %s\n", sqlite3_errmsg(db));
        return(0);
    }
    
    // Bindear los parámetros a la sentencia SQL
    sqlite3_bind_int(stmt, 1, id_cl);
    sqlite3_bind_text(stmt, 2, nom_cl, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email_cl, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, contra_cl, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, fecha_n_cl, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, es_admin);
    sqlite3_bind_int(stmt, 7, id_ciudad);
    
    // Ejecutar la sentencia SQL
    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE) {
        fprintf(stderr, "Error ejecutando la consulta SQL: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stderr, "Nuevo usuario insertado correctamente\n");
    }

    // Liberar la sentencia preparada
    sqlite3_finalize(stmt);

    // Cerrar la base de datos
    sqlite3_close(db);
    return 0;
}
