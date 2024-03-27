#ifndef _CIUDAD_H
#define _CIUDAD_H

#include "pais.h"

typedef struct Ciudad{

    int id_Ciudad;
    char* nom_Ciudad;
    Pais * pais;

} Ciudad;

#endif