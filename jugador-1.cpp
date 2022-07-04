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
int misTropas = 1;
int oponenteTropas = 1;
char superior[TAMANO_MAT][TAMANO_MAT];
char inferior[TAMANO_MAT][TAMANO_MAT];
std::unordered_map<char, int> jugadorTropas = {
    {'T', 5}, // Buque Transportador
    {'A', 4}, // Buque Acorazado
    {'C', 3}, // Crucero
    {'S', 3}, // Submarino
    {'D', 2}  // Buque Destructor 
};

int main() {
    int jugadorUnoId;
    int tamanoMensaje;

    // Para red de mensajes
    mensaje enviar;
    mensaje recibir;

    // Inicializar tablero
    llenarTableros(superior, inferior);

    // Indicar que el jugador 1 ha empezado
    enviar.tipo = 1;
    enviar.filMat = 0;
    enviar.colMat = 0;
    enviar.tipoMensaje = 0;
    enviar.mov = SIN_ASIGNAR;
    enviar.tropaHundida = false;

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
    mostrarTableros(superior, inferior);

    // Colocar tropas en tablero
    int tropasColocadas = 5;
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

    // Indicar que el jugador 1 ha colocado sus tropas
    enviar.tipo = 1;
    enviar.filMat = 0;
    enviar.colMat = 0;
    enviar.tipoMensaje = 0;
    enviar.mov = SIN_ASIGNAR;
    enviar.tropaHundida = false;

    // Mandar mensaje
    msgsnd(jugadorUnoId, &enviar, tamanoMensaje, 0);

    // Esperar hasta que el jugador 2 coloque sus tropas
    printf("Esperando a que el jugador 2  coloque sus tropas...\n");

    // Recibir mensaje del jugador 2
    msgrcv(jugadorUnoId, &recibir, tamanoMensaje, 2, 0);

    // Inicializar juego
    while(1) {
        int ganador = hayGanador(misTropas, oponenteTropas);

        switch (ganador) {
            case 0: break;
            case 1: printf("¡Ha ganado el jugador 1!\n"); return 0;
            case 2: printf("¡Ha ganado el jugador 2!\n"); return 0;
            default: break;
        }

        printf("COORDENADAS DE ATAQUE\n");
        
        printf("Ingrese fila:\n");
        scanf("%i", &fil);
        
        printf("Ingrese columna:\n");
        scanf("%i", &col);

        std::pair<int, int> coordenada = {fil, col};

        if(esCoordenadaValida(coordenada)) {
            if(esCoordenadaIngresada(coordenada, superior))
                printf("Coordenada ya ingresada.\n");
            else {
                // Enviar ataque
                enviar.tipo = 1;
                enviar.filMat = fil;
                enviar.colMat = col;
                enviar.tipoMensaje = 1;
                enviar.mov = SIN_ASIGNAR;
                enviar.tropaHundida = false;
                
                msgsnd(jugadorUnoId, &enviar, tamanoMensaje, 0);

                // Esperar si fue un golpe y hundio una tropa o un fallo por parte del jugador 2
                printf("Esperando si fue un golpe o un fallo por parte del jugador 2...\n");

                msgrcv(jugadorUnoId, &recibir, tamanoMensaje, 2, 0);

                // Registrar golpe o fallo en tablero superior
                registrarMiAtaque(coordenada, static_cast<int>(recibir.mov), superior);

                // Disminuir tropas del oponente si una tropa fue hundida
                disminuirTropas(oponenteTropas, recibir.tropaHundida, "¡Tropa del oponente hundida! Tropas restantes: %i\n");
                
                // Mostrar estado del tablero despues de mi ataque
                mostrarTableros(superior, inferior);

                ganador = hayGanador(misTropas, oponenteTropas);

                switch (ganador) {
                    case 0: break;
                    case 1: printf("¡Ha ganado el jugador 1!\n"); return 0;
                    case 2: printf("¡Ha ganado el jugador 2!\n"); return 0;
                    default: break;
                }

                // Esperar al movimiento del jugador 2
                printf("Esperando al movimiento del jugador 2...\n");

                msgrcv(jugadorUnoId, &recibir, tamanoMensaje, 0, 0);

                // Verificar que haya sido golpe o fallo
                coordenada.first = recibir.filMat;
                coordenada.second = recibir.colMat;
                int golpe = fueGolpe(coordenada, inferior);
                
                // Registrar si fue golpe en tablero inferior
                bool hundida = tropaHundida(coordenada, golpe, jugadorTropas, inferior);
                
                // Disminuir mis tropas
                disminuirTropas(misTropas, hundida, "¡Tropa propia hundida! Tropas restantes: %i\n");

                // Mostrar estado del tablero despues del ataque del oponente
                mostrarTableros(superior, inferior);

                // Enviar si el ataque fue un golpe y hundio una tropa o un fallo
                printf("Enviando si fue un golpe o un fallo al jugador 2...\n");

                enviar.tipo = 1;
                enviar.filMat = recibir.filMat;
                enviar.colMat = recibir.colMat;
                enviar.tipoMensaje = 1;
                enviar.mov = static_cast<movimiento>(golpe);
                enviar.tropaHundida = hundida;

                msgsnd(jugadorUnoId, &enviar, tamanoMensaje, 0);

                ganador = hayGanador(misTropas, oponenteTropas);

                switch (ganador) {
                    case 0: break;
                    case 1: printf("¡Ha ganado el jugador 1!\n"); return 0;
                    case 2: printf("¡Ha ganado el jugador 2!\n"); return 0;
                    default: break;
                }
            }
        }
        else
            printf("Las coordenadas (%i,%i) son invalidas. Ingreselas de nuevo.\n", fil, col);
    }

    return 0;
}