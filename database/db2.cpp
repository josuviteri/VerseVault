#include <stdio.h>
#include <stdlib.h>
#include "db2.h"
#include <string>
#include <iostream>
#include "../structs/libro.h"

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

void commandLog(char mensaje[]) {
    FILE* f;
    time_t rawtime;
    struct tm* timeinfo;
    char timestamp[80];

    // Obtener la hora actual
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Formatear la fecha y hora
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

    f = fopen("../sockets2/ServerWithProtocol/server.log", "a");

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

    int es_admin = 0;

    // Verificar si el correo electrónico termina con "@opendeusto.es"
    if (endsWith(email_cl, "@opendeusto.es") || endsWith(email_cl, "@deusto.es")) {
        es_admin = 1;
    }

    // Si el correo electrónico no existe, proceder con la inserción
    char sql[] = "INSERT INTO Cliente (nom_cl, email_cl, es_admin, pass_cl) VALUES (?,?,?,?)";
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
    result = sqlite3_bind_int(stmt, 3, es_admin);
    if (result != SQLITE_OK) {
        errorMsg("Error binding pass_cl parameter\n");
        printf("Error binding pass_cl parameter\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_bind_text(stmt, 4, pass_cl, strlen(pass_cl), SQLITE_STATIC);
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

bool endsWith(const std::string &str, const std::string &suffix) {
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

int esAdmin(const char* email) {
    sqlite3* db;
    char query[200];
    sqlite3_stmt* stmt;
    int rc;
    int isAdmin = 0; // Inicialmente suponemos que el cliente no es admin

    rc = sqlite3_open("libreria.db", &db);
    if (rc) {
        fprintf(stderr, "Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    snprintf(query, sizeof(query), "SELECT es_admin FROM Cliente WHERE email_cl ='%s'", email);

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        isAdmin = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return isAdmin;
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


int agregarLibro(sqlite3 *db, char titulo[], char nom_autor[], char idioma[], char fecha_publicacion[]) {
    sqlite3_stmt *stmt;

    // Verificar si el autor existe en la base de datos
    char queryForId[] = "SELECT id_autor FROM Autor WHERE nom_autor = ?";
    int resultForId = sqlite3_prepare_v2(db, queryForId, strlen(queryForId) + 1, &stmt, NULL);
    if (resultForId != SQLITE_OK) {
        errorMsg("Error preparing statement (SELECT)\n");
        printf("Error preparing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return resultForId;
    }
    resultForId = sqlite3_bind_text(stmt, 1, nom_autor, strlen(nom_autor), SQLITE_STATIC);
    if (resultForId != SQLITE_OK) {
        errorMsg("Error binding nom_autor parameter\n");
        printf("Error binding nom_autor parameter\n");
        printf("%s\n", sqlite3_errmsg(db));
        return resultForId;
    }
    resultForId = sqlite3_step(stmt);
    int id_autor;
    if (resultForId == SQLITE_ROW) {
        // Obtener el ID del autor
        id_autor = sqlite3_column_int(stmt, 0);
    } else if (resultForId == SQLITE_DONE) {
        // El autor no existe, insertarlo en la tabla de autores
        char insertAuthorQuery[] = "INSERT INTO Autor (nom_autor) VALUES (?)";
        sqlite3_stmt *stmtInsertAuthor;
        int resultInsertAuthor = sqlite3_prepare_v2(db, insertAuthorQuery, strlen(insertAuthorQuery) + 1, &stmtInsertAuthor, NULL);
        if (resultInsertAuthor != SQLITE_OK) {
            errorMsg("Error preparing statement (INSERT)\n");
            printf("Error preparing statement (INSERT)\n");
            printf("%s\n", sqlite3_errmsg(db));
            return resultInsertAuthor;
        }
        resultInsertAuthor = sqlite3_bind_text(stmtInsertAuthor, 1, nom_autor, strlen(nom_autor), SQLITE_STATIC);
        if (resultInsertAuthor != SQLITE_OK) {
            errorMsg("Error binding nom_autor parameter\n");
            printf("Error binding nom_autor parameter\n");
            printf("%s\n", sqlite3_errmsg(db));
            return resultInsertAuthor;
        }
        resultInsertAuthor = sqlite3_step(stmtInsertAuthor);
        if (resultInsertAuthor != SQLITE_DONE) {
            errorMsg("Error inserting new data into Autor table\n");
            printf("Error inserting new data into Autor table\n");
            return resultInsertAuthor;
        }
        id_autor = sqlite3_last_insert_rowid(db); // Obtener el ID del autor insertado
        sqlite3_finalize(stmtInsertAuthor);
    } else {
        // Manejar el error
        errorMsg("Error obtaining the id\n");
        printf("Error obtaining the id\n");
        return resultForId;
    }
    sqlite3_finalize(stmt);

    // Insertar el libro con el ID del autor obtenido
    char insertBookQuery[] = "INSERT INTO Libro (titulo, id_autor, idioma, fecha_publicacion) VALUES (?,?,?,?)";
    int result = sqlite3_prepare_v2(db, insertBookQuery, strlen(insertBookQuery) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        errorMsg("Error preparing statement (INSERT)\n");
        printf("Error preparing statement (INSERT)\n");
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
        errorMsg("Error inserting new data into Libro table\n");
        printf("Error inserting new data into Libro table\n");
        return result;
    }
    sqlite3_finalize(stmt);

    return SQLITE_OK;
}


int agregarLibroMiLista(sqlite3 *db, int id_cliente, char tituloLibro[], char fecha_lec[]){
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
        printf("Titulo: %s\n", titulo_libro);
        printf("Autor: %s\n", nom_autor);
        printf("Idioma: %s\n", idioma);
        printf("Fecha de publicacion: %s\n", fecha_publicacion);
        
            
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

int eliminarLibro(sqlite3 *db, int idCliente,char titulo[]){
    sqlite3_stmt *stmt = NULL;
    int result;

    // Consulta SQL para obtener el id_libro
    char query[] = "DELETE FROM Progreso WHERE id_libro IN (SELECT id_libro FROM Libro WHERE titulo = ?) AND id_cl = ?";

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
    result = sqlite3_bind_int(stmt, 2, idCliente);
    if (result != SQLITE_OK) {
        errorMsg("Error binding client parameter for delet\n");
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
int eliminarLibroBD1(sqlite3 *db, char titulo[]){
    sqlite3_stmt *stmt = NULL;
    int result;

    // Consulta SQL para obtener el id_libro
    char query[] = "DELETE FROM Libro WHERE titulo = ?";

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
        errorMsg("Error deleting data from LIBRO table\n");
        printf("Error deleting data from LIBRO table\n");
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
char* mostrarMiLista(int id_cliente_actual) {
    sqlite3 *db;
    char query[400];
    sqlite3_stmt *stmt;
    char* resultados = NULL;
    int resultado;
    int resultados_size = 0;

    // Abrir la base de datos
    resultado = sqlite3_open("libreria.db", &db);
    if (resultado) {
        fprintf(stderr, "Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    // Construir la consulta SQL para buscar los libros del cliente actual en la tabla Progreso
    snprintf(query, sizeof(query), "SELECT id_libro, fecha_lec, pag_actual FROM Progreso WHERE id_cl = %d", id_cliente_actual);

    // Preparar la consulta SQL
    resultado = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (resultado != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    // Inicializar la cadena de resultados
    resultados = (char*)malloc(1024);
    if (resultados == NULL) {
        fprintf(stderr, "Error al asignar memoria\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return NULL;
    }
    resultados[0] = '\0';

    // Ejecutar la consulta SQL y manejar los resultados
    int contador_libros = 0;
    while ((resultado = sqlite3_step(stmt)) == SQLITE_ROW) {
        contador_libros++;
        int id_libro = sqlite3_column_int(stmt, 0); // ID del libro
        const char* fecha_lectura = (const char*)sqlite3_column_text(stmt, 1); // Fecha de lectura
        int pagina_actual = sqlite3_column_int(stmt, 2); // Página actual

        // Realizar una segunda consulta para obtener el título del libro
        char titulo_query[300];
        snprintf(titulo_query, sizeof(titulo_query), "SELECT titulo FROM Libro WHERE id_libro = %d", id_libro);
        sqlite3_stmt *titulo_stmt;
        int titulo_resultado = sqlite3_prepare_v2(db, titulo_query, -1, &titulo_stmt, NULL);
        if (titulo_resultado == SQLITE_OK && sqlite3_step(titulo_stmt) == SQLITE_ROW) {
            const char *titulo = (const char*)sqlite3_column_text(titulo_stmt, 0);

            char buffer[256];
            snprintf(buffer, sizeof(buffer), "\t%d. Titulo: %s, Fecha Lectura: %s, Pagina Actual: %d\n", contador_libros, titulo, fecha_lectura, pagina_actual);
            resultados_size += strlen(buffer) + 1;
            resultados = (char*)realloc(resultados, resultados_size);
            if (resultados == NULL) {
                fprintf(stderr, "Error al reasignar memoria\n");
                sqlite3_finalize(titulo_stmt);
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return NULL;
            }
            strcat(resultados, buffer);
        }

        // Finalizar la consulta del título del libro
        sqlite3_finalize(titulo_stmt);
    }

    // Verificar si no se encontraron libros
    if (contador_libros == 0) {
        strcpy(resultados, "No se encontraron libros en tu lista.\n");
    }

    // Verificar si hubo un error al ejecutar la consulta
    if (resultado != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta SQL: %s\n", sqlite3_errmsg(db));
    }

    // Finalizar la consulta y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return resultados;
}

int cargarProgreso(int id_libro) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    int pagina_actual = 0;

    // Abrir la base de datos
    rc = sqlite3_open("libreria.db", &db);
    if (rc) {
        std::cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 0; // Si hay un error, devuelve 0
    }

    // Construir la consulta SQL para obtener el progreso del libro
    std::string query = "SELECT pag_actual FROM Progreso WHERE id_libro = " + std::to_string(id_libro);

    // Preparar la consulta SQL
    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Error al preparar la consulta SQL: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 0; // Si hay un error, devuelve 0
    }

    // Ejecutar la consulta SQL y obtener el progreso
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        pagina_actual = sqlite3_column_int(stmt, 0);
    }

    // Finalizar la consulta y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return pagina_actual;
}

void actualizarProgreso(int id_libro, int pagina_actual, char time[]) {
    sqlite3 *db;
    char *error_message = nullptr;
    int rc;

    // Abrir la base de datos
    rc = sqlite3_open("libreria.db", &db);
    if (rc) {
        std::cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // Construir la consulta SQL para actualizar la página actual del libro
    std::string query = "UPDATE Progreso SET pag_actual = " + std::to_string(pagina_actual) + ", fecha_lec = '" + std::string(time) + "' WHERE id_libro = " + std::to_string(id_libro);

    // Ejecutar la consulta SQL
    rc = sqlite3_exec(db, query.c_str(), nullptr, 0, &error_message);
    if (rc != SQLITE_OK) {
        std::cerr << "Error al ejecutar la consulta SQL: " << error_message << std::endl;
        sqlite3_free(error_message);
    } else {
        std::cout << "Progreso actualizado correctamente." << std::endl;
    }

    // Cerrar la base de datos
    sqlite3_close(db);
}

char* mostrarRecomendaciones() {
    sqlite3 *db;
    char query[200]; // Ajusta el tamaño según tus necesidades
    sqlite3_stmt *stmt;
    int resultado;
    char* recomendaciones = NULL;
    int recomendaciones_size = 0;

    // Abrir la base de datos
    resultado = sqlite3_open("libreria.db", &db);
    if (resultado) {
        fprintf(stderr, "Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    // Construir la consulta SQL utilizando un JOIN para combinar los datos de las tablas Libro y Autor
    snprintf(query, sizeof(query), "SELECT Libro.titulo, Autor.nom_autor FROM Libro INNER JOIN Autor ON Libro.id_autor = Autor.id_autor ORDER BY random() LIMIT 5");

    // Preparar la consulta SQL
    resultado = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (resultado != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    // Inicializar la cadena de resultados
    recomendaciones = (char*)malloc(1024);
    if (recomendaciones == NULL) {
        fprintf(stderr, "Error al asignar memoria\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return NULL;
    }
    recomendaciones[0] = '\0';

    // Ejecutar la consulta SQL y manejar los resultados
    int contador_libros = 0;
    while ((resultado = sqlite3_step(stmt)) == SQLITE_ROW) {
        contador_libros++;
        const unsigned char *titulo = sqlite3_column_text(stmt, 0); // Título del libro
        const unsigned char *nombre_autor = sqlite3_column_text(stmt, 1); // Nombre del autor

        char buffer[256];
        snprintf(buffer, sizeof(buffer), "\t%d. Titulo: %s  Autor: %s\n", contador_libros, titulo, nombre_autor);
        recomendaciones_size += strlen(buffer) + 1;
        recomendaciones = (char*)realloc(recomendaciones, recomendaciones_size);
        if (recomendaciones == NULL) {
            fprintf(stderr, "Error al reasignar memoria\n");
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return NULL;
        }
        strcat(recomendaciones, buffer);
    }

    // Verificar si hubo un error al ejecutar la consulta
    if (resultado != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta SQL: %s\n", sqlite3_errmsg(db));
    }

    // Finalizar la consulta y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Verificar si no se encontraron libros
    if (contador_libros == 0) {
        strcpy(recomendaciones, "No se encontraron recomendaciones.\n");
    }

    return recomendaciones;
}

