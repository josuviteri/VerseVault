#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "../sqlite/sqlite3.h"


#define NEGRITA "\e[1m" // Renombramos el código de los caracteres en negrita para que sea mas entendible
#define QUITAR_NEGRITA "\e[m" // Renombramos el codigo de quitar la negrita a los caracteres para que sea mas entendible
//"Sin dolor no hay gloria" - Proverbio Romano 

void imprimirInicial(){
    char input[10];
    char sel;

    printf("\nProyecto Programacion IV | Grupo PVI-04\n\n");
    printf(NEGRITA"Bienvenido al Sistema de Libreria Virtual\nVerseVault\n\n" QUITAR_NEGRITA);

    do{
        printf("\nSelecciona una opcion: \n");
        printf("1. Iniciar Sesion \n2. Registrar cuenta \n3. Continuar como invitado \n4. Salir\n\n");

        fgets(input, sizeof(input), stdin);
        sscanf(input, " %c", &sel); 

        if(sel == '1'){
            
            iniciarSesion();

        }else if(sel == '2'){
            registrarCuenta();

        }else if(sel == '3'){
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
     sqlite3 *db;
    char *error_message = 0;
    int exit_code;

    // Abrimos la conexión a la base de datos
    exit_code = sqlite3_open(".\\libreria.db", &db);

    if (exit_code) {
        fprintf(stderr, "No se pudo abrir la base de datos: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Variables para almacenar los datos del nuevo cliente
    char nombre[100];
    char email[100];
    char fecha_nacimiento[100];
    int id_ciudad;

    // Solicitamos al usuario los datos para registrar la cuenta
    printf("Ingrese su nombre: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0'; // Eliminamos el carácter de nueva línea al final

    printf("Ingrese su correo electronico: ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = '\0'; // Eliminamos el carácter de nueva línea al final

    printf("Ingrese su fecha de nacimiento (AAAA-MM-DD): ");
    fgets(fecha_nacimiento, sizeof(fecha_nacimiento), stdin);
    fecha_nacimiento[strcspn(fecha_nacimiento, "\n")] = '\0'; // Eliminamos el carácter de nueva línea al final

    printf("Ingrese el ID de su ciudad: ");
    scanf("%d", &id_ciudad);
    getchar(); // Limpiamos el búfer de entrada después de scanf

    // Sentencia SQL para insertar un nuevo cliente con los datos proporcionados por el usuario
    char sql[200];
    sprintf(sql, "INSERT INTO Cliente (nom_cl, email_cl, fecha_n_cl, es_admin, id_ciudad) VALUES ('%s', '%s', '%s', 0, %d);", nombre, email, fecha_nacimiento, id_ciudad);

    exit_code = sqlite3_exec(db, sql, 0, 0, &error_message); // Ejecutamos la sentencia SQL

    if (exit_code != SQLITE_OK) {
        fprintf(stderr, "Error al insertar datos: %s\n", error_message);
        sqlite3_free(error_message);
    } else {
        printf("Registro de cuenta exitoso.\n");
    }

    sqlite3_close(db); // Cerramos la conexión a la base de datos
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
