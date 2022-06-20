#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/msg.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "mensaje.h"
#include "tablero.h"

int fil;
int col;
char superior[TAMANO_MAT][TAMANO_MAT];
char inferior[TAMANO_MAT][TAMANO_MAT];

int main() {
    int aux = 1;
    int jugadorDosId;
    int tamanoMensaje;

    // Para red de mensajes
    mensaje enviar;
    mensaje recibir;

    // Inicializar tablero
    llenarTableros(superior, inferior);

    // Calcular tamano de mensaje
    tamanoMensaje = sizeof(mensaje) - sizeof(long);

    // Inicializar pila de mensajes
    jugadorDosId = msgget(LLAVE, 0600 | IPC_CREAT);

    // Esperar hasta que el jugador 1 empiece
    printf("Esperando al jugador 1...\n");

    // Recibir confirmacion del jugador 1
    msgrcv(jugadorDosId, &recibir, tamanoMensaje, 1, 0);

    // Salir si la conexion entre jugador 1 y 2 falla
    if(recibir.tipoMensaje != 0) {
        printf("ERROR: Conexion entre ambos jugadores fallo, vuelva a intentar.\n");

        // Eliminar pila de mensajes
        msgctl(jugadorDosId, IPC_RMID, 0);

        return 0;
    }

    // Indicar que el jugador 2 ha empezado
    enviar.tipo = 2;
    enviar.filMat = 0;
    enviar.colMat = 0;
    enviar.tipoMensaje = 0;

    // Enviar mensaje
    msgsnd(jugadorDosId, &enviar, tamanoMensaje, 0);

    // Mostrar tablero vacio
    mostrarTableros(superior, inferior);

    // Colocar tropas en tablero
    // TODO

    // Esperar movimiento del jugador 1
    printf("Esperando al jugador 1...\n");

    // Recibir primer movimiento del jugador 1
    msgrcv(jugadorDosId, &recibir, tamanoMensaje, 0, 0);

    // Reflejar movimiento del jugador 1
    // TODO

    // Mostrar tablero
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