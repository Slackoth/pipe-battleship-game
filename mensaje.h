#ifndef MENSAJE_H
#define MENSAJE_H

typedef struct Mensaje {
    long tipo;
    int filMat; // Fila de matriz
    int colMat; // Columna de matriz
    long tipoMensaje;
} mensaje;

#endif