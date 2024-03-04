#include "cellLife.h"
#include "lattice2d.h"

CellLife::CellLife(const PositionDim<2>& pos, const State& state) {
  position_ = pos;
  state_ = state;
  nextState_ = false;
}

const State CellLife::getState() const {
  return state_;
}

void CellLife::setState(State newState) {
  state_ = newState;
}

State CellLife::getNextState() const {
  return nextState_;
}

void CellLife::setNextState(State newState) {
  nextState_ = newState;
}

void CellLife::updateState() {
  state_ = nextState_;
}

const PositionDim<2> CellLife::getPosition() const {
  return position_;
}

void CellLife::setPosition(int row, int col) {
  position_.setCoordinate(0, row);
  position_.setCoordinate(1, col);
}

// Vecindad
std::vector<CellLife> CellLife::getNeighbors(Lattice2D& lattice) {
  
  lattice.updatePositions();
  std::vector<CellLife> neighbors;
  PositionDim<2> pos(this->getPosition());

  if (lattice.getFrontera() != "noBorder")
  {
    // Conocer los estados de su vecindad, en sentido horario empezando por la izquierda
    neighbors.push_back(lattice[PositionDim<2>(2,pos[0], pos[1] - 1)]); // izquierda
    neighbors.push_back(lattice[PositionDim<2>(2,pos[0] - 1, pos[1] - 1)]); // arriba izquierda
    neighbors.push_back(lattice[PositionDim<2>(2,pos[0] - 1, pos[1])]); // arriba
    neighbors.push_back(lattice[PositionDim<2>(2,pos[0] - 1, pos[1] + 1)]); // arriba derecha
    neighbors.push_back(lattice[PositionDim<2>(2,pos[0], pos[1] + 1)]); // derecha
    neighbors.push_back(lattice[PositionDim<2>(2,pos[0] + 1, pos[1] + 1)]); // abajo derecha
    neighbors.push_back(lattice[PositionDim<2>(2,pos[0] + 1, pos[1])]); // abajo
    neighbors.push_back(lattice[PositionDim<2>(2,pos[0] + 1, pos[1] - 1)]); // abajo izquierda
  } else
  {
    if (pos[0] == 0 && pos[1] == 0) // Esquina superior izq
    {

      neighbors.push_back(lattice[PositionDim<2>(2,pos[0], pos[1] + 1)]); // derecha
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] + 1, pos[1] + 1)]); // abajo derecha
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] + 1, pos[1])]); // abajo

    } else if (pos[0] == 0 && pos[1] == lattice.getCols() - 1) // Esquina superior derecha
    {

      neighbors.push_back(lattice[PositionDim<2>(2,pos[0], pos[1] - 1)]); // izquierda
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] + 1, pos[1])]); // abajo
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] + 1, pos[1] - 1)]); // abajo izquierda

    } else if (pos[0] == lattice.getRows() - 1 && pos[1] == lattice.getCols() - 1) // Esquina inferior derecha
    {
      
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0], pos[1] - 1)]); // izquierda
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] - 1, pos[1] - 1)]); // arriba izquierda
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] - 1, pos[1])]); // arriba

    } else if (pos[0] == lattice.getRows() - 1 && pos[1] == 0) // Esquina inferior izquierda
    {
      
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] - 1, pos[1])]); // arriba
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] - 1, pos[1] + 1)]); // arriba derecha
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0], pos[1] + 1)]); // derecha

    } else if (pos[0] == 0) // Borde superior
    {

      neighbors.push_back(lattice[PositionDim<2>(2,pos[0], pos[1] - 1)]); // izquierda
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0], pos[1] + 1)]); // derecha
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] + 1, pos[1] + 1)]); // abajo derecha
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] + 1, pos[1])]); // abajo
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] + 1, pos[1] - 1)]); // abajo izquierda

    } else if (pos[1] == lattice.getCols() - 1) // Borde derecho
    {
      
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] + 1, pos[1])]); // abajo
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] + 1, pos[1] - 1)]); // abajo izquierda
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0], pos[1] - 1)]); // izquierda
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] - 1, pos[1] - 1)]); // arriba izquierda
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] - 1, pos[1])]); // arriba

    } else if (pos[0] == lattice.getRows() - 1) // Borde inferior
    {

      neighbors.push_back(lattice[PositionDim<2>(2,pos[0], pos[1] - 1)]); // izquierda
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] - 1, pos[1] - 1)]); // arriba izquierda
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] - 1, pos[1])]); // arriba
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] - 1, pos[1] + 1)]); // arriba derecha
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0], pos[1] + 1)]); // derecha

    } else if (pos[1] == 0) // Borde izquierdo
    {
      
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] - 1, pos[1])]); // arriba
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] - 1, pos[1] + 1)]); // arriba derecha
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0], pos[1] + 1)]); // derecha
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] + 1, pos[1] + 1)]); // abajo derecha
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] + 1, pos[1])]); // abajo

    } else
    {
      // Conocer los estados de su vecindad, en sentido horario empezando por la izquierda
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0], pos[1] - 1)]); // izquierda
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] - 1, pos[1] - 1)]); // arriba izquierda
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] - 1, pos[1])]); // arriba
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] - 1, pos[1] + 1)]); // arriba derecha
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0], pos[1] + 1)]); // derecha
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] + 1, pos[1] + 1)]); // abajo derecha
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] + 1, pos[1])]); // abajo
      neighbors.push_back(lattice[PositionDim<2>(2,pos[0] + 1, pos[1] - 1)]); // abajo izquierda
    }
  }
  
  
  
  return neighbors;
}

// Funcion de transicion
void CellLife::nextState(Lattice2D& lattice) {
  // Contar células vivas en los vecinos
  int aliveCount = 0;

  std::vector<CellLife> neighbors = this->getNeighbors(lattice);

  for (int i = 0; i < neighbors.size(); i++)
  {
    if(neighbors[i].getState()){
      aliveCount++;
    }
  }
  
  // Lógica de la funcion de transicion
  if (this->getState())
  {
    if (aliveCount == 2 || aliveCount == 3)
    {
      this->nextState_ = true;
    } else
    {
      this->nextState_ = false;
    }
  } else
  {
    if (aliveCount == 3)
    {
      this->nextState_ = true;
    } else
    {
      this->nextState_ = false;
    } 
  }
}

// Sobrescribir el método display
std::ostream& CellLife::display(std::ostream& os) const {
  // Implementación específica de CellACE para mostrar la célula en pantalla
  os << (this->getState() ? 'X' : ' '); //'X' para células vivas y ' ' para células muertas
  return os;
}