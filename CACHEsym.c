#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>

typedef struct {
    short int ETQ;
    short int Datos[8];
}T_LINEA_CACHE;

T_LINEA_CACHE cache[4];
int tiempoGlobal = 0;
int numFallos = 0;
int numAccesos = 0;
unsigned char RAM[1024];

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
    FILE *fRAM;
    fRAM = fopen("RAM.bin", "rb");
    if (fRAM == NULL){
        printf("El fichero no existe\n");
        exit(-1);
    }
    fread(RAM, sizeof(RAM), 1, fRAM);
    fclose(fRAM);
}

//Metodo para imprimir los datos de la cache
void imprimirDatos(){
    for (int i = 0; i < 4; i++){
            printf("ETQ:%X   Datos ", cache[i].ETQ);
            for (int j = 7; j >= 0; j--){
                printf("%X ", cache[i].Datos[j]);
            }
            printf("\n");
    }
}

//Método para leer y descomponer los números hexadecimales para luego comprobar si la etiqueta de la direcion es igual a la de la linea de la cache
void readFHexadecimal(){
    FILE *fichero;
    unsigned int hexNum[12];
    short int etiqueta;
    short int palabra;
    short int linea;
    short int bloque;
    char texto[100];
    int contador = 0;
    fichero = fopen("accesos_memoria.txt", "r");
    if (fichero == NULL){
        printf("El fichero no existe\n");
        exit(-1);
    }

    for (int i = 0; i < 12; i++){
        fscanf(fichero, "%X", &hexNum[i]);
        linea = (hexNum[i] & 0b0000011000) >> 3;
        palabra = (hexNum[i] & 0b0000000111);
        etiqueta = hexNum[i] >> 5;
        bloque = hexNum[i] >> 3;

        //Comprobacion de si la etiqueta de la direcion es igual a la de la linea de la cache
        if (cache[linea].ETQ != etiqueta) {
            numFallos++;
            printf("T: %d, Fallo de CACHE %d, ADDR %04X ETQ %X linea %02X palabra %02X bloque %02X\n", tiempoGlobal, numFallos, hexNum[i], etiqueta, linea, palabra, bloque);
            tiempoGlobal += 10;
            cache[linea].ETQ = etiqueta;
            //Se copia el bloque correspondiente desde el array RAM
            for (int j = 0; j < 8; j++){
                cache[linea].Datos[j] = RAM[bloque * 8 + j];
            }
            printf("Cargando el bloque %02X en la linea %02X\n", bloque, linea);

        }
        if (cache[linea].ETQ == etiqueta){
            numAccesos++;
            tiempoGlobal += 1;
            printf("T: %d, Acierto de CACHE, ADDR %04X ETQ %X linea %02X palabra %02X DATO %02X\n", tiempoGlobal, hexNum[i], etiqueta, linea, palabra, RAM[bloque *8 + palabra]);
            texto[contador+1] = RAM[bloque *8 + palabra];
            contador++;
        }
        
        imprimirDatos();
        sleep(2);
        printf("\n");
    }
    printf("\nNúmero total de accesos: %d\nNúmero de fallos: %d\nTexto leido: ", numAccesos, numFallos);
    for (int j = 0; texto[j] != '\0'; j++){
        printf("%c", texto[j]);
    }
    printf("\n");
    fclose(fichero);
    exit(-1);
}

void main(int argc, char *argv[])
{
    inicializar();
    readFBinary();
    readFHexadecimal();
}
