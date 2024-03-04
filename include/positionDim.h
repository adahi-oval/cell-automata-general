#pragma once

#include <cstdarg>
#include <exception>
#include <stdexcept>

// Clase de excepción para coordinadas fuera de rango
class ac_exception : public std::exception {
public:
    const char* what() const noexcept override {
        return "Coordenada fuera de rango";
    }
};

// Plantilla de la clase PositionDim
template <int Dim = 2, class Coordinate_t = int>
class PositionDim : public Position {
private:
    Coordinate_t Coordinates[Dim];

public:
    // Constructor con lista variable de parámetros
    PositionDim(int sz, ...) {
        va_list vl;
        va_start(vl, sz);
        for (int d = 0; d < Dim; d++) {
            Coordinates[d] = va_arg(vl, Coordinate_t);
        }
        va_end(vl);
    }

    // Operador de acceso a la i-ésima coordenada
    Coordinate_t operator[](unsigned int index) const override {
        if (index >= Dim) {
            throw ac_exception();
        }
        return Coordinates[index];
    }

    // Método para establecer las coordenadas
    void setCoordinate(unsigned int index, Coordinate_t value) {
        if (index >= Dim) {
            throw ac_exception(); // o manejar el error de alguna otra manera
        }
        Coordinates[index] = value;
    }
};

