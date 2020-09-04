#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define tamanoPagina 4*1024 //4kb
#define numOffset 12 //4kb

/**
    @file assigment4.c
    @brief Función que retorna el numero de pagina y el offset de una dirección logica ingresada.
    @author Luis Ángel Moya Larrea.
    @date Septiembre 2020.
*/


/**
    @brief Función que toma parametros enviados por el usuario para generar un archivo con información aleatoria de procesos
    @param argc: cantidad de parametros de la función main, argv: lista de parametros de la función main.
    @returns void por defecto.
*/
void main(int argc, char *argv[])
{
   if (argc == 2)
    {
        int numeroDecimal=atoi(argv[1]);
        if(atoi(argv[1])>=0){
            unsigned long offset = numeroDecimal & ((1u << 12) - 1);
            //unsigned long offset = (numeroDecimal>>0)&(1<<(12-0))-1;
            unsigned long numPagina = (numeroDecimal>>12)&(1<<(32-12))-1;
            printf("La dirección %d contiene: \n",numeroDecimal); 
            printf("número de página =  %lu \n",numPagina); 
            printf("Offset= %lu \n",offset);
    }else{
            printf("Error en la dirección de memoria \n"); 
    }

    }else
    {
        printf("Error en el ingreso de parametros\n");
        printf("Por favor, ingrese unicamente una dirección virtual (En decimal)\n");
        exit(1);
    }
}