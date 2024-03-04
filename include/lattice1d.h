#pragma once

#include <vector>
#include <iostream>
#include "cellACE.h"

// Declaración adelantada de la clase Cell
class CellACE;

// Declaración de la clase Lattice
class Lattice1D {
public:
  // Constructor
  Lattice1D(int size, std::string frontera);

  // Destructor
  ~Lattice1D();

  int getSize() const;
  std::string getFrontera() const;

  // Método para obtener una referencia constante a una célula en una posición específica
  const Cell& getCell(const PositionDim<1>& position) const;

  // Método para cargar la configuración inicial del autómata celular
  void loadInitialConfiguration(std::vector<char>);
  void nextGeneration();

  // Sobrecarga del operador <<
  friend std::ostream& operator<<(std::ostream&, const Lattice1D&);

private:
  std::vector<CellACE*> cells_;
  int size_;
  std::string frontera_;
};
