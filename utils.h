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

void disminuirOponenteTropas(int& tropas, bool tropaHundida) {
    if(tropaHundida) {
        --tropas;
        printf("¡Tropa del oponente hundida! Tropas restantes: %i", tropas);
    }
}

bool hayGanador(int& tropas) {
    return tropas == 0;
}

#endif