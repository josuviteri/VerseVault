#include <stdlib.h>
#include <stdio.h>
#include "menu.h"

#define NEGRITA "\e[1m" // Renombramos el código de los caracteres en negrita para que sea mas entendible
#define QUITAR_NEGRITA "\e[m" // Renombramos el codigo de quitar la negrita a los caracteres para que sea mas entendible


void imprimirInicial(){
    char input[10];
    char sel;

    printf("Proyecto Programacion IV | Grupo PVI-04\n");
    printf(NEGRITA"Bienvenido al Sistema de Libreria Virtual\n\n" QUITAR_NEGRITA);

    do{
        printf("\nSelecciona una opcion: \n");
        printf("1. Iniciar Sesion \n2. Registrar cuenta \n3. Continuar como invitado \n4. Salir\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel); 

        if(sel == '1'){
            printf("\ncorrecto 1\n\n");
            iniciarSesion();

        }else if(sel == '2'){
            printf("\ncorrecto 2\n\n");
            registrarCuenta();

        }else if(sel == '3'){
            printf("\ncorrecto 3\n\n");
        
        }else if(sel == '4'){
            printf("\ncerrando...");
        }else{
            printf("\nIntroduce un valor valido\n\n");
    }

    }while(sel != '4');
}

void iniciarSesion(){

}
void registrarCuenta(){

}

//apartado gestion de contenido
void guardarLibro(){

}
void cargarLibro(){
    
}