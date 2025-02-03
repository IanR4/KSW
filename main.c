#include "parser.h"

int main (int argc, char *argv[]){
    entrada = fopen(argv[1], "r");
    if (!entrada) {
        perror("Error al abrir el archivo de entrada");
        return 1;
    }
    inicializarDiccionario(tabla_simb);
    agregarPalabrasReservadas(tabla_simb);
    /*FILE* salida = fopen(argv[2], "w");
    if (!salida) {
        perror("Error al abrir el archivo de salida");
        fclose(entrada);
        return 1;
    }
    */
    /*token tok;
    do {
        tok = scanner();
    } while (tok != FDT);*/
    parser();

    fclose(entrada);
    return 0;
}