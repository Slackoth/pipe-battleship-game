#ifndef MENSAJE_H
#define MENSAJE_H
#define LLAVE 34856 

enum movimiento { FALLO, GOLPE, SIN_ASIGNAR };
// tipo:
// 		0 = ver "tipoMensaje"
// 		1 = jugador 1 ha empezado.
// 		2 = jugador 2 ha empezado.
//      3 = jugador 1 ha colocado sus tropas.
//      4 = jugador 2 ha colocado sus tropas.
// tipoMensaje:
// 		0 = Esperar al otro jugador.
// 		1 = Turno del otro jugador.
// 		2 = Ganador jugador 1.
// 		3 = Ganador jugador 2.

typedef struct Mensaje {
    long tipo;
    int filMat; // Fila de matriz
    int colMat; // Columna de matriz
    movimiento mov;
    bool tropaHundida; 
    long tipoMensaje;
} mensaje;

#endif