#pragma once

#include <utility> // Para utilizar std::pair
#include <vector>
#include <iostream>
#include "cell.h"
#include "positionDim.h"
#include "lattice2d.h"

// Definición del tipo de dato para la posición
using Position = std::pair<int, int>;
// Definición del tipo de dato para el estado
using State = bool;

// Declaracion adelantada de la clase Lattice
class Lattice;

// Definición de la clase Cell
class CellLife: public Cell {
public:

  // Constructor de la célula
  CellLife(const PositionDim<2>& pos, const State& state);

  // getter y setter de estado
  const State getState() const;
  void setState(State newState);

  // getter y setter de siguiente estado
  State getNextState() const;
  void setNextState(State newState);

  // updater
  void updateState();

  // getter y setter de posicion
  const PositionDim<2> getPosition() const;
  void setPosition(int row, int col);

  // vecindad
  std::vector<CellLife> getNeighbors(Lattice2D& lattice);

  // funcion de transicion
  State transitionFunction(std::vector<Cell> neighbors);

  // Sobrecarga de display
  std::ostream& CellLife::display(std::ostream&) const override;

  void nextState(Lattice2D&);

private:
  PositionDim<2> position_; // Posición de la célula en el retículo
  State state_;       // Estado de la célula
  State nextState_;
};
