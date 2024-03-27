#ifndef _LIBRO_H
#define _LIBRO_H

#include "autor.h"



// Estructura para representar un libro
typedef struct Libro{

    int id_Libro;
    char* Titulo;
    char* ruta_Libro;
    int fecha_Publicacion;

    Autor * autor_Libro;

} Libro;




#endif