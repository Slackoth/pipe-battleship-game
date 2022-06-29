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

#endif