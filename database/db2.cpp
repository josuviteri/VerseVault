#include <stdio.h>
#include <stdlib.h>
#include "db2.h"
#include <string>

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

    if (f != NULL) {
        fprintf(f, "[%s] %s\n", timestamp, mensaje);
        fclose(f);
    } else {
        printf("Error al abrir el archivo.\n");
    }
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




int iniciarSesion(sqlite3 *db, char email_cl[], char pass_cl[]) {
    sqlite3_stmt *stmt;
    char sql[] = "SELECT id_cl FROM Cliente WHERE email_cl = ? AND pass_cl = ?";
    int id_cliente = -1; // Inicializamos el ID del cliente con un valor negativo que indica un error
    
    // Preparar la consulta SQL
    int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error preparing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return -1;
    }

    // Enlazar los parámetros de la consulta SQL
    result = sqlite3_bind_text(stmt, 1, email_cl, strlen(email_cl), SQLITE_STATIC);
    if (result != SQLITE_OK) {
        printf("Error binding email_cl parameter\n");
        printf("%s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    result = sqlite3_bind_text(stmt, 2, pass_cl, strlen(pass_cl), SQLITE_STATIC);
    if (result != SQLITE_OK) {
        printf("Error binding pass_cl parameter\n");
        printf("%s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    // Ejecutar la consulta
    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        // Si se encontraron resultados, obtener el ID del cliente
        id_cliente = sqlite3_column_int(stmt, 0);
    } else {
        // Si no se encontraron resultados, las credenciales son incorrectas
        printf("Credenciales incorrectas\n");
    }

    sqlite3_finalize(stmt);
    return id_cliente;
}


int agregarLibro(sqlite3 *db, char titulo[], char nom_autor[], char idioma[], char fecha_publicacion[]){
    sqlite3_stmt *stmt;

     // Verificar si el libro ya existe en la base de datos
    char query[] = "SELECT COUNT(*) FROM Libro WHERE titulo = ?";
    int result = sqlite3_prepare_v2(db, query, strlen(query) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        errorMsg("Error preparing statement (SELECT)\n");
        printf("Error preparing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_bind_text(stmt, 1, titulo, strlen(titulo), SQLITE_STATIC);
    if (result != SQLITE_OK) {
        errorMsg("Error binding titulo parameter\n");
        printf("Error binding titulo parameter\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        if (count > 0) {
            // El titulo ya existe, no se puede registrar
            printf("Este libro ya esta registrada en el sistema\n");
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

    // Si el libro no existe, proceder con la inserción
    char sql[] = "INSERT INTO Libro (titulo, id_autor, idioma, fecha_publicacion) VALUES (?,?,?,?)";
    result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        errorMsg("Error preparing statement (INSERT)\n");
        printf("Error preparing statement (INSERT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("SQL query prepared (INSERT)\n");

    result = sqlite3_bind_text(stmt, 1, titulo, strlen(titulo), SQLITE_STATIC);
    if (result != SQLITE_OK) {
        errorMsg("Error binding nom_cl parameter\n");
        printf("Error binding nom_cl parameter\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    // Cambiamos el nombre por la id del autor
    char queryForId[] = "SELECT id_autor FROM AUTOR WHERE nom_autor = ?";
    sqlite3_stmt *stmtForId;
    int resultForId = sqlite3_prepare_v2(db, queryForId, strlen(queryForId) + 1, &stmtForId, NULL);
    if (resultForId != SQLITE_OK) {
        errorMsg("Error preparing statement (SELECT)\n");
        printf("Error preparing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return resultForId;
    }
    resultForId = sqlite3_bind_text(stmtForId, 1, nom_autor, strlen(nom_autor), SQLITE_STATIC);
    if (resultForId != SQLITE_OK) {
        errorMsg("Error binding nom_autor parameter\n");
        printf("Error binding nom_autor parameter\n");
        printf("%s\n", sqlite3_errmsg(db));
        return resultForId;
    }
    resultForId = sqlite3_step(stmtForId);
    int id_autor;
    if (resultForId == SQLITE_ROW) {
    // Obtener el ID del autor
        id_autor = sqlite3_column_int(stmtForId, 0);
    } else if (resultForId != SQLITE_DONE) {
    // Manejar el error
        errorMsg("Error obtaining the id\n");
        printf("Error obtaining the id\n");
    }

    // Liberamos la consulta SELECT
    resultForId = sqlite3_finalize(stmtForId);
    if (resultForId != SQLITE_OK) {
        errorMsg("Error finalizing statement (SELECT)\n");
        printf("Error finalizing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return resultForId;
    }


    result = sqlite3_bind_int(stmt, 2, id_autor);
    if (result != SQLITE_OK) {
        errorMsg("Error binding id_autor parameter\n");
        printf("Error binding id_autor parameter\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    
    result = sqlite3_bind_text(stmt, 3, idioma, strlen(idioma), SQLITE_STATIC);
    if (result != SQLITE_OK) {
        errorMsg("Error binding idioma parameter\n");
        printf("Error binding idioma parameter\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_bind_text(stmt, 4, fecha_publicacion, strlen(fecha_publicacion), SQLITE_STATIC);
    if (result != SQLITE_OK) {
        errorMsg("Error binding fecha_publicacion parameter\n");
        printf("Error binding fecha_publicacion parameter\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        errorMsg("Error inserting new data into LIBRO table\n");
        printf("Error inserting new data into LIBRO table\n");
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

int aportarLibro(sqlite3 *db, int id_cliente, char tituloLibro[], char fecha_lec[]){
    sqlite3_stmt *stmt;

    // Consulta SQL para seleccionar un libro disponible en la base de datos junto con el nombre del autor
    char query[] = "SELECT Libro.titulo, Autor.nom_autor, Libro.idioma, Libro.fecha_publicacion FROM Libro INNER JOIN Autor ON Libro.id_autor = Autor.id_autor WHERE Libro.titulo = ?";
    int result = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
    if (result != SQLITE_OK) {
        errorMsg("Error preparing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        printf("ERRRRROR\n");
        return result;
    }

    
    result = sqlite3_bind_text(stmt, 1, tituloLibro, strlen(tituloLibro), SQLITE_STATIC);
    if (result != SQLITE_OK) {
        errorMsg("Error binding title parameter\n");
        sqlite3_finalize(stmt);
        return result;
    }

    
    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        // Obtain the details of the selected book
        char *titulo_libro = (char *)sqlite3_column_text(stmt, 0);
        char *nom_autor = (char *)sqlite3_column_text(stmt, 1);
        char *idioma = (char *)sqlite3_column_text(stmt, 2);
        char *fecha_publicacion = (char *)sqlite3_column_text(stmt, 3);

        printf("Libro agregado a la lista personal\n");
        printf("Título: %s\n", titulo_libro);
        printf("Autor: %s\n", nom_autor);
        printf("Idioma: %s\n", idioma);
        printf("Fecha de publicación: %s\n", fecha_publicacion);
        
            
        guardarProgreso(db, id_cliente, tituloLibro, fecha_lec, 0);;
        
    } else {
        errorMsg("No se encontró un libro con el título proporcionado\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
        errorMsg("Error finalizing statement (SELECT)\n");
        printf("Error finalizing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }


    return SQLITE_OK;
}

int guardarProgreso(sqlite3 *db, int id_cliente, const char titulo[], const char fecha_lec[], int pag_actual) {
    sqlite3_stmt *stmt = NULL;
    int result;

    // Consulta SQL para obtener el id_libro
    char query_select[] = "SELECT id_libro FROM Libro WHERE titulo = ?";
    sqlite3_stmt *stmt_select = NULL;
    result = sqlite3_prepare_v2(db, query_select, strlen(query_select) + 1, &stmt_select, NULL);
    if (result != SQLITE_OK) {
        errorMsg("Error preparing select statement\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_bind_text(stmt_select, 1, titulo, -1, SQLITE_STATIC);
    if (result != SQLITE_OK) {
        errorMsg("Error binding titulo parameter for select\n");
        printf("%s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt_select);
        return result;
    }

    result = sqlite3_step(stmt_select);
    if (result == SQLITE_ROW) {
        int id_libro = sqlite3_column_int(stmt_select, 0);

        // Consulta SQL para insertar el progreso del libro en la tabla Progreso
        char query_insert[] = "INSERT INTO Progreso (id_cl, id_libro, fecha_lec, pag_actual) VALUES (?, ?, ?, ?)";
        result = sqlite3_prepare_v2(db, query_insert, strlen(query_insert) + 1, &stmt, NULL);
        if (result != SQLITE_OK) {
            errorMsg("Error preparing insert statement\n");
            printf("%s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt_select);
            return result;
        }

        result = sqlite3_bind_int(stmt, 1, id_cliente);
        if (result != SQLITE_OK) {
            errorMsg("Error binding id_cliente parameter for insert\n");
            printf("%s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt_select);
            sqlite3_finalize(stmt);
            return result;
        }

        result = sqlite3_bind_int(stmt, 2, id_libro);
        if (result != SQLITE_OK) {
            errorMsg("Error binding id_libro parameter for insert\n");
            printf("%s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt_select);
            sqlite3_finalize(stmt);
            return result;
        }

        result = sqlite3_bind_text(stmt, 3, fecha_lec, -1, SQLITE_STATIC);
        if (result != SQLITE_OK) {
            errorMsg("Error binding fecha_lec parameter for insert\n");
            printf("%s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt_select);
            sqlite3_finalize(stmt);
            return result;
        }

        result = sqlite3_bind_int(stmt, 4, pag_actual);
        if (result != SQLITE_OK) {
            errorMsg("Error binding pag_actual parameter for insert\n");
            printf("%s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt_select);
            sqlite3_finalize(stmt);
            return result;
        }

        // Ejecuta la consulta de inserción
        result = sqlite3_step(stmt);
        if (result != SQLITE_DONE) {
            errorMsg("Error inserting progreso del libro into Progreso table\n");
            printf("%s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt_select);
            sqlite3_finalize(stmt);
            return result;
        }

        printf("Progreso del libro insertado correctamente en la tabla Progreso\n");

        // Finaliza la declaración preparada
        sqlite3_finalize(stmt);
    } else {
        errorMsg("No se encontró un libro con el título proporcionado\n");
        printf("%s\n", sqlite3_errmsg(db));
    }

    // Finaliza la declaración preparada del select
    sqlite3_finalize(stmt_select);

    return SQLITE_OK;
}

int eliminarLibro(sqlite3 *db, char titulo[]){
    sqlite3_stmt *stmt = NULL;
    int result;

    // Consulta SQL para obtener el id_libro
    char query[] = "DELETE FROM Progreso WHERE id_libro IN (SELECT id_libro FROM Libro WHERE titulo = ?)";

    result = sqlite3_prepare_v2(db, query, strlen(query) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        errorMsg("Error preparing delete statement\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_bind_text(stmt, 1, titulo, strlen(titulo), SQLITE_STATIC);
        if (result != SQLITE_OK) {
        errorMsg("Error binding titulo parameter for delet\n");
        printf("%s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return result;
    }

    result = sqlite3_step(stmt);
        if (result != SQLITE_DONE) {
        errorMsg("Error inserting new data into LIBRO table\n");
        printf("Error inserting new data into LIBRO table\n");
        return result;
    }

    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
        errorMsg("Error finalizing statement (INSERT)\n");
        printf("Error finalizing statement (INSERT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    return SQLITE_OK;
}



// int main() {


//     sqlite3 *db;

//     int rc = sqlite3_open("libreria.db", &db);
//     if (rc != SQLITE_OK) {
//         errorMsg("Error opening database\n");
//         printf("Error opening database\n");
//         return rc;
//     }

//     printf("Database opened\n\n") ;

//     char nombre[] = "Josu";
//     char email[] = "j@email.com";
//     char pass[] = "contraseña123";


//     rc = registrarCliente(db, nombre, email, pass);
//     if (rc != SQLITE_OK) {
//         printf("Error inserting new data\n");
//         printf("%s\n", sqlite3_errmsg(db));
//         return rc;
//     }

//   //  char *nombre_func = iniciarSesion(db, email, pass); 
//   //  if (nombre_func != NULL) {
//   //      printf("Inicio de sesion exitoso. Bienvenido, %s\n", nombre_func);
//  //      free(nombre_func); // Liberar la memoria asignada
//   //  } else {
//       printf("Inicio de sesión fallido. Verifica tus credenciales.\n");
//   }


//     rc = showAllClientes(db);
//     if (rc != SQLITE_OK) {
//         printf("Error getting all clients\n");
//         printf("%s\n", sqlite3_errmsg(db));
//        return rc;
//     }


// //   rc = deleteAllClients(db);
// //   if (rc != SQLITE_OK) {
// //        printf("Error deleting all clients\n");
// //       printf("%s\n", sqlite3_errmsg(db));
// //       return rc;
// //  }



//     rc = sqlite3_close(db);
//     if (rc != SQLITE_OK) {
//         printf("Error opening database\n");
//         printf("%s\n", sqlite3_errmsg(db));
//         return rc;
//     }

//     printf("Database closed\n") ;

//     return 0;


// }
