#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>

#define tamanoPagina pow(2, 8) //4kb
#define numOffset pow(2, 8)    //256
int tabla_paginas[256];
char *memoria_principal[256];
int pfn = 0;
char *nombre_archivo_dest;
int c_Fichero;
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

void guardar_archivo_destino(int c_Fichero, int virtual, long int fisica, int valor)
{
    char buf[200];
    char *data;
    data=malloc(200);
    sprintf(data, "Virtual address: %d Physical address: %ld Value: %d \n",virtual, fisica, valor);
    strcpy(buf,data);
    write(c_Fichero,buf,strlen(buf));
}

void traducirLogicaFisica(int numero, int c_Fichero)
{
    unsigned long offset = numero & ((1u << 8) - 1);
    unsigned long numPagina = (numero >> 8) & (1 << (16 - 8)) - 1;
    char *texto;
    int val;
    printf("cfichero %d\n",c_Fichero);

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
        
guardar_archivo_destino(c_Fichero,numero, (pfn * 256) + offset, memoria_principal[pfn][offset]);
        //printf("Virtual address: %d Physical address: %ld Value: %d \n", numero, (pfn * 256) + offset, memoria_principal[pfn][offset]);
        tabla_paginas[numPagina] = pfn;
        pfn++;
        fclose(fp);
    }
    else
    {
        guardar_archivo_destino(c_Fichero,numero,(tabla_paginas[numPagina] * 256) + offset, memoria_principal[tabla_paginas[numPagina]][offset]);

       // printf("Virtual address: %d Physical address: %ld Value: %d\n", numero, (tabla_paginas[numPagina] * 256) + offset, memoria_principal[tabla_paginas[numPagina]][offset]);
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
        nombre_archivo_dest = argv[2];
        char caracter[1024];
        archivo = fopen(nombre_Archivo, "r");
        int c_Fichero = open(nombre_archivo_dest, O_CREAT | O_WRONLY | O_TRUNC);
            printf("cfichero %d\n",c_Fichero);
        if (archivo == NULL)
        {
            printf("Ups, parece que hubo un problema en abrir el archivo %s \n", nombre_Archivo);
        }
        else
        {
            printf("El contenido del archivo de prueba es \n\n");
            while (fgets(caracter, 1024, (FILE *)archivo))
            {
                traducirLogicaFisica(atoi(caracter),c_Fichero );
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
