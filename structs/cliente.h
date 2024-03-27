#ifndef _CLIENTE_H
#define _CLIENTE_H

#define MAX_NOMB_CLIENTE 15
#define MAX 20


typedef struct Cliente{

    int id_Cliente;
    char nom_Cliente[MAX_NOMB_CLIENTE];
    char * email_cl;
    char passw[MAX]; // No se si es mejor hacerlo dinamica o de esta forma? para los casos de poner limites
    char * fecha_nac_cl;
    int es_admin;
    int id_ciudad;

} Cliente;

#endif