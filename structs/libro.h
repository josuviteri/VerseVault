#ifndef _LIBRO_H
#define _LIBRO_H

#include "autor.h"

#define MAX_TITULO 200
#define MAX_AUTOR 200

// Estructura para representar un libro
typedef struct Libro{

    int id_Libro;
    char titulo[MAX_TITULO];
    char fecha_Publicacion[11];
    char idioma[4];
    char autor_Libro[MAX_AUTOR];

} Libro;




#endif