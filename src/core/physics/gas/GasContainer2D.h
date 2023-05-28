/***************************************************************
 * Copyright (C) 2023
 *    HSE SPb (Higher school of economics in Saint-Petersburg).
 ***************************************************************/

/* PROJECT   : ultimate_py_project
 * AUTHORS   : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev
 * FILE NAME : GasContainer2D.h
 * PURPOSE   : test 2-dimensional gas class
 *
 * No part of this file may be changed and used without agreement of
 * authors of this project.
 */

#pragma once

#include "../Simulator.h"

namespace unreal_fluid::physics::gas {

  class GasCell {
  public:
    double amountOfGas = 10;  // = mass / molarMass
    double volume = 1;        // volume of cell (now 1x1x1)
    double temperature = 300; // temperature of gas
    double targetFlow = 0;
    double particlesFlow = 0;
    vec3f color = {1, 1, 1}; // color is a quantity to define gas

    GasCell() = default;
    explicit GasCell(double pressure, vec3f color = {1, 1, 1}, double temperature = 300);

    double getPressure() const;

    /// @brief slice gas cell
    /// @param particlesCount amount of gas to slice
    /// @return new gas cell with sliced gas
    GasCell slice(double particlesCount);
    /// @brief add gas cell to current
    /// @param cell gas cell to add
    void add(GasCell cell);
  };

  class GasContainer2d : public IPhysicalObject {

    int height, width;
    std::vector<std::vector<GasCell>> storage, temporary;

  public:
    GasContainer2d(int height, int width, int particle_number);

    void increase(GasCell &cell1, GasCell &cell2);

    void simulate(GasCell &cell1, GasCell &cell2, double targetFlow);

    void recalc(double dt);

    void diffuse();

    void diffuse(GasCell &cell1, GasCell &cell2);

    void moveFlow(double dt);

    /* abstract class implementation */

    Type getType() override;
    void *getData() override;

  private:
    void simulate(double dt) override;
  };

} // namespace unreal_fluid::physics::gas

// end of GasContainer2D.h