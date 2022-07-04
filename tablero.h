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

bool esPosibleArriba(std::pair<int, int>& coordenada, int casillas, char (*matriz)[TAMANO_MAT]) {
    for(int i = coordenada.first, j = casillas; j > 0; i--, j--) 
        if(matriz[i][coordenada.second] != 'V') 
            return false;
    
    return true;
}

bool esPosibleAbajo(std::pair<int, int>& coordenada, int casillas, char (*matriz)[TAMANO_MAT]) {
    for(int i = coordenada.first, j = casillas; j > 0; i++, j--) 
        if(matriz[i][coordenada.second] != 'V') 
            return false;
    
    return true;
}

bool esPosibleIzquierda(std::pair<int, int>& coordenada, int casillas, char (*matriz)[TAMANO_MAT]) {
    for(int i = coordenada.second, j = casillas; j > 0; i--, j--) 
        if(matriz[coordenada.first][i] != 'V') 
            return false;
    
    return true;
}

bool esPosibleDerecha(std::pair<int, int>& coordenada, int casillas, char (*matriz)[TAMANO_MAT]) {
    for(int i = coordenada.second, j = casillas; j > 0; i++, j--)  
        if(matriz[coordenada.first][i] != 'V') 
            return false;
    
    return true;
}

posicion obtenerPosicionVertical(std::pair<int, int>& coordenada, int casillas, char (*matriz)[TAMANO_MAT]) {
    if(coordenada.first - casillas + 1 >= 0) 
        if(esPosibleArriba(coordenada, casillas, matriz))
            return ARRIBA;
    
    if(coordenada.first + casillas < TAMANO_MAT)
        if(esPosibleAbajo(coordenada, casillas, matriz))
            return ABAJO;
    
    printf("Ya existe una tropa ocupando una de las casillas donde se quiere posicionar verticalmente la nueva tropa.\n");
    return INVALIDA;      
}

posicion obtenerPosicionHorizontal(std::pair<int, int>& coordenada, int casillas, char (*matriz)[TAMANO_MAT]) {
    if(coordenada.second - casillas + 1 >= 0) 
        if(esPosibleIzquierda(coordenada, casillas, matriz))
            return IZQUIERDA;
    
    if(coordenada.second + casillas < TAMANO_MAT)
        if(esPosibleDerecha(coordenada, casillas, matriz))
            return DERECHA;
    
    printf("Ya existe una tropa ocupando una de las casillas donde se quiere posicionar verticalmente la nueva tropa.\n");
    return INVALIDA; 
}

void posicionarArribaIzquierda(std::pair<int, int>& coordenada, std::pair<char, int>& tropa, posicion pos, char (*matriz)[TAMANO_MAT]) {    
    for(int i = pos == ARRIBA ? coordenada.first : coordenada.second, j = tropa.second; j > 0; i--, j--) 
        if(pos == ARRIBA)
            matriz[i][coordenada.second] = tropa.first;
        else
            matriz[coordenada.first][i] = tropa.first;
}

void posicionarAbajoDerecha(std::pair<int, int>& coordenada, std::pair<char, int>& tropa, posicion pos, char (*matriz)[TAMANO_MAT]) {    
    for(int i = pos == ABAJO ? coordenada.first : coordenada.second, j = tropa.second; j > 0; i++, j--) 
        if(pos == ABAJO)
            matriz[i][coordenada.second] = tropa.first;
        else
            matriz[coordenada.first][i] = tropa.first;
}

void posicionar(std::pair<int, int>& coordenada, std::pair<char, int>& tropa, posicion pos, char (*matriz)[TAMANO_MAT]) {
    switch (pos) {
        case ARRIBA:
        case IZQUIERDA: posicionarArribaIzquierda(coordenada, tropa, pos, matriz); break;
        
        case ABAJO:
        case DERECHA: posicionarAbajoDerecha(coordenada, tropa, pos, matriz); break;

        default: return;
    }
}

bool posicionarVerticalHorizontal(std::pair<int, int>& coordenada, std::pair<char, int>& tropa, posicion tipoPosicion, char (*matriz)[TAMANO_MAT]) {
    posicion pos = 
        tipoPosicion == VERTICAL ? obtenerPosicionVertical(coordenada, tropa.second, matriz) : obtenerPosicionHorizontal(coordenada, tropa.second, matriz);

    if(pos == INVALIDA)
        return false;
    else {
        posicionar(coordenada, tropa, pos, matriz);
        
        return true;
    }
}

bool posicionarTropa(std::pair<int, int>& coordenada, posicion tipoPosicion, char tropaKey, char (*matriz)[TAMANO_MAT]) {
    if(esCoordenadaValida(coordenada) == VALIDA) {
        std::pair<char, int> tropa = crearTropa(tropaKey);
        
        return posicionarVerticalHorizontal(coordenada, tropa, tipoPosicion, matriz);
    }
    else {
        printf("Ha ingresado una coordenada invalida, intente de nuevo.\n");
        return false;
    }
}

bool esCoordenadaIngresada(std::pair<int, int>& coordenada, char (*matriz)[TAMANO_MAT]) {
    return matriz[coordenada.first][coordenada.second] == 'X' || matriz[coordenada.first][coordenada.second] == 'O';
}

void registrarMiAtaque(std::pair<int, int>& coordenada, int movimiento, char (*matriz)[TAMANO_MAT]) {
    switch(movimiento) {
        case 0: matriz[coordenada.first][coordenada.second] = 'O'; printf("¡Fallaste!\n"); break;
        case 1: matriz[coordenada.first][coordenada.second] = 'X'; printf("¡Acertaste!\n"); break;
        default: matriz[coordenada.first][coordenada.second] = 'O'; break;
    }
}

int fueGolpe(std::pair<int, int>& coordenada, char (*matriz)[TAMANO_MAT]) {
    char tropa = matriz[coordenada.first][coordenada.second];

    switch(tropa) {
        case 'T':
        case 'A':
        case 'C':
        case 'S':
        case 'D': return 1;

        default: return 0;
    }
}

bool tropaHundida(std::pair<int, int>& coordenada, int movimiento, std::unordered_map<char, int>& tropas, char (*matriz)[TAMANO_MAT]) {
    if(movimiento == 1) {
        char tropa = matriz[coordenada.first][coordenada.second];
        int valor = tropas[tropa];

        // Disminuir valor de tropa
        tropas[tropa] = valor - 1;

        // Colocar golpe
        printf("¡Acerto!\n"); 
        matriz[coordenada.first][coordenada.second] = 'X';

        if(tropas[tropa] == 0) {
            printf("¡Tropa hundida!\n"); 
            return true;
        }
        else
            return false;
    }
    else 
        {
            matriz[coordenada.first][coordenada.second] = 'O';
            printf("¡Fallo!\n");
            return false;
        }
}

#endif