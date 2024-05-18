#ifndef _CLIENTE_H
#define _CLIENTE_H

#define MAX_NOMB_CLIENTE 15
#define MAX_EMAIL 100
#define MAX_PASS 20
#define MAX_FECHA 20

typedef struct Cliente {
    int id_Cliente;
    char nom_Cliente[MAX_NOMB_CLIENTE];
    char email_cl[MAX_EMAIL];
    char passw[MAX_PASS]; // Limitar el tamaño de la contraseña
    char fecha_nac_cl[MAX_FECHA];
    int es_admin;
    int id_ciudad;
} Cliente;

#endif