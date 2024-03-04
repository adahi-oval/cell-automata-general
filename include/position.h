#pragma once

#include <vector>

typedef int Coor_t;

class Position {
public:
    // Operador de acceso a la i-ésima coordenada
    virtual Coor_t operator[](unsigned int) const = 0;
};
