#include <stdio.h>
#include "sqlite/sqlite3.h"
#include <string.h>

void errorMsg(char mensaje[]){
    FILE* f;


    f = fopen("log.txt", "a"); //log.log?

    fprintf(f, "%s\n", mensaje);

    fclose(f);
}


int showAllClientes(sqlite3 *db) {
    sqlite3_stmt *stmt;

    char sql[] = "select id_cl, nom_cl from Cliente";

    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) ;
    if (result != SQLITE_OK) {
        printf("Error preparing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("SQL query prepared (SELECT)\n");

    int id;
    char name[100];

    printf("\n");
    printf("\n");
    printf("Showing clients:\n");
    do {
        result = sqlite3_step(stmt) ;
        if (result == SQLITE_ROW) {
            id = sqlite3_column_int(stmt, 0);
            strcpy(name, (char *) sqlite3_column_text(stmt, 1));
            printf("ID: %d Name: %s\n", id, name);
        }
    } while (result == SQLITE_ROW);

    printf("\n");
    printf("\n");

    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
        printf("Error finalizing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("Prepared statement finalized (SELECT)\n");

    return SQLITE_OK;
}

int deleteAllClients(sqlite3 *db) {
    sqlite3_stmt *stmt;

    char sql[] = "delete from Cliente";

    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) ;
    if (result != SQLITE_OK) {
        printf("Error preparing statement (DELETE)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("SQL query prepared (DELETE)\n");

    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        printf("Error deleting data\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
        printf("Error finalizing statement (DELETE)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("Prepared statement finalized (DELETE)\n");

    return SQLITE_OK;
}

int insertNewCliente(sqlite3 *db, char name[]) {
    sqlite3_stmt *stmt;

    char sql[] = "insert into Cliente (id_cl, nom_cl, email_cl, contra_cl, fecha_n_cl, es_admin, id_ciudad) values (NULL, ?, NULL, NULL, NULL, 0, NULL)";
    int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
    if (result != SQLITE_OK) {
        printf("Error preparing statement (INSERT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("SQL query prepared (INSERT)\n");

    result = sqlite3_bind_text(stmt, 1, name, strlen(name), SQLITE_STATIC);
    if (result != SQLITE_OK) {
        printf("Error binding parameters\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        printf("Error inserting new data into CLIENTE table\n");
        return result;
    }

    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
        printf("Error finalizing statement (INSERT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("Prepared statement finalized (INSERT)\n");

    return SQLITE_OK;
}




int main() {








    sqlite3 *db;

    int result = sqlite3_open("libreria.sqlite", &db);
    if (result != SQLITE_OK) {
        printf("Error opening database\n");
        return result;
    }

    printf("Database opened\n\n") ;

    //delete funciona
    result = deleteAllClients(db);
    if (result != SQLITE_OK) {
        printf("Error deleting all clients\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }
    //al insertar no encuentra la columna id_cl
    result = insertNewCliente(db, "Josu");
    if (result != SQLITE_OK) {
        printf("Error inserting new data\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }




    result = showAllClientes(db);
    if (result != SQLITE_OK) {
        printf("Error getting all clients\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }


    result = sqlite3_close(db);
    if (result != SQLITE_OK) {
        printf("Error opening database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("Database closed\n") ;

    errorMsg("aaa");


    return 0;


}