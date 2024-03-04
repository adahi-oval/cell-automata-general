#include "src/Lattice/lattice2d.h"
#include "cellLife.h"
#include <fstream>

// Implementación del constructor de Lattice
Lattice2D::Lattice2D(int N, int M) {

  rows = N;
  cols = M;
  popMode = false;

  // Crear las células en memoria dinámica y establecer su estado inicial a "muerta" (false)
  for (int i = 0; i < N; ++i) {
    std::vector<CellLife*> row;
    for (int j = 0; j < M; ++j) {
      row.push_back(new CellLife(PositionDim<2>(2,i,j), false));
    }
    cells_.push_back(row);
  }

  // Solicitar por teclado las posiciones de las células vivas en la configuración inicial
  askForLiveCells();
}

// Constructor por archivo
Lattice2D::Lattice2D(const char* filename) {

  popMode = false;

  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
    return;
  }

  // Leer las dimensiones de la retícula del archivo
  file >> rows >> cols;
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorar el resto de la línea para mover el puntero al inicio de la próxima línea

  // Reservar espacio para las células
  cells_.resize(rows, std::vector<CellLife*>(cols));

  // Leer las cadenas de caracteres del archivo para inicializar las células
  for (int i = 0; i < rows; ++i) {
    std::string rowString;
    std::getline(file, rowString); // Leer la línea completa, incluidos los espacios en blanco

    // Verificar que la longitud de la cadena sea igual al número de columnas
    if (rowString.length() != cols) {
      std::cerr << "Error: La longitud de la fila no coincide con el número de columnas especificado." << std::endl;
      return;
    }

    for (int j = 0; j < cols; ++j) {
      // Crear una célula viva si el carácter es 'X', de lo contrario, crear una célula muerta
      bool isAlive = (rowString[j] == 'X');
      cells_[i][j] = new CellLife(PositionDim<2>(2,i,j), isAlive);
    }
  }

  // Cerrar el archivo
  file.close();
}

Lattice2D::Lattice2D(int once) {
  
  rows = 1;
  cols = 1;
  popMode = false;

}

// Destructor de Lattice
Lattice2D::~Lattice2D() {
  // Liberar la memoria de las células
  for (auto& row : cells_) {
    for (auto& CellLife : row) {
      delete CellLife;
    }
  }
}

int Lattice2D::getRows() const {
  return rows;
}

int Lattice2D::getCols() const {
  return cols;
}

// getter frontera
std::string Lattice2D::getFrontera() const {
    return frontera_;
}

// setter frontera
void Lattice2D::setFrontera(const std::string& frontera) {
    frontera_ = frontera;
}

// getter popmode
bool Lattice2D::getPopMode() const {
  return popMode;
}

// setter popmode
void Lattice2D::setPopMode(bool b) {
  popMode = b;
}

// Método privado para solicitar por teclado las posiciones de las células vivas en la configuración inicial
void Lattice2D::askForLiveCells() {
  char choice = 's';
  std::cout << "Ingrese las posiciones de las células vivas (fila columna):" << std::endl;
  
  while (choice == 's') {
    int row, col;
    std::cout << "Fila: ";
    std::cin >> row;
    std::cout << "Columna: ";
    std::cin >> col;

    if (row >= 0 && row < rows && col >= 0 && col < cols) {
      // Establecer el estado de la célula en vivo (true)
      cells_[row][col]->setState(true);
    } else {
      std::cout << "Posición inválida. Por favor, ingrese una posición dentro del retículo." << std::endl;
    }

    std::cout << "¿Desea añadir otra célula viva? (s/n): ";
    std::cin >> choice;
    if (choice == 's')
    {
      continue;
    } else if (choice == 'n')
    {
      break;
    } else
    {
      std::cout << "Opción inválida. Saliendo del ingreso de células vivas." << std::endl;
      break;
    }
  }
}

// Implementación del método para calcular la población actual (número de células vivas)
std::size_t Lattice2D::Population() const {
  std::size_t aliveCount = 0;
  for (const auto& row : cells_) {
    for (const auto& CellLife : row) {
      if (CellLife->getState()) { // Si el estado de la célula es verdadero (viva)
        ++aliveCount;
      }
    }
  }
  return aliveCount;
}

// Sobrecarga del operador [] para acceder a las células por su posición en el retículo
CellLife& Lattice2D::operator[](const PositionDim<2>& pos) const {
  // Obtener las coordenadas de la posición
  int x = pos[0];
  int y = pos[1];

  // Verificar que las coordenadas estén dentro de los límites del retículo
  if (x >= 0 && x < rows && y >= 0 && y < cols) {
    // Devolver la referencia a la célula en la posición dada
    return *cells_[x][y];
  } else {
    // Si las coordenadas están fuera de los límites, lanzar una excepción o devolver una referencia nula
    // Aquí se elige lanzar una excepción
    return *this->deadCell;
  }
}

// Metodo para actualizar las posiciones
void Lattice2D::updatePositions() {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      this->cells_[i][j]->setPosition(i,j);
    }
  }
}

void Lattice2D::updateStates() {
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      this->cells_[i][j]->updateState();
    }
  }
}

// Condicion abierta, temp es si caliente o fria (true o false)
void Lattice2D::openFrontier(const bool temp) {
  PositionDim<2> pos(2,0,0);
  // Añadir columnas a los lados con células en el estado dado
  for (int i = 0; i < rows; ++i) {
    CellLife* auxCellLife1 = new CellLife(pos, temp);
    CellLife* auxCellLife2 = new CellLife(pos, temp);
    // Izquierda
    cells_[i].insert(cells_[i].begin(), auxCellLife1);
    // Derecha
    cells_[i].push_back(auxCellLife2);
  }
  cols += 2; // Se añaden dos columnas nuevas

  // Añadir filas arriba y abajo con células en el estado dado
  // Fila arriba
  std::vector<CellLife*> upRow;
  for (int i = 0; i < cols; i++)
  {
    upRow.push_back(new CellLife(PositionDim<2>(2,0,0), temp));
  }
  cells_.insert(cells_.begin(), upRow);
  
  // Fila abajo
  std::vector<CellLife*> downRow;
  for (int i = 0; i < cols; i++)
  {
    downRow.push_back(new CellLife(PositionDim<2>(2,0,0), temp));
  }
  cells_.push_back(downRow);
  rows += 2; // Se añaden dos filas nuevas

  // Ajustar las posiciones de las células existentes
  this->updatePositions();

}

// Condicion de frontera periodica
void Lattice2D::periodicFrontier() {
  // Expansión de la frontera periódica
  std::vector<std::vector<CellLife*>> tempCellLifes = cells_; // Variable temporal para almacenar las células originales

  // Expansión hacia los lados
  for (int i = 0; i < rows; ++i) {
    // Izquierda
    cells_[i].insert(cells_[i].begin(), new CellLife(PositionDim<2>(2,0,0), tempCellLifes[i][cols - 1]->getState()));
    // Derecha
    cells_[i].push_back(new CellLife(PositionDim<2>(2,0,0),tempCellLifes[i][0]->getState()));
  }

  std::vector<CellLife*> upRow;
  for (int i = 0; i < cols; i++)
  {
    upRow.push_back(new CellLife(PositionDim<2>(2,0,0), tempCellLifes[0][i]->getState()));
  }
  cells_.insert(cells_.begin(), upRow);
  
  // Fila abajo
  std::vector<CellLife*> downRow;
  for (int i = 0; i < cols; i++)
  {
    downRow.push_back(new CellLife(PositionDim<2>(2,0,0), tempCellLifes[rows - 1][i]->getState()));
  }
  cells_.push_back(downRow);

  // Esquinas
  // arriba izquierda
  cells_[0].insert(cells_[0].begin(), new CellLife(PositionDim<2>(2,0,0), tempCellLifes[rows - 1][cols - 1]->getState()));
  // arriba derecha
  cells_[0].push_back(new CellLife(PositionDim<2>(2,0,0), tempCellLifes[rows - 1][0]->getState()));
  
  rows += 2;
  // abajo derecha
  cells_[rows - 1].push_back(new CellLife(PositionDim<2>(2,0,0), tempCellLifes[0][0]->getState()));
  // abajo izquierda
  cells_[rows - 1].insert(cells_[rows - 1].begin(), new CellLife(PositionDim<2>(2,0,0), tempCellLifes[0][cols - 1]->getState()));

  cols += 2;

  // Ajustar las posiciones de las células existentes
  this->updatePositions();
}

// Expand lattice para sin frontera
void Lattice2D::noFrontier(int row, int col) {

  // Expandir el retículo creando una nueva fila o columna con células muertas en la dirección correspondiente
  
  // Esquinas
  if (row == 0 && col == 0) {
    // Esquina superior izquierda
    // Insertar una nueva columna en el lado izquierdo
    for (int i = 0; i < rows; ++i) {
      cells_[i].insert(cells_[i].begin(), new CellLife(PositionDim<2>(2,i,0), false)); // Agregar al principio
    }
    ++cols;

    // Insertar una nueva fila en la parte superior
    cells_.insert(cells_.begin(), std::vector<CellLife*>(cols, new CellLife(PositionDim<2>(2,rows,cols), false))); // Agregar al principio
    ++rows;

    // Ajustar las posiciones de todas las células existentes en las columnas
    this->updatePositions();

  } else if (row == 0 && col == cols - 1) {
    // Esquina superior derecha
    // Columna por la parte derecha
    for (int i = 0; i < rows; ++i) {
      cells_[i].push_back(new CellLife(PositionDim<2>(0, 0), false)); // Agregar al final
    }
    ++cols;

    // Insertar una nueva fila en la parte superior
    cells_.insert(cells_.begin(), std::vector<CellLife*>(cols, new CellLife(PositionDim<2>(2,rows,cols), false))); // Agregar al principio
    ++rows;

    // Ajustar las posiciones de todas las células existentes en las filas
    this->updatePositions();

  } else if (row == rows - 1 && col == 0) {
    // Esquina inferior izquierda
    // Insertar una nueva columna en el lado izquierdo
    for (int i = 0; i < rows; ++i) {
      cells_[i].insert(cells_[i].begin(), new CellLife(PositionDim<2>(i, 0), false)); // Agregar al principio
    }
    ++cols;
    
    // Insertar una nueva fila en la parte inferior
    cells_.push_back(std::vector<CellLife*>(cols, new CellLife(PositionDim<2>(rows, 0), false)));
    ++rows;

    // Ajustar las posiciones de todas las células existentes en las columnas
    this->updatePositions();

  } else if (row == rows - 1 && col == cols - 1) {
    // Esquina inferior derecha

    // Columna por la parte derecha
    for (int i = 0; i < rows; ++i) {
      cells_[i].push_back(new CellLife(PositionDim<2>(0, 0), false)); // Agregar al final
    }
    ++cols;

    // fila parte inferior
    cells_.push_back(std::vector<CellLife*>(cols, new CellLife(PositionDim<2>(rows, cols), false)));
    ++rows;

    // Actualizar posiciones
    this->updatePositions();

  }
  else if (row == 0) {
    // Expansión hacia arriba
    // Insertar una nueva fila de células muertas en la parte superior
    cells_.insert(cells_.begin(), std::vector<CellLife*>(cols, new CellLife(PositionDim<2>(0, 0), false)));
    ++rows;

    // Ajustar las posiciones de todas las células existentes en las filas
    this->updatePositions();

  } else if (row == rows - 1) {
    // Expansión hacia abajo
    // Insertar una nueva fila de células muertas en la parte inferior
    cells_.push_back(std::vector<CellLife*>(cols, new CellLife(PositionDim<2>(rows, 0), false)));
    ++rows;
    
    // Actualizar posicion
    this->updatePositions();

  } 
  else if (col == 0) {
    // Expansión hacia la izquierda
    // Insertar una nueva columna de células muertas en el lado izquierdo
    for (int i = 0; i < rows; ++i) {
      cells_[i].insert(cells_[i].begin(), new CellLife(PositionDim<2>(i, 0), false)); // Agregar al principio
    }
    ++cols;

    // Ajustar las posiciones de todas las células existentes en las columnas
    this->updatePositions();

  } else if (col == cols - 1) {
    // Expansión hacia la derecha
    // Insertar una nueva columna de células muertas en el lado derecho
    for (int i = 0; i < rows; ++i) {
      cells_[i].push_back(new CellLife(PositionDim<2>(i, cols), false)); // Agregar al final
    }
    ++cols;

    this->updatePositions();

  }
}

// Restaurar tamaño original, para print
void Lattice2D::removeBorders() {
  // Eliminar las columnas adicionales en los lados
  for (int i = 0; i < rows; ++i) {
    cells_[i].erase(cells_[i].begin()); // Eliminar la primera célula en la fila (lado izquierdo)
    cells_[i].pop_back(); // Eliminar la última célula en la fila (lado derecho)
  }
  cols -= 2; // Se eliminan dos columnas

  // Eliminar las filas adicionales arriba y abajo
  cells_.erase(cells_.begin()); // Eliminar la primera fila (arriba)
  cells_.pop_back(); // Eliminar la última fila (abajo)
  rows -= 2; // Se eliminan dos filas

  // Restaurar las posiciones de las células existentes
  this->updatePositions();
}


// Calculo de la siguiente generación
void Lattice2D::nextGeneration() {
  if (this->getFrontera() == "abiertaFria")
  {

    this->openFrontier(false); // expande el lattice con celulas tipo false
    for (int i = 1; i < this->getRows() - 1; i++)
    {
      for (int j = 1; j < this->getCols() - 1; j++)
      {
        std::vector<CellLife> neighbors = this->cells_[i][j]->getNeighbors(*this); // vecinos de cada celula
        this->cells_[i][j]->nextState(*this);
      }
    }
    this->updateStates();
    this->removeBorders(); // volver al tamaño original
    
    
  } else if (this->getFrontera() == "abiertaCaliente")
  {
    this->openFrontier(true); // expande el lattice con celulas tipo true
    for (int i = 1; i < this->getRows() - 1; i++)
    {
      for (int j = 1; j < this->getCols() - 1; j++)
      {
        std::vector<CellLife> neighbors = this->cells_[i][j]->getNeighbors(*this); // vecinos de cada celula
        this->cells_[i][j]->nextState(*this);
      }
    }
    this->updateStates();
    this->removeBorders(); // volver al tamaño original
    

  } else if (this->getFrontera() == "periodic")
  {

    this->periodicFrontier(); // expande el lattice con frontera periodica
    for (int i = 1; i < this->getRows() - 1; i++)
    {
      for (int j = 1; j < this->getCols() - 1; j++)
      {
        std::vector<CellLife> neighbors = this->cells_[i][j]->getNeighbors(*this); // vecinos de cada celula
        this->cells_[i][j]->nextState(*this);
      }
    }
    this->updateStates();
    this->removeBorders(); // volver al tamaño original
    

  } else if (this->getFrontera() == "noBorder") {

    for (int i = 0; i < this->getRows(); i++)
    {
      for (int j = 0; j < this->getCols(); j++)
      {
        std::vector<CellLife> neighbors = this->cells_[i][j]->getNeighbors(*this); // vecinos de cada celula
        this->cells_[i][j]->nextState(*this);
      }
    }
    this->updateStates();
    for (int i = 0; i < this->getRows(); i++)
    {
      for (int j = 0; j < this->getCols(); j++)
      {
        if (this->cells_[i][j]->getState())
        {
          this->noFrontier(i,j);
        }
      }
    }
    
    
  }
  if (this->getPopMode())
  {
    std::cout << "Número de células vivas: " << this->Population() << std::endl << std::endl;
  } else
  {
    std::cout << *this << std::endl << std::endl; // sacar por pantalla
  }
  
}

// sobrecarga operador<<
std::ostream& operator<<(std::ostream& os, const Lattice2D& lattice) {
  for (int i = 0; i < lattice.getRows(); i++)
  {
    for (int j = 0; j < lattice.getCols(); j++)
    {
      os << lattice[PositionDim<2>(i,j)];
    }
    os << std::endl;
  }

  return os;
}

// guardar en archivo
void Lattice2D::saveToFile(const char* filename) const {
  std::ofstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
    return;
  }

  // Escribir las dimensiones del tablero
  file << rows << " " << cols << std::endl;

  // Escribir el estado de cada celda en el tablero
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      file << (cells_[i][j]->getState() ? 'X' : ' ');
    }
    file << std::endl;
  }

  // Cerrar el archivo
  file.close();
}

Lattice2D& Lattice2D::operator=(const Lattice2D& other) {
    if (this == &other) {
        return *this;
    }

    // Copiar las dimensiones y el modo de población
    rows = other.rows;
    cols = other.cols;
    popMode = other.popMode;
    frontera_ = other.frontera_;
    std::vector<CellLife*> CellLifes;

    // Copiar el estado de las células
    for (int i = 0; i < rows; ++i) {
        cells_.push_back(CellLifes);
        for (int j = 0; j < cols; ++j) {
            cells_[i].push_back(new CellLife(*other.cells_[i][j]));
        }
    }

    return *this;
}
