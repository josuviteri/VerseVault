#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "../sqlite/sqlite3.h"
#include "../database/db2.c"


#define NEGRITA "\e[1m" // Renombramos el código de los caracteres en negrita para que sea mas entendible
#define QUITAR_NEGRITA "\e[m" // Renombramos el codigo de quitar la negrita a los caracteres para que sea mas entendible
//"Sin dolor no hay gloria" - Proverbio Romano 
int id_cliente_actual = -1;
time_t rawtime;
struct tm* timeinfo;
char actualTime[80];

void imprimirInicial(){
    char input[10];
    char nombre[31];
    char email[101];
    char pass[31];
    char sel;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(actualTime, sizeof(actualTime), "%Y-%m-%d", timeinfo);

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
    char titulo[30];
    char nom_autor[30];
    char idioma[10];
    char fecha_publicacion[10];
    char fecha_actual[10];

    printf(NEGRITA"Menu de Gestion de Contenido\nUsuario: \n\n" QUITAR_NEGRITA);


    do{
        printf("\nSelecciona una opcion: \n");
        printf("1. Agregar Libro\n2. Eliminar Libro\n3. Aportar Libro\n4. Descargar Libro\n5. Volver\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel); 

        if(sel == '1'){
            printf("\ncorrecto 1\n\n");
            printf("\nIntroduce los datos del libro:\nIntroduce el nombre del libro:\n(30 caracteres como maximo)\n");
            fgets(titulo, sizeof(titulo), stdin);
            strtok(titulo, "\n"); // Elimina el carácter

            printf("\nIntroduce el nombre del autor:\n");
            fgets(nom_autor, sizeof(nom_autor), stdin);
            strtok(nom_autor, "\n"); // Elimina el carácter
            printf(nom_autor);

            printf("\nIntroduce el idioma de una manera reducida:\n(ejemplo: es, en...)\n");
            fgets(idioma, sizeof(idioma), stdin);
            strtok(idioma, "\n"); // Elimina el carácter

            printf("\nIntroduce la fecha de publicacion del libro: \n(formato: aaaa-mm-dd)\n");
            fgets(fecha_publicacion, sizeof(fecha_publicacion), stdin);
            strtok(fecha_publicacion, "\n"); // Elimina el carácter

            agregarLibroMenu(titulo, nom_autor, idioma, fecha_publicacion);

        }else if(sel == '2'){
            printf("\ncorrecto 2\n\n");
            eliminarLibro();

        }else if(sel == '3'){
            printf("\ncorrecto 3\n\n");
            printf("\nIntroduce el titulo del libro que desea guardar en tu lista: \n");
            fgets(titulo, sizeof(titulo), stdin);
            strtok(titulo, "\n"); // Elimina el carácter
            aportarLibroMenu(titulo, actualTime);

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
    char titulo[30];
    char fecha_actual[30];

    printf(NEGRITA"Menu de Gestion de Contenido\nSesion de invitado\n\n" QUITAR_NEGRITA);


    do{
        printf("\nSelecciona una opcion: \n");
        printf("1. Aportar Libro\n2. Descargar Libro\n3. Volver\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel); 

        if(sel == '1'){
            printf("\ncorrecto 1\n\n");
            printf("\nIntroduce el titulo del libro que desea guardar en tu lista: \n");
            fgets(titulo, sizeof(titulo), stdin);
            strtok(titulo, "\n"); // Elimina el carácter
            aportarLibroMenu(titulo, actualTime);

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
void iniciarSesionMenu(char email_cl[], char pass_cl[]){
    sqlite3 *db;
    int rc = sqlite3_open("libreria.db", &db);
    if (rc != SQLITE_OK) {
        errorMsg("Error opening database\n");
        printf("Error opening database\n");
        return;
    }

    id_cliente_actual = iniciarSesion(db, email_cl, pass_cl);
    if (id_cliente_actual != -1) {
        printf("Inicio de sesion exitoso. Bienvenido\n");
        imprimirMenu();
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
void agregarLibroMenu(char titulo[], char nom_autor[], char idioma[], char fecha_publicacion[]){
    //debria contectarse y agregar un libro a la db
    sqlite3 *db;
    int rl = sqlite3_open("libreria.db", &db);
    if (rl != SQLITE_OK) {
        errorMsg("Error opening database\n");
        printf("Error opening database\n");
        return;
    }
    rl = agregarLibro(db, titulo, nom_autor, idioma, fecha_publicacion);
    if (rl != SQLITE_OK) {
        printf("Error inserting new data\n");
        printf("%s\n", sqlite3_errmsg(db));
    } else {
        printf("Libro agregado exitosamente\n");
    }

    rl = sqlite3_close(db);
    if (rl != SQLITE_OK) {
        printf("Error closing database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }
}
void eliminarLibro(){
    //debria contectarse a la db y eliminar un libro de la lista personal    
}
void aportarLibroMenu(char titulo[], char fecha_lec[]){
    //debria contectarse y agregar un libro de los disponibles en la db a la lista personal
    sqlite3 *db;
        int al = sqlite3_open("libreria.db", &db);
    if (al != SQLITE_OK) {
        errorMsg("Error opening database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }

    // Intenta agregar el libro
    al = aportarLibro(db, id_cliente_actual ,titulo, fecha_lec);
    
    if (al != SQLITE_OK) {
        printf("Error inserting new data lo siento\n");
        printf("%s\n", sqlite3_errmsg(db));
    } else {
        printf("Libro agregado exitosamente\n");
    }

    // Cierra la base de datos
    al = sqlite3_close(db);
    if (al != SQLITE_OK) {
        printf("Error closing database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }
} 
void guardarProgresoListaPersonal(int id_cliente, char titulo[], char fecha_lec[], int pag_actual) {
    sqlite3 *db;
    int al = sqlite3_open("libreria.db", &db);
    if (al != SQLITE_OK) {
        errorMsg("Error opening database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }

    al = guardarProgreso(db, id_cliente, titulo, fecha_lec, 0);
    if (al != SQLITE_OK) {
        printf("Error inserting new data que pena\n");
        printf("%s\n", sqlite3_errmsg(db));
    } else {
        printf("Progreso del libro guardado exitosamente\n");
    }

    al = sqlite3_close(db);
    if (al != SQLITE_OK) {
        printf("Error closing database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }

    return;
}
void descargarLibro(){
    //debria contectarse a la db y descargar un libro en formato txt, o el formato seleccionado    
}