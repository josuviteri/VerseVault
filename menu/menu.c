#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "../sqlite/sqlite3.h"
#include "../database/db2.h"


#define NEGRITA "\e[1m" // Renombramos el código de los caracteres en negrita para que sea mas entendible
#define QUITAR_NEGRITA "\e[m" // Renombramos el codigo de quitar la negrita a los caracteres para que sea mas entendible
//"Sin dolor no hay gloria" - Proverbio Romano 

void imprimirInicial(){
    char input[10];
    char nombre[31];
    char email[101];
    char pass[31];
    char sel;

    printf("\nProyecto Programacion IV | Grupo PVI-04\n\n");
    printf(NEGRITA"Bienvenido al Sistema de Libreria Virtual\nVerseVault\n\n" QUITAR_NEGRITA);

    do{
        printf("\nSelecciona una opcion: \n");
        printf("1. Iniciar Sesion \n2. Registrar cuenta \n3. Continuar como invitado \n4. Salir\n\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel); 

        if(sel == '1'){
            printf("\nIntroduce tus datos para iniciar sesion:\nIntroduce tu correo electronico:\n");
            fgets(email, sizeof(email), stdin);

            printf("\nIntroduce tu contrasenya:\n");
            fgets(pass, sizeof(pass), stdin);

            iniciarSesionMenu(email,pass);

        }else if(sel == '2'){
            printf("\nIntroduce tus datos para registrarte:\nIntroduce tu nombre:\n(30 caracteres como maximo)\n");
            fgets(nombre, sizeof(nombre), stdin);

            printf("\nIntroduce tu correo electronico:\n");
            fgets(email, sizeof(email), stdin);

            printf("\nIntroduce tu contrasenya:\n(30 caracteres como maximo)\n");
            fgets(pass, sizeof(pass), stdin);


            registrarClienteMenu(nombre,email,pass);

        }else if(sel == '3'){
            imprimirMenuInvitado();
        }else if(sel == '4'){
            printf("\ncerrando...\n");
        }else{
            printf("\nIntroduce un valor valido\n\n");
    }

    }while(sel != '4' && sel != '3' && sel != '2' && sel != '1');
}

void imprimirMenu(){
    char input[10];
    char sel;

    printf(NEGRITA"Menu Principal\n\n" QUITAR_NEGRITA);


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
        printf("1. Buscar Libro\n2. Aportar Libro\n3. Volver\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel); 

        if(sel == '1'){
            printf("\ncorrecto 1\n\n");
            // buscarLibro();

        }else if(sel == '2'){
            printf("\ncorrecto 2\n\n");
            aportarLibro();

        }else if(sel == '3'){
            printf("\nvolviendo...\n\n");
            imprimirMenu();
        }
        else{
            printf("\nIntroduce un valor valido\n\n");
    }

    }while(sel != '3' && sel != '2' && sel != '1');

}

void imprimirGestionInvitado(){
    char input[10];
    char sel;

    printf(NEGRITA"Menu de Gestion de Contenido\nSesion de invitado\n\n" QUITAR_NEGRITA);


    do{
        printf("\nSelecciona una opcion: \n");
        printf("1. Buscar Libro\n2. Aportar Libro\n3. Volver\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel); 

        if(sel == '1'){
            printf("\ncorrecto 1\n\n");
            // buscarLibro();
        }else if(sel == '2'){
            printf("\ncorrecto 2\n\n");
            aportarLibro();
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
void iniciarSesionMenu(char email_cl[], char pass_cl[]){
    sqlite3 *db;
    int rc = sqlite3_open("libreria.db", &db);
    if (rc != SQLITE_OK) {
        errorMsg("Error opening database\n");
        printf("Error opening database\n");
        return;
    }

    char *nombre_cliente = iniciarSesion(db, email_cl, pass_cl);
    if (nombre_cliente != NULL) {
        printf("Inicio de sesion exitoso. Bienvenido, %s\n", nombre_cliente);
        imprimirMenu();
        free(nombre_cliente); // Liberar la memoria asignada
    } else {
        printf("Inicio de sesion fallido. Verifica tus credenciales.\n");
    }

    rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        printf("Error opening database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }
}




void registrarClienteMenu(char nom_cl[], char email_cl[], char pass_cl[]){
    sqlite3 *db;
    int rc = sqlite3_open("libreria.db", &db);
    if (rc != SQLITE_OK) {
        errorMsg("Error opening database\n");
        printf("Error opening database\n");
        return;
    }

    rc = registrarCliente(db, nom_cl, email_cl, pass_cl);
    if (rc != SQLITE_OK) {
        printf("Error inserting new data\n");
        printf("%s\n", sqlite3_errmsg(db));
    } else {
        printf("Cliente registrado exitosamente\n");
    }

    rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        printf("Error closing database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }
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