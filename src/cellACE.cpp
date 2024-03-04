#pragma once

#include "cellACE.h"
#include "positionDim.h"
#include "lattice1d.h"

// Constructor de la célula
CellACE::CellACE(const PositionDim<1>& position, const State& initialState) {
  position_ = position;
  state_ = initialState;
}

CellACE::CellACE() {
  position_ = 0;
  state_ = false;
}

// Obtener el estado de la célula
State CellACE::getState() const {
  return state_;
}

// Obtener el estado siguiente de la célula
State CellACE::getNextState() const {
  return nextState_;
}

// Establecer el estado de la célula
void CellACE::setState(State newState) {
  state_ = newState;
}

// Establecer nextState_
void CellACE::setNextState(State newState) {
  nextState_ = newState;
}

std::vector<State> CellACE::getNeighbors(const Lattice1D& lattice) const {
  std::vector<State> neighbors;

  if (position_[0] > 0 && position_[0] < lattice.getSize() - 1) { // Verificar si la posición actual no es la primera
    neighbors.push_back(lattice.getCell(position_[0] - 1).getState());
    neighbors.push_back(lattice.getCell(position_[0] + 1).getState());
  } 
  
  else if(position_[0] == 0) { // Caso para la primera Célula

    if(lattice.getFrontera() == "abiertaFria") { 
      neighbors.push_back(false);
      neighbors.push_back(lattice.getCell(position_[0] + 1).getState());
    }  

    else if(lattice.getFrontera() == "abiertaCaliente") { 
      neighbors.push_back(true);
      neighbors.push_back(lattice.getCell(position_[0] + 1).getState());
    }

    else if(lattice.getFrontera() == "periodic") {
      neighbors.push_back(lattice.getCell(lattice.getSize() - 1).getState()); // última posición
      neighbors.push_back(lattice.getCell(position_[0] + 1).getState());
    }

    else { throw std::invalid_argument("Tipo de frontera no válido"); }
  } 
  
  else if(position_[0] == lattice.getSize() - 1) { // Caso para última célula

    if(lattice.getFrontera() == "abiertaFria") { 
      neighbors.push_back(lattice.getCell(position_[0] - 1).getState());
      neighbors.push_back(false); 
    }

    else if(lattice.getFrontera() == "abiertaCaliente") { 
      neighbors.push_back(lattice.getCell(position_[0] - 1).getState());
      neighbors.push_back(true); 
    }

    else if(lattice.getFrontera() == "periodic") {
      neighbors.push_back(lattice.getCell(position_[0] - 1).getState());
      neighbors.push_back(lattice.getCell(0).getState()); // primera posición
    }

    else { throw std::invalid_argument("Tipo de frontera no válido"); }
  }

  return neighbors;
}


// Función de transición, para next State usando la regla 110
const State CellACE::transitionFunction(const State& C, const State& L, const State& R) const {
  State result = C + R + C * R + L * C * R;
  return result;
}

// Calcular el siguiente estado de la célula
void CellACE::nextState(const Lattice1D& lattice) {
  std::vector<State> vecinos = this->getNeighbors(lattice);

  State C = this->getState();
  State L = vecinos[0];
  State R = vecinos[1];

  this->nextState_ = transitionFunction(C, L, R);
}

// Actualizar estado
void CellACE::updateState() {
  this->setState(this->getNextState());
}

// Sobrescribir el método display
std::ostream& CellACE::display(std::ostream& os) const {
  // Implementación específica de CellACE para mostrar la célula en pantalla
  os << (this->getState() ? 'X' : ' '); //'X' para células vivas y ' ' para células muertas
  return os;
}