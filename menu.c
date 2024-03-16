#include <stdlib.h>
#include <stdio.h>
#include "menu.h"

#define NEGRITA "\e[1m" // Renombramos el código de los caracteres en negrita para que sea mas entendible
#define QUITAR_NEGRITA "\e[m" // Renombramos el codigo de quitar la negrita a los caracteres para que sea mas entendible


void imprimirInicial(){
    char input[10];
    char sel;

    printf("\nProyecto Programacion IV | Grupo PVI-04\n");
    printf(NEGRITA"Bienvenido al Sistema de Libreria Virtual\n\n" QUITAR_NEGRITA);

    do{
        printf("\nSelecciona una opcion: \n");
        printf("1. Iniciar Sesion \n2. Registrar cuenta \n3. Continuar como invitado \n4. Salir\n\n");

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
            imprimirMenuInvitado();
        }else if(sel == '4'){
            printf("\ncerrando...");
        }else{
            printf("\nIntroduce un valor valido\n\n");
    }

    }while(sel != '4' && sel != '3' && sel != '2' && sel != '1');
}

void imprimirMenu(){
    char input[10];
    char sel;

    printf(NEGRITA"Menu Principal\nUsuario: \n\n" QUITAR_NEGRITA);


    do{
        printf("\nSelecciona una opcion: \n");
        printf("1. Seleccionar libro de mi lista\n2. Gestionar Contenido\n3. Cerrar Sesion\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel); 

        if(sel == '1'){
            printf("\ncorrecto 1\n\n");
            //cargar los libros de ese usuario en la db y mostrarlos

        }else if(sel == '2'){
            printf("\ncorrecto 2\n\n");
            imprimirGestion();

        }else if(sel == '3'){
            printf("\ncerrando sesion...\n\n");
            imprimirInicial();
        }else{
            printf("\nIntroduce un valor valido\n\n");
    }

    }while(sel != '3' && sel != '2' && sel != '1');


}

void imprimirMenuInvitado(){
    char input[10];
    char sel;

    printf(NEGRITA"Menu Principal\nSesion de invitado\n\n" QUITAR_NEGRITA);


    do{
        printf("\nSelecciona una opcion: \n");
        printf("1. Seleccionar libro\n2. Gestionar Contenido\n3. Cerrar Sesion\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel); 

        if(sel == '1'){
            printf("\ncorrecto 1\n\n");
            //cargar los libros de ese usuario en la db y mostrarlos

        }else if(sel == '2'){
            printf("\ncorrecto 2\n\n");
            imprimirGestionInvitado();

        }else if(sel == '3'){
            printf("\ncerrando sesion...\n\n");
            imprimirInicial();
        }else{
            printf("\nIntroduce un valor valido\n\n");
    }

    }while(sel != '3' && sel != '2' && sel != '1');


}

void imprimirGestion(){
    char input[10];
    char sel;

    printf(NEGRITA"Menu de Gestion de Contenido\nUsuario: \n\n" QUITAR_NEGRITA);


    do{
        printf("\nSelecciona una opcion: \n");
        printf("1. Agregar Libro\n2. Eliminar Libro\n3. Aportar Libro\n4. Descargar Libro\n5. Volver\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel); 

        if(sel == '1'){
            printf("\ncorrecto 1\n\n");
            agregarLibro();

        }else if(sel == '2'){
            printf("\ncorrecto 2\n\n");
            eliminarLibro();

        }else if(sel == '3'){
            printf("\ncorrecto 3\n\n");
            aportarLibro();
        }else if(sel == '4'){
            printf("\ncorrecto 4\n\n");
            descargarLibro();
        }else if(sel == '5'){
            printf("\nvolviendo...\n\n");
            imprimirMenu();
        }
        else{
            printf("\nIntroduce un valor valido\n\n");
    }

    }while(sel != '4' && sel != '3' && sel != '2' && sel != '1' && sel != '5');

}

void imprimirGestionInvitado(){
    char input[10];
    char sel;

    printf(NEGRITA"Menu de Gestion de Contenido\nSesion de invitado\n\n" QUITAR_NEGRITA);


    do{
        printf("\nSelecciona una opcion: \n");
        printf("1. Aportar Libro\n2. Descargar Libro\n3. Volver\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel); 

        if(sel == '1'){
            printf("\ncorrecto 1\n\n");
            aportarLibro();
        }else if(sel == '2'){
            printf("\ncorrecto 2\n\n");
            descargarLibro();
        }else if(sel == '3'){
            printf("\nvolviendo...\n\n");
            imprimirMenuInvitado();  
        }
        else{
            printf("\nIntroduce un valor valido\n\n");
    }

    }while(sel != '3' && sel != '2' && sel != '1');
  
}



//apartado gestion usuarios
void iniciarSesion(){ //char usuario, char contraseña
    //deberia conectarse a la db y hacer una query con los datos
    imprimirMenu();
}
void registrarCuenta(){
    //deberia conectarse a la db y registrar los datos 
}

//apartado gestion de contenido de la db
void agregarLibro(){
    //debria contectarse y agregar un libro de los disponibles en la db a la lista personal    
}
void eliminarLibro(){
    //debria contectarse a la db y eliminar un libro de la lista personal    
}
void aportarLibro(){
    //debria contectarse y agregar un libro a la db
} 
void descargarLibro(){
    //debria contectarse a la db y descargar un libro en formato txt, o el formato seleccionado    
}