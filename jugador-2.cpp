#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/msg.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "mensaje.h"
#include "tablero.h"
#include "utils.h"
#include <iostream>

int fil;
int col;
int pos;
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
    int tropasColocadas = 1;
    while(tropasColocadas <= 5) {
        std::cout << "Tropa: " << obtenerNombreTropa(tropasOrden.at(tropasColocadas)) << std::endl;
        std::cout << "Longitud: " << tropas.at(tropasOrden.at(tropasColocadas)) << " casillas" << std::endl;

        printf("Ingrese la fila inicial de la tropa:\n");
        scanf("%i", &fil);

        printf("Ingrese la columna inicial de la tropa:\n");
        scanf("%i", &col);

        printf("¿Vertical (0) u Horizontal (1) ?:\n");
        scanf("%i", &pos);

        std::pair<int, int> coordenada = {fil, col};
        bool tropaColocada = posicionarTropa(coordenada, static_cast<posicion>(pos), tropasOrden.at(tropasColocadas), inferior);
        
        if(tropaColocada) {
            tropasColocadas++;
            mostrarTableros(superior, inferior);
        }
    }

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