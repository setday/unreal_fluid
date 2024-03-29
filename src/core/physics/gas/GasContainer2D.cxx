/***************************************************************
 * Copyright (C) 2023
 *    UnrealFluid Team (https://github.com/setday/unreal_fluid) and
 *    HSE SPb (Higher school of economics in Saint-Petersburg).
 ***************************************************************/

/* PROJECT                 : UnrealFluid
 * AUTHORS OF THIS PROJECT : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev
 * FILE NAME               : GasContainer2D.cxx
 * FILE AUTHORS            : Daniil Martsenyuk, Serkov Alexander (co-author).
 * PURPOSE                 : test 2-dimensional gas class
 *
 * No part of this file may be changed and used without
 * agreement of authors of this project.
 */

#include "GasContainer2D.h"

using namespace unreal_fluid::physics::gas;

GasContainer2d::GasContainer2d(int height, int width, int particle_number) : _height(height),
                                                                             _width(width) {
  _storage.resize(height, std::vector<GasCell>(width));
  _flowsX.resize(height, std::vector<double>(width + 1));
  _flowsY.resize(height + 1, std::vector<double>(width));

  for (int counter = 0; counter < particle_number; ++counter) {
    int x = rand() % height, y = rand() % width;

    _storage[x][y] = GasCell(rand() % 100, vec3f(rand() % 100, rand() % 100, rand() % 100) / 100.0);
  }
}

double GasContainer2d::calculateFlow(const GasCell &cell1, const GasCell &cell2) const {
  double pressureDiff = cell1.getPressure() - cell2.getPressure();
  double targetFlow = pressureDiff / 10.0;
  return targetFlow;
}

void GasContainer2d::calculateAndSaveFlow(GasCell &cell1, GasCell &cell2, int y, int x, bool isHorizontal) {
  double targetFlow = calculateFlow(cell1, cell2);
  if (isHorizontal) _flowsX[y][x] += targetFlow;
  else
    _flowsY[y][x] += targetFlow;
}

void GasContainer2d::calculateFlows(double dt) {
  for (int row = 0; row < _height; ++row) {
    for (int column = 0; column < _width; ++column) {
      auto &cell = _storage[row][column];

      if (row < _height - 1) calculateAndSaveFlow(cell, _storage[row + 1][column], row, column, false);
      if (column < _width - 1) calculateAndSaveFlow(cell, _storage[row][column + 1], row, column, true);
    }
  }
}

void GasContainer2d::applyFlow(GasCell &cell1, GasCell &cell2, double targetFlow) const {
  if (targetFlow > 0) cell2.add(cell1.slice(targetFlow));
  else
    cell1.add(cell2.slice(-targetFlow));
}

void GasContainer2d::applyFlows(double dt) {
  for (size_t row = 0; row < _height; ++row) {
    for (size_t column = 0; column < _width; ++column) {
      auto &cell = _storage[row][column];

      if (row < _height - 1) {
        double targetFlow = _flowsY[row][column];
        applyFlow(cell, _storage[row + 1][column], targetFlow);
      }
      if (column < _width - 1) {
        double targetFlow = _flowsX[row][column];
        applyFlow(cell, _storage[row][column + 1], targetFlow);
      }
    }
  }
}

void GasContainer2d::diffuseTwoCells(GasCell &cell1, GasCell &cell2, double dt) {
  double slicingPart = dt * std::min(cell1.amountOfGas, cell2.amountOfGas);

  auto cell_1 = cell1.slice(slicingPart);
  auto cell_2 = cell2.slice(slicingPart);

  cell1.add(cell_2);
  cell2.add(cell_1);
}

void GasContainer2d::diffuseCells(double dt) {
  for (size_t row = 0; row < _height; ++row) {
    for (size_t column = 0; column < _width; ++column) {
      auto &cell = _storage[row][column];
      if (row < _height - 1) diffuseTwoCells(cell, _storage[row + 1][column], dt);
      if (column < _width - 1) diffuseTwoCells(cell, _storage[row][column + 1], dt);
    }
  }
}

void GasContainer2d::dissolveCell(GasCell &cell, double dt) {
  cell.amountOfGas -= dt * cell.amountOfGas;
  cell.amountOfGas = std::max(cell.amountOfGas, 0.0);
}

void GasContainer2d::dissolveCells(double dt) {
  int edgeSize = 3;

  for (size_t row = 0; row < _height; ++row) {
    for (size_t column = 0; column < _width; ++column) {
      if (row < edgeSize || row >= _height - edgeSize || column < edgeSize || column >= _width - edgeSize) {
        auto &cell = _storage[row][column];
        dissolveCell(cell, dt);
      }
    }
  }
}

unreal_fluid::physics::IPhysicalObject::Type GasContainer2d::getType() {
  return Type::GAS_CONTAINER_2D;
}

void *GasContainer2d::getData() {
  return &_storage;
}

void GasContainer2d::simulate(double dt) {
  calculateFlows(dt);
  applyFlows(dt);
  diffuseCells(dt);
  dissolveCells(dt);
  advect(dt);
}

void GasContainer2d::advect(double dt) {
  for (int counter = 0; counter < 100; ++counter) {
    if (rand() & 1) continue;
    int x = rand() % _height, y = rand() % _width;
    auto cell = _storage[x][y];
    cell.amountOfGas += 100 * dt * cell.amountOfGas;
  }
}

// End of GasContainer2D.cxx
