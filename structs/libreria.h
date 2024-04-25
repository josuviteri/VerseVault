#ifndef _LIBRERIA_H
#define _LIBRERIA_H

#include "libro.h"


typedef struct Libreria{
    
    int id_libreria;
    char *nom_libreria;
    char *ciudad_libreria;
    char *direccion_libreria;
    char *horario_libreria;
    Libro* libros; 
    int num_libros; 

} Libreria;

#endif