#ifndef UTILS_H

#include <string>

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

void disminuirTropas(int& tropas, bool tropaHundida, std::string msj) {
    if(tropaHundida) {
        --tropas;
        printf(msj.c_str(), tropas);
    }
}

int hayGanador(int& misTropas, int& oponenteTropas,int jugadorid) {
    int ganador=0;
    if(misTropas == 0){
        switch (jugadorid)
        {
        case 1:
            ganador=2;
            break;
        case 2:
            ganador=1;
        }
        return ganador;
    }
    else if(oponenteTropas == 0){
        switch (jugadorid)
        {
        case 1:
            ganador=1;
            break;
        case 2:
            ganador=2;
        }
        return ganador;
    }
    else
        return 0;
}

#endif