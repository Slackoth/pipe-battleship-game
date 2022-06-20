#ifndef TABLERO_H
#define TABLERO_H
#define TAMANO_MAT 10

#include "stdio.h"

void llenarTablero(char (*matriz)[TAMANO_MAT]) {
    for(int i = 0; i < TAMANO_MAT; i++)
        for(int j = 0; j < TAMANO_MAT; j++)
            matriz[i][j] = 'T';
}

void llenarTableros(char (*superior)[TAMANO_MAT], char (*inferior)[TAMANO_MAT]) {
    llenarTablero(superior);
    llenarTablero(inferior);
}

char obtenerCaracterAdecuado(char casilla) {
    switch (casilla) {
        case 'T': return ' ';
        
        default: return ' ';
    }
}

void mostrarTablero(char (*matriz)[TAMANO_MAT]) {
    // Encabezado
    printf("| C |");
    for (int i = 0; i < TAMANO_MAT; i++) {
        printf("| %i |", i);
    }
    printf("\n");

    // Cuerpo
    for (int i = 0; i < TAMANO_MAT; i++) {
        printf("| %i |", i);

        for(int j = 0; j < TAMANO_MAT; j++) {
            printf("| %c |", obtenerCaracterAdecuado(matriz[i][j]));
        }
        printf("\n");
    }
    
}

void mostrarTableros(char (*superior)[TAMANO_MAT], char (*inferior)[TAMANO_MAT]) {
    // Mostrar superior
    mostrarTablero(superior);

    // Division
    for (int i = 0; i < TAMANO_MAT + 1; i++) {
        printf("  %c  ", '*');
    }
    printf("\n");

    // Mostrar inferior
    mostrarTablero(inferior);
}

#endif