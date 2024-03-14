#ifndef _MENU_H_
#define _MENU_H_

#define MAX 20 //max passwd length

typedef struct {
    char id;
    char passw[MAX];
} User;

//apartado gestion usuarios
void iniciarSesion();
void registrarCuenta();

//apartado gestion de contenido
void guardarLibro(); //subir al servidor o guardar en local?
void cargarLibro();



#endif