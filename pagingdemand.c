#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define tamanoPagina pow(2, 8) //4kb
#define numOffset pow(2, 8)    //256
int tabla_paginas[256];
char *memoria_principal[256];
int pfn = 0;

void llenar_tabla_paginas()
{
    for (int i = 0; i < (sizeof(tabla_paginas) / sizeof(tabla_paginas[0])); i++)
    {
        tabla_paginas[i] = -1;
    }
}
void llenar_memoria_principal()
{
    for (int i = 0; i < 256; i++)
    {
        memoria_principal[i] = malloc(256);
    }
}

void traducirLogicaFisica(int numero)
{
    unsigned long offset = numero & ((1u << 8) - 1);
    unsigned long numPagina = (numero >> 8) & (1 << (16 - 8)) - 1;
    char *texto;
    int val;
    if (tabla_paginas[numPagina] == -1)
    {
        FILE *fp = fopen("BACKING_STORE.bin", "rb");
        int val_fseek = fseek(fp, numPagina * 256, SEEK_SET);
        if (val_fseek != 0)
        {
            printf("Error en el seek");
            return;
        }
        fread(memoria_principal[pfn], 256, 1, fp); // Lee el archivo sobre tabla2
        printf("Virtual address: %d Physical address: %ld Value: %d \n",numero,(pfn*256)+offset,memoria_principal[pfn][offset] );
        tabla_paginas[numPagina] = pfn;
        pfn++;
        fclose(fp);
    }
    else
    {
        printf("Virtual address: %d Physical address: %ld Value: %d\n",numero,(tabla_paginas[numPagina]*256)+offset,memoria_principal[tabla_paginas[numPagina]][offset]);
    }
}

void main(int argc, char *argv[])
{
    if (argc == 3)
    {
        llenar_tabla_paginas();
        llenar_memoria_principal();
        FILE *archivo;
        char *nombre_Archivo = argv[1];
        char caracter[1024];
        archivo = fopen(nombre_Archivo, "r");
        if (archivo == NULL)
        {
            printf("Ups, parece que hubo un problema en abrir el archivo %s \n", nombre_Archivo);
        }
        else
        {
            printf("El contenido del archivo de prueba es \n\n");
            while (fgets(caracter, 1024, (FILE *)archivo))
            {
                traducirLogicaFisica(atoi(caracter));
            }
        }
        fclose(archivo);
    }
    else
    {
        printf("Error en el ingreso de parametros\n");
        printf("Por favor, ingrese unicamente una dirección virtual (En decimal)\n");
        exit(1);
    }
}
