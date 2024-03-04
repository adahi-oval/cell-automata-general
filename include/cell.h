#pragma once

#include <utility> // Para utilizar std::pair
#include <vector>
#include <iostream>
#include "position.h"
#include "positionDim.h"

// Definición del estado de la célula
typedef bool State;

// Definición de la clase Cell
class Cell {
protected:
  const PositionDim<1>& position; // Referencia a la posición de la célula
  State state;              // Estado de la célula

public:
  // Constructor
  Cell(const Position& pos, const State& st) : position(1), state(st) {}

  // Constructor
  Cell(): position(0), state(false) {}

  // Métodos para obtener la posición y el estado de la célula
  const PositionDim<1>& getPosition() const { return position; }
  State getState() const { return state; }

  // Método virtual para actualizar el estado de la célula
  virtual void updateState();

  // Método virtual para mostrar la célula en pantalla
  virtual std::ostream& display(std::ostream&) const = 0;

  friend std::ostream& operator<<(std::ostream&, const Cell&);
};

