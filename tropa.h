#ifndef TROPAS_H
#define TROPAS_H

typedef struct Tropa {
    int tamano; // Numero de casillas que toma la tropa en el tablero
    int simbolo; // Caracter que representa la tropa en el tablero
    bool hundido; // Bandera para saber si esta hundida o no la tropa
    int contHundido; // Contador para saber cuanto falta para hundir la tropa
} tropa;

#endif