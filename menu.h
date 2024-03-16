#ifndef _MENU_H_
#define _MENU_H_

#define MAX 20 //max passwd length

typedef struct {
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
void iniciarSesion();
void registrarCuenta();

//apartado gestion de contenido
void agregarLibro();
void eliminarLibro();
void aportarLibro();
void descargarLibro();

#endif