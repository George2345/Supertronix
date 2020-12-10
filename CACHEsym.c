#include "stdio.h"
#include "stdlib.h"

typedef struct {
    short int ETQ;
    short int Datos[8];
}T_LINEA_CACHE;

T_LINEA_CACHE cache[4];
int tiempoGlobal = 0;
int numFallos = 0;

//Metodo que inicializa todas los campos de la cache
void inicializar(){
    for (int i = 0; i < 4; i++){
        cache[i].ETQ = 0xFF;
        for (int j = 0; j < 8; j++){
            cache[i].Datos[j] = 0;
        }
    }
}

//Metodo para leer el fichero binario RAM.bin
void readFBinary(){
    unsigned char RAM[1024];
    FILE *fRAM;
    fRAM = fopen("RAM.bin", "rb");
    if (fRAM == NULL){
        printf("El fichero no existe\n");
        exit(-1);
    }
    fread(RAM, sizeof(RAM), 1, fRAM);
    printf("caracter: %d\n", RAM[499]);
    fclose(fRAM);
}

void readFHexadecimal(){
    FILE *fichero;
    unsigned int num[12];
    fichero = fopen("accesos_memoria.txt", "r");
    if (fichero == NULL){
        printf("El fichero no existe\n");
        exit(-1);
    }
    fscanf(fichero, "%X", num);
    printf("Numero %d\n", num[0]);
    
    fclose(fichero);
}

void main(int argc, char *argv[])
{
    inicializar();
    readFBinary();
    readFHexadecimal();
}
