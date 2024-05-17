#ifndef _LIBRO_H
#define _LIBRO_H

#include "autor.h"

#define MAX_TITULO 200


// Estructura para representar un libro
typedef struct Libro{

    int id_Libro;
    char titulo[MAX_TITULO];
    int fecha_Publicacion;

    Autor * autor_Libro;

} Libro;




#endif