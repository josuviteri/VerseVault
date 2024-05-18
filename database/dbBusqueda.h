#ifndef DBBUSQUEDA_H
#define DBBUSQUEDA_H


char* peticionTitulo(const char *titulo);
char* peticionAutor(const char *autor);
int peticionAutorPorTitulo(char* titulo);
int peticionIdLibroPorTitulo(const std::string& titulo);

#endif /* DBBUSQUEDA_H */