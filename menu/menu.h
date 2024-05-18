#ifndef _MENU_H_
#define _MENU_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#define MAX 20 //max passwd length

typedef struct User{
    char id;
    char passw[MAX];
} User;


#ifdef __cplusplus
extern "C" {
#endif
//apartado de menus
void imprimirInicial();
void imprimirMenu();
void imprimirMenuInvitado();
void imprimirGestion();
void imprimirGestionInvitado();
//apartado gestion usuarios
int iniciarSesionMenu(char email_cl[], char pass_cl[]);
int registrarClienteMenu(char nom_cl[], char email_cl[], char pass_cl[]);

void menuMiLista();

//apartado gestion de contenido
int agregarLibroMenu(char titulo[], char nom_autor[], char idioma[], char fecha_publicacion[]);
int eliminarLibroMenu(int id_cliente, char titulo[]);
int aportarLibroMenu(int id_cliente, char titulo[], char fecha_lec[]);
int eliminarLibroBD(char titulo[]);
void guardarProgresoListaPersonal(int id_cliente, char titulo[], char fecha_lec[], int pag_actual);
int descargarLibro(char *titulo);
//void buscarLibro();

void imprimirLineas(const std::vector<std::string>& lines, int start);
bool CheckleerLibro(std::string titulo);
void leer(std::string titulo, std::ifstream& archivo); //Usamos referencia para pasar archivo

void limpiarBuffer();

#ifdef __cplusplus
}
#endif
#endif