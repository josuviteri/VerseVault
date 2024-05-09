#ifndef DB2_H
#define DB2_H

#include "../sqlite/sqlite3.h"
#include <string.h>
#include <time.h>
#include <string>

void errorMsg(char mensaje[]);
int showAllClientes(sqlite3 *db);
int deleteAllClients(sqlite3 *db);

int registrarCliente(sqlite3 *db, char nom_cl[], char email_cl[], char pass_cl[]);

int iniciarSesion(sqlite3 *db, char email_cl[], char pass_cl[]);

int agregarLibro(sqlite3 *db, char titulo[], char nom_autor[], char idioma[], char fecha_publicacion[]);
int aportarLibro(sqlite3 *db, int id_cliente, char titulo[], char fecha_lec[]);
int guardarProgreso(sqlite3 *db, int id_cliente, const char titulo[], const char fecha_lec[], int pag_actual);

int eliminarLibro(sqlite3 *db, char titulo[]);

void mostrarMiLista(int id_cliente_actual);

#endif /* MENU_H */