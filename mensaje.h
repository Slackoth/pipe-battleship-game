#ifndef MENSAJE_H
#define MENSAJE_H
#define LLAVE 34856 

// tipo:
// 		0 = ver "tipoMensaje"
// 		1 = jugador 1 ha empezado.
// 		2 = jugador 2 ha empezado.
// tipoMensaje:
// 		0 = Esperar al otro jugador.
// 		1 = Turno del otro jugador.
// 		2 = Ganador jugador 1.
// 		3 = Ganador jugador 2.

typedef struct Mensaje {
    long tipo;
    int filMat; // Fila de matriz
    int colMat; // Columna de matriz
    long tipoMensaje;
} mensaje;

#endif