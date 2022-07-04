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

int hayGanador(int& misTropas, int& oponenteTropas) {
    if(misTropas == 0)
        return 2;
    else if(oponenteTropas == 0)
        return 1;
    else
        return 0;
}

#endif