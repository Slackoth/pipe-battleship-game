#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/msg.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "mensaje.h"
#include "tablero.h"
#include <iostream>
#include <string>

int fil;
int col;
int pos;
char superior[TAMANO_MAT][TAMANO_MAT];
char inferior[TAMANO_MAT][TAMANO_MAT];

std::string obtenerNombreTropa(char tropa);

int main() {
    int aux = 1;
    int jugadorUnoId;
    int tamanoMensaje;

    // Para red de mensajes
    mensaje enviar;
    mensaje recibir;

    // Inicializar tablero
    llenarTableros(superior, inferior);

    // Indicar que el jugador 1 ha empezado
    // enviar.tipo = 1;
    // enviar.filMat = 0;
    // enviar.colMat = 0;
    // enviar.tipoMensaje = 0;

    // // Calcular tamano de mensaje
    // tamanoMensaje = sizeof(mensaje) - sizeof(long);

    // // Inicializar pila de mensajes
    // jugadorUnoId = msgget(LLAVE, 0600 | IPC_CREAT);

    // // Mandar mensaje
    // msgsnd(jugadorUnoId, &enviar, tamanoMensaje, 0);

    // // Esperar hasta que el jugador 2 empiece
    // printf("Esperando al jugador 2...\n");

    // // Recibir mensaje del jugador 2
    // msgrcv(jugadorUnoId, &recibir, tamanoMensaje, 2, 0);

    // // Salir si la conexion entre jugador 1 y 2 falla
    // if(recibir.tipoMensaje != 0) {
    //     printf("ERROR: Conexion entre ambos jugadores fallo, vuelva a intentar.\n");

    //     // Eliminar pila de mensajes
    //     msgctl(jugadorUnoId, IPC_RMID, 0);

    //     return 0;
    // }

    // Mostrar tablero vacio
    mostrarTableros(superior, inferior);

    // Colocar tropas en tablero
    int tropasColocadas = 1;
    while(tropasColocadas <= 5) {
        std::cout << "Tropa: " << obtenerNombreTropa(tropasOrden.at(tropasColocadas)) << std::endl;
        std::cout << "Longitud: " << tropas.at(tropasOrden.at(tropasColocadas)) << " casillas" << std::endl;

        // printf("Ingrese la fila inicial de la tropa:\n");
        // scanf("%i", &fil);

        // printf("Ingrese la columna inicial de la tropa:\n");
        // scanf("%i", &col);

        // printf("¿Vertical (0) u Horizontal (1) ?:\n");
        // scanf("%i", &pos);

        std::pair<int, int> coordenada = {5, 0};
        bool tropaColocada = posicionarTropa(coordenada, static_cast<posicion>(0), tropasOrden.at(tropasColocadas), inferior);

        std::cout << "Tropa colocada: " << tropaColocada << std::endl;
        
        if(tropaColocada) {
            mostrarTableros(superior, inferior);
            tropasColocadas = 6;
        }
        else
            break;
    }

    // Inicializar juego
    // while(aux == 1) {
    //     printf("Ingrese coordenadas.\n");
    //     printf("Ingrese fila:\n");
    //     printf("Ingrese columna:\n");
    //     scanf("%i", &aux);
    // }

    return 0;
}

std::string obtenerNombreTropa(char tropa) {
    switch (tropa) {
        case 'T': return "Buque Transportador";
        case 'A': return "Buque Acorazado";
        case 'C': return "Crucero";
        case 'S': return "Submarino";
        case 'D': return "Buque Destructor";
        default: return "";
    }
}