#pragma once

#include <utility> // Para utilizar std::pair
#include <vector>
#include <iostream>
#include "position.h"
#include "positionDim.h"
#include "cell.h"
#include "lattice1d.h"

// Declaración de la clase Cell
class CellACE: public Cell {
public:
  // Constructor
  CellACE(const PositionDim<1>& position, const State& initialState = false);

  // Constructor por defecto
  CellACE();

  // Métodos
  State getState() const; // Método para obtener el estado de la célula
  State getNextState() const;
  void setState(State newState); // Método para establecer el estado de la célula
  void setNextState(State newState);
  void nextState(const Lattice1D& lattice); // Método para calcular el siguiente estado de la célula
  void updateState(); // Método para actualizar el estado de la célula
  const State transitionFunction(const State& C, const State& L, const State& R) const;
  std::vector<State> getNeighbors(const Lattice1D& lattice) const;

  std::ostream& CellACE::display(std::ostream& os) const override;
  void CellACE::nextState(const Lattice1D& lattice) const;

private:
  PositionDim<1> position_; // Posición dentro del retículo
  State state_;       // Estado actual de la célula
  State nextState_;   // Estado siguiente de la célula
};

