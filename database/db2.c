#include <stdio.h>
#include "../sqlite/sqlite3.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>

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

    fprintf(f, "[%s] %s\n", timestamp, mensaje);

    fclose(f);
}

//funciona
int showAllClientes(sqlite3 *db) {
    sqlite3_stmt *stmt;

    char sql[] = "SELECT nom_cl, email_cl FROM Cliente";

    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        errorMsg("Error preparing statement (SELECT)\n");
        printf("Error preparing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("SQL query prepared (SELECT)\n");

    char name[100];
    char email[100];

    printf("\n");
    printf("Showing clients:\n");
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        strcpy(name, (char *)sqlite3_column_text(stmt, 0));
        strcpy(email, (char *)sqlite3_column_text(stmt, 1));
        printf("Name: %s Email: %s\n", name, email);
    }

    printf("\n");

    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
        errorMsg("Error finalizing statement (SELECT)\n");
        printf("Error finalizing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("Prepared statement finalized (SELECT)\n");

    return SQLITE_OK;
}




//funciona
int deleteAllClients(sqlite3 *db) {
    sqlite3_stmt *stmt;

    char sql[] = "delete from Cliente";

    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) ;
    if (result != SQLITE_OK) {
        errorMsg("Error preparing statement (DELETE)\n");
        printf("Error preparing statement (DELETE)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("SQL query prepared (DELETE)\n");

    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        errorMsg("Error deleting data\n");
        printf("Error deleting data\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
        errorMsg("Error finalizing statement (DELETE)\n");
        printf("Error finalizing statement (DELETE)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("Prepared statement finalized (DELETE)\n");

    return SQLITE_OK;
}



//funciona
int registrarCliente(sqlite3 *db, char nom_cl[], char email_cl[], char pass_cl[]) {
    sqlite3_stmt *stmt;

    // Verificar si el correo electrónico ya existe en la base de datos
    char query[] = "SELECT COUNT(*) FROM Cliente WHERE email_cl = ?";
    int result = sqlite3_prepare_v2(db, query, strlen(query) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        errorMsg("Error preparing statement (SELECT)\n");
        printf("Error preparing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_bind_text(stmt, 1, email_cl, strlen(email_cl), SQLITE_STATIC);
    if (result != SQLITE_OK) {
        errorMsg("Error binding email_cl parameter\n");
        printf("Error binding email_cl parameter\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        if (count > 0) {
            // El correo electrónico ya existe, no se puede registrar
            printf("Esta direccion de correo electronico ya esta registrada en el sistema\n");
            return SQLITE_ERROR;
        }
    } else {
        errorMsg("Error fetching data\n");
        printf("Error fetching data\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    // Liberar la consulta SELECT
    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
        errorMsg("Error finalizing statement (SELECT)\n");
        printf("Error finalizing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    // Si el correo electrónico no existe, proceder con la inserción
    char sql[] = "INSERT INTO Cliente (nom_cl, email_cl, pass_cl) VALUES (?,?,?)";
    result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        errorMsg("Error preparing statement (INSERT)\n");
        printf("Error preparing statement (INSERT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("SQL query prepared (INSERT)\n");

    result = sqlite3_bind_text(stmt, 1, nom_cl, strlen(nom_cl), SQLITE_STATIC);
    if (result != SQLITE_OK) {
        errorMsg("Error binding nom_cl parameter\n");
        printf("Error binding nom_cl parameter\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_bind_text(stmt, 2, email_cl, strlen(email_cl), SQLITE_STATIC);
    if (result != SQLITE_OK) {
        errorMsg("Error binding email_cl parameter\n");
        printf("Error binding email_cl parameter\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_bind_text(stmt, 3, pass_cl, strlen(pass_cl), SQLITE_STATIC);
    if (result != SQLITE_OK) {
        errorMsg("Error binding pass_cl parameter\n");
        printf("Error binding pass_cl parameter\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        errorMsg("Error inserting new data into CLIENTE table\n");
        printf("Error inserting new data into CLIENTE table\n");
        return result;
    }

    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
        errorMsg("Error finalizing statement (INSERT)\n");
        printf("Error finalizing statement (INSERT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("Prepared statement finalized (INSERT)\n");

    return SQLITE_OK;
}




char* iniciarSesion(sqlite3 *db, char email_cl[], char pass_cl[]) {
    sqlite3_stmt *stmt;
    char sql[] = "SELECT nom_cl FROM Cliente WHERE email_cl = ? AND pass_cl = ?";
    
    // Preparar la consulta SQL
    int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error preparing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return NULL;
    }

    // Enlazar los parámetros de la consulta SQL
    result = sqlite3_bind_text(stmt, 1, email_cl, strlen(email_cl), SQLITE_STATIC);
    if (result != SQLITE_OK) {
        printf("Error binding email_cl parameter\n");
        printf("%s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return NULL;
    }

    result = sqlite3_bind_text(stmt, 2, pass_cl, strlen(pass_cl), SQLITE_STATIC);
    if (result != SQLITE_OK) {
        printf("Error binding pass_cl parameter\n");
        printf("%s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return NULL;
    }

    // Ejecutar la consulta
    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        // Si se encontraron resultados, devolver el nombre del cliente
        const unsigned char *nombre = sqlite3_column_text(stmt, 0);
        char *nombre_cliente = strdup((const char*)nombre);
        sqlite3_finalize(stmt);
        return nombre_cliente;
    } else {
        // Si no se encontraron resultados, las credenciales son incorrectas
        printf("Credenciales incorrectas\n");
        sqlite3_finalize(stmt);
        return NULL;
    }
}




int main() {


    sqlite3 *db;

    int rc = sqlite3_open("libreria.db", &db);
    if (rc != SQLITE_OK) {
        errorMsg("Error opening database\n");
        printf("Error opening database\n");
        return rc;
    }

    printf("Database opened\n\n") ;

    char nombre[] = "Josu";
    char email[] = "j@email.com";
    char pass[] = "contraseña123";


    rc = registrarCliente(db, nombre, email, pass);
    if (rc != SQLITE_OK) {
        printf("Error inserting new data\n");
        printf("%s\n", sqlite3_errmsg(db));
        return rc;
    }

  //  char *nombre_func = iniciarSesion(db, email, pass); 
  //  if (nombre_func != NULL) {
  //      printf("Inicio de sesion exitoso. Bienvenido, %s\n", nombre_func);
 //      free(nombre_func); // Liberar la memoria asignada
  //  } else {
  //      printf("Inicio de sesión fallido. Verifica tus credenciales.\n");
  //  }


    rc = showAllClientes(db);
    if (rc != SQLITE_OK) {
        printf("Error getting all clients\n");
        printf("%s\n", sqlite3_errmsg(db));
       return rc;
    }


 //   rc = deleteAllClients(db);
 //   if (rc != SQLITE_OK) {
//        printf("Error deleting all clients\n");
 //       printf("%s\n", sqlite3_errmsg(db));
 //       return rc;
  //  }



    rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        printf("Error opening database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return rc;
    }

    printf("Database closed\n") ;

    return 0;


}