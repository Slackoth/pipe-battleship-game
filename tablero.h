#ifndef TABLERO_H
#define TABLERO_H
#define TAMANO_MAT 10

#include "stdio.h"
#include <utility>
#include <unordered_map>

enum posicion { VERTICAL, HORIZONTAL, ARRIBA, ABAJO, DERECHA, IZQUIERDA, VALIDA, INVALIDA };
std::unordered_map<char, int> tropas = {
    {'T', 5}, // Buque Transportador
    {'A', 4}, // Buque Acorazado
    {'C', 3}, // Crucero
    {'S', 3}, // Submarino
    {'D', 2}  // Buque Destructor 
};
std::unordered_map<char, int> tropasOrden = {
    {1, 'T'}, // Buque Transportador
    {2, 'A'}, // Buque Acorazado
    {3, 'C'}, // Crucero
    {4, 'S'}, // Submarino
    {5, 'D'}  // Buque Destructor 
};

void llenarTablero(char (*matriz)[TAMANO_MAT]) {
    for(int i = 0; i < TAMANO_MAT; i++)
        for(int j = 0; j < TAMANO_MAT; j++)
            matriz[i][j] = 'V';
}

void llenarTableros(char (*superior)[TAMANO_MAT], char (*inferior)[TAMANO_MAT]) {
    llenarTablero(superior);
    llenarTablero(inferior);
}

char obtenerCaracterAdecuado(char casilla) {
    switch (casilla) {
        case 'V': return ' ';
        
        default: return casilla;
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

int esCoordenadaValida(std::pair<int, int>& coordenada) {
    if(coordenada.first >= 0 && coordenada.first < TAMANO_MAT && coordenada.second >= 0 && coordenada.second < TAMANO_MAT)
        return VALIDA;
    else
        return INVALIDA;
}

std::pair<char, int> crearTropa(char tropaKey) {
    return {tropaKey, tropas.at(tropaKey)};
}

bool esPosibleArriba(int x, int casillas, char (*matriz)[TAMANO_MAT]) {
    for(int i = x, j = casillas; j > 0; i--, j--) 
        if(matriz[i][casillas] != 'V') 
            return false;
    
    return true;
}

bool esPosibleAbajo(int y, int casillas, char (*matriz)[TAMANO_MAT]) {
    for(int i = y; i < casillas; i++) 
        if(matriz[i][casillas] != 'V') 
            return false;
    
    return true;
}

posicion obtenerPosicionVertical(std::pair<int, int>& coordenada, int casillas, char (*matriz)[TAMANO_MAT]) {
    if(coordenada.first - casillas + 1 >= 0) 
        if(esPosibleArriba(coordenada.first, casillas, matriz))
            return ARRIBA;
    
    if(coordenada.first + casillas < TAMANO_MAT)
        if(esPosibleAbajo(coordenada.first, casillas, matriz))
            return ABAJO;
    
    printf("Ya existe una tropa ocupando una de las casillas donde se quiere posicionar verticalmente la nueva tropa.\n");
    return INVALIDA;      
}

posicion obtenerPosicionHorizontal(int y, int casillas) {
    return INVALIDA; 
}

bool posicionarVertical(std::pair<int, int>& coordenada, std::pair<char, int>& tropa, char (*matriz)[TAMANO_MAT]) {
    posicion pos = obtenerPosicionVertical(coordenada, tropa.second, matriz);

    if(pos == INVALIDA)
        return false;
    else if(pos == ARRIBA) {
        for(int i = coordenada.first, j = tropa.second; j > 0; i--, j--) 
            matriz[i][coordenada.second] = tropa.first;
        
        return true;
    }
    else if(pos == ABAJO) {
        for(int i = coordenada.first, j = tropa.second; j > 0; i++, j--) 
            matriz[i][coordenada.second] = tropa.first;
        
        return true;
    }
    else 
        return false; 
}

bool posicionarHorizontal(std::pair<int, int>& coordenada, std::pair<char, int>& tropa, char (*matriz)[TAMANO_MAT]) {
    return true;
}

bool posicionarTropa(std::pair<int, int>& coordenada, posicion tipoPosicion, char tropaKey, char (*matriz)[TAMANO_MAT]) {
    if(esCoordenadaValida(coordenada) == VALIDA) {
        std::pair<char, int> tropa = crearTropa(tropaKey);
        switch (tipoPosicion) {
            case VERTICAL: return posicionarVertical(coordenada, tropa, matriz);
            case HORIZONTAL: return posicionarHorizontal(coordenada, tropa, matriz);
            
            default: return false;
        }
        printf("Coordenada valida.\n");
        return true;
    }
    else {
        printf("Ha ingresado una coordenada invalida, intente de nuevo.\n");
        return false;
    }
}

#endif