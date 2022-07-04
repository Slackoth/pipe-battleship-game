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
int oponenteTropas = 5;
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
    enviar.mov = SIN_ASIGNAR;
    enviar.tropaHundida = false;

    // Enviar mensaje
    msgsnd(jugadorDosId, &enviar, tamanoMensaje, 0);

    // Mostrar tablero vacio
    mostrarTableros(superior, inferior);

    // Colocar tropas en tablero
    int tropasColocadas = 1;
    while(tropasColocadas <= 1) {
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

    // Esperar hasta que el jugador 1 coloque sus tropas
    printf("Esperando a que el jugador 1 coloque sus tropas...\n");

    // Recibir confirmacion del jugador 1
    msgrcv(jugadorDosId, &recibir, tamanoMensaje, 1, 0);

    // Salir si la conexion entre jugador 1 y 2 falla
    if(recibir.tipoMensaje != 0) {
        printf("ERROR: Conexion entre ambos jugadores fallo, vuelva a intentar.\n");

        // Eliminar pila de mensajes
        msgctl(jugadorDosId, IPC_RMID, 0);

        return 0;
    }

    // Indicar que el jugador 2 ha colocado sus tropas
    enviar.tipo = 2;
    enviar.filMat = 0;
    enviar.colMat = 0;
    enviar.tipoMensaje = 0;
    enviar.mov = SIN_ASIGNAR;
    enviar.tropaHundida = false;

    // Enviar mensaje
    msgsnd(jugadorDosId, &enviar, tamanoMensaje, 0);

    // Esperar movimiento del jugador 1
    printf("Esperando al movimiento del jugador 1...\n");

    // Recibir primer movimiento del jugador 1
    msgrcv(jugadorDosId, &recibir, tamanoMensaje, 1, 0);

    // Verificar que haya sido golpe o fallo
    std::pair<int, int> c = {recibir.filMat, recibir.colMat};
    int golpe = fueGolpe(c, inferior);

    // Registrar si fue golpe en tablero inferior
    bool hundida = tropaHundida(c, golpe, jugadorTropas, inferior);

    // Mostrar estado del tablero despues del ataque del oponente
    mostrarTableros(superior, inferior);

    // Enviar si el ataque fue un golpe y hundio una tropa o un fallo
    printf("Enviando si fue un golpe o un fallo al jugador 1...\n");

    enviar.tipo = 1;
    enviar.filMat = recibir.filMat;
    enviar.colMat = recibir.colMat;
    enviar.tipoMensaje = 1;
    enviar.mov = static_cast<movimiento>(golpe);
    enviar.tropaHundida = hundida;

    msgsnd(jugadorDosId, &enviar, tamanoMensaje, 0);

    // Inicializar juego
    while(1) {
        if(hayGanador(oponenteTropas)) {
            printf("¡Ha ganado el jugador 2!\n");

            enviar.tipo = 1;
            enviar.filMat = 0;
            enviar.colMat = 0;
            enviar.tipoMensaje = 3;
            enviar.mov = SIN_ASIGNAR;
            enviar.tropaHundida = false;
            
            msgsnd(jugadorDosId, &enviar, tamanoMensaje, 0);
            return 0;
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
                
                msgsnd(jugadorDosId, &enviar, tamanoMensaje, 0);

                // Esperar si fue un golpe y hundio una tropa o un fallo por parte del jugador 2
                printf("Esperando si fue un golpe o un fallo por parte del jugador 1...\n");

                msgrcv(jugadorDosId, &recibir, tamanoMensaje, 0, 0);

                // Registrar golpe o fallo en tablero superior
                registrarMiAtaque(coordenada, static_cast<int>(recibir.mov), superior);

                // Disminuir tropas del oponente si una tropa fue hundida
                disminuirOponenteTropas(oponenteTropas, recibir.tropaHundida);
                
                // Mostrar estado del tablero despues de mi ataque
                mostrarTableros(superior, inferior);

                // Esperar al movimiento del jugador 1
                printf("Esperando al movimiento del jugador 1...\n");

                msgrcv(jugadorDosId, &recibir, tamanoMensaje, 0, 0);

                // Verificar que haya sido golpe o fallo
                int golpe = fueGolpe(coordenada, inferior);
                
                // Registrar si fue golpe en tablero inferior
                bool hundida = tropaHundida(coordenada, golpe, jugadorTropas, inferior);

                // Mostrar estado del tablero despues del ataque del oponente
                mostrarTableros(superior, inferior);

                // Enviar si el ataque fue un golpe y hundio una tropa o un fallo
                printf("Enviando si fue un golpe o un fallo al jugador 1...\n");

                enviar.tipo = 1;
                enviar.filMat = recibir.filMat;
                enviar.colMat = recibir.colMat;
                enviar.tipoMensaje = 1;
                enviar.mov = static_cast<movimiento>(golpe);
                enviar.tropaHundida = hundida;

                msgsnd(jugadorDosId, &enviar, tamanoMensaje, 0);

                if(recibir.tipoMensaje == 2) {
                    printf("¡Ha ganado el jugador 1!\n");

                    msgctl(jugadorDosId, IPC_RMID, 0);
					return 0;
                }
            }
        }
        else
            printf("Las coordenadas (%i,%i) son invalidas. Ingreselas de nuevo.\n", fil, col);
    }

    return 0;
}