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
    printf("caracter: %d\n", RAM[216]);
    fclose(fRAM);
}

//Método para leer y descomponer los números hexadecimales
void readFHexadecimal(){
    FILE *fichero;
    unsigned int hexNum[12];
    short int etiqueta;
    short int palabra;
    short int linea;
    short int bloque;
    char texto[100];
    fichero = fopen("accesos_memoria.txt", "r");
    if (fichero == NULL){
        printf("El fichero no existe\n");
        exit(-1);
    }

    fscanf(fichero, "%X", &hexNum[0]);
    linea = (hexNum[0] & 0b0000011000) >> 3;
    palabra = (hexNum[0] & 0b0000000111);
    etiqueta = hexNum[0] >> 5;
    bloque = hexNum[0] >> 3;
    printf("Linea %d\n", linea);
    printf("Palabra %d\n", palabra);
    printf("Etiqueta %d\n", etiqueta);
    printf("Bloque %d\n", bloque);
    printf("Numero %x\n", hexNum[0]); 

    if (cache[0].ETQ == etiqueta){
        printf("T: %d, Acierto de CACHE, ADDR %04X ETQ %X linea %02X palabra %02X DATO %02X", tiempoGlobal, hexNum[0], etiqueta, linea, palabra, tiempoGlobal);
        tiempoGlobal += 1;
    }
    else {
        numFallos++;
        printf("T: %d, Fallo de CACHE %d, ADDR %04X ETQ %X linea %02X palabra %02X bloque %02X", tiempoGlobal, numFallos, hexNum[0], etiqueta, linea, palabra, bloque);
        tiempoGlobal += 10;

    }
    
    fclose(fichero);
}

void main(int argc, char *argv[])
{
    inicializar();
    readFBinary();
    readFHexadecimal();
}
