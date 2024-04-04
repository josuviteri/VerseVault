#ifndef DB2_H
#define DB2_H

#include "../sqlite/sqlite3.h"

void errorMsg(char mensaje[]);
int showAllClientes(sqlite3 *db);
int deleteAllClients(sqlite3 *db);

int registrarCliente(sqlite3 *db, char nom_cl[], char email_cl[], char pass_cl[]);

char* iniciarSesion(sqlite3 *db, char email_cl[], char pass_cl[]);


#endif /* MENU_H */