#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/msg.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "mensaje.h"

int fil;
int col;

int main() {
    int aux = 1;
    int jugadorUnoId;
    int tamanoMensaje;

    // Para red de mensajes
    mensaje enviar;
    mensaje recibir;

    // Inicializar tablero
    // TODO

    // Indicar que el jugador 1 ha empezado
    enviar.tipo = 1;
    enviar.filMat = 0;
    enviar.colMat = 0;
    enviar.tipoMensaje = 0;

    // Calcular tamano de mensaje
    tamanoMensaje = sizeof(mensaje) - sizeof(long);

    // Inicializar pila de mensajes
    jugadorUnoId = msgget(LLAVE, 0600 | IPC_CREAT);

    // Mandar mensaje
    msgsnd(jugadorUnoId, &enviar, tamanoMensaje, 0);

    // Esperar hasta que el jugador 2 empiece
    printf("Esperando al jugador 2...\n");

    // Recibir mensaje del jugador 2
    msgrcv(jugadorUnoId, &recibir, tamanoMensaje, 2, 0);

    // Salir si la conexion entre jugador 1 y 2 falla
    if(recibir.tipoMensaje != 0) {
        printf("ERROR: Conexion entre ambos jugadores fallo, vuelva a intentar.\n");

        // Eliminar pila de mensajes
        msgctl(jugadorUnoId, IPC_RMID, 0);

        return 0;
    }

    // Mostrar tablero vacio
    // TODO

    // Inicializar juego
    while(aux == 1) {
        printf("Ingrese coordenadas.\n");
        printf("Ingrese fila:\n");
        printf("Ingrese columna:\n");
        scanf("%i", &aux);
    }

    return 0;
}