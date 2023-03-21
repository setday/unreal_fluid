/***************************************************************
 * Copyright (C) 2023
 *    HSE SPb (Higher school of economics in Saint-Petersburg).
 ***************************************************************/

/* PROJECT   : ultimate_py_project
 * AUTHORS   : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev
 * FILE NAME : CellsDistributor.h
 * PURPOSE   : distribute particles
 *
 * No part of this file may be changed and used without agreement of
 * authors of this project.
 */

#pragma once

#include <unordered_map>
#include <vector>
#include "../Particle.h"

namespace unreal_fluid::physics::fluid {
  class CellsDistributor {
  private:
    uint64_t first;
    uint64_t second;
    std::unordered_map<uint64_t, std::vector<Particle *>> cells;
    std::unordered_map<uint64_t, std::vector<Particle *>>::iterator cell_iterator;
    std::vector<Particle *> big_particles;
    std::vector<math::Vector3<int>> bias{
            {-1, 0, 0}, {-1, -1, 0}, {-1, 1, 0}, {-1, 0, -1}, {-1, 0, 1}, {-1, -1, -1}, {-1, 1, -1}, {-1, -1, 1}, {-1, 1, 1},
            {0, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1}, {0, -1, -1}, {0, 1, -1}, {0, -1, 1}, {0, 1, 1},
            {1, 0, 0}, {1, -1, 0}, {1, 1, 0}, {1, 0, -1}, {1, 0, 1}, {1, -1, -1}, {1, 1, -1}, {1, -1, 1}, {1, 1, 1}
    };

    static uint64_t getId(vec3 position);

  public:
    constexpr static std::pair<Particle *, Particle *> terminator = {nullptr, nullptr};

  public:
    explicit CellsDistributor(std::vector<Particle *> &particles);
    ~CellsDistributor() = default;

    std::vector<Particle *> &getBigParticles();
    std::pair<Particle *, Particle *> nextPair();
  };
} // namespace unreal_fluid::physics::fluid

// end of CellsDistributor.h
