#ifndef DBBUSQUEDA_H
#define DBBUSQUEDA_H


void peticionTitulo(const char *titulo);
void peticionAutor(const char *autor);
int peticionAutorPorTitulo(char* titulo);
int peticionIdLibroPorTitulo(const std::string& titulo);

#endif /* DBBUSQUEDA_H */