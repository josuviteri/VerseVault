#ifndef _MENU_H_
#define _MENU_H_

#define MAX 20 //max passwd length

typedef struct User{
    char id;
    char passw[MAX];
} User;

//apartado de menus
void imprimirInicial();
void imprimirMenu();
void imprimirMenuInvitado();
void imprimirGestion();
void imprimirGestionInvitado();
//apartado gestion usuarios
void iniciarSesionMenu(char email_cl[], char pass_cl[]);
void registrarClienteMenu(char nom_cl[], char email_cl[], char pass_cl[]);

//apartado gestion de contenido
void agregarLibroMenu(char titulo[], char nom_autor[], char idioma[], char fecha_publicacion[]);
void eliminarLibroMenu(char titulo[]);
void aportarLibroMenu(char titulo[], char fecha_lec[]);
void guardarProgresoListaPersonal(int id_cliente, char titulo[], char fecha_lec[], int pag_actual);
void descargarLibro();

#endif