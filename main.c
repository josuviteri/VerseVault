#include <stdio.h>
// #include <glew.h>    Estas 2 librerías serán utilizadas 
// #include <imgui.h>   para crear y gestionar las ventanas
#define NEGRITA "\e[1m" // Renombramos el código de los caracteres en negrita para que sea mas entendible
#define QUITAR_NEGRITA "\e[m" // Renombramos el codigo de quitar la negrita a los caracteres para que sea mas entendible

int main(void)
{
printf("Proyecto Programacion IV\n");
printf(NEGRITA"Libreria virtual\n\n" QUITAR_NEGRITA);

return 0;
}
