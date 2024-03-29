/***************************************************************
 * Copyright (C) 2023
 *    HSE SPb (Higher school of economics in Saint-Petersburg).
 ***************************************************************/

/* PROJECT   : ultimate_py_project
 * AUTHORS   : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev
 * FILE NAME : AbstractObject.cxx
 * PURPOSE   : Abstract object to store render and physical objects in one
 *
 * No part of this file may be changed and used without agreement of
 * authors of this project.
 */

#include "AbstractObject.h"
#include "../physics/gas/GasContainer2D.h"
#include "../physics/solid/mesh/SolidMesh.h"
#include "../render/components/mesh/presets/Cube.h"
#include "../src/core/render/components/material/MaterialPresets.h"
#include "../src/core/render/components/mesh/presets/Sphere.h"
#include "../src/core/render/components/mesh/presets/Plane.h"

using namespace unreal_fluid;

AbstractObject::AbstractObject(physics::IPhysicalObject *physicalObject, const std::vector<render::RenderObject *> &renderObjects) : physicalObject(physicalObject),
                                                                                                                                     renderObjects(renderObjects) {}

AbstractObject::AbstractObject(physics::fluid::FluidDescriptor descriptor) : physicalObject(new physics::fluid::SimpleFluidContainer(descriptor)) {}

AbstractObject::AbstractObject(physics::IPhysicalObject *physicalObject) : physicalObject(physicalObject) {}

void parseGasContainer2d(physics::IPhysicalObject *container2D, std::vector<render::RenderObject *> &renderObjects) {
  void *data = container2D->getData();
  auto &cells = *static_cast<std::vector<std::vector<physics::gas::GasCell>> *>(data);

  size_t height = cells.size();
  if (height == 0)
    return;

  size_t width = cells.front().size();
  if (width == 0)
    return;

  static std::vector<float> colors(0 * 0 * 3);
  colors.resize(height * width * 3);
  static std::vector<float> amountOfGas(0 * 0 * 3);
  amountOfGas.resize(height * width);

  if (renderObjects.empty()) {
    auto cube = render::mesh::Plane(1.5, 1.5, 1, 1, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0});
    auto renderObject = new render::RenderObject;

    renderObject->material = render::material::Debug();
    renderObject->bakedMesh = std::make_unique<render::mesh::BakedMesh>(&cube);
    renderObject->textures[0] = new unreal_fluid::render::Texture((int)width, (int)height,
                                                                  (std::size_t)3, sizeof(float)); // color
    renderObject->textures[1] = new unreal_fluid::render::Texture((int)width, (int)height,
                                                                  (std::size_t)1, sizeof(float)); // amountOfGas
    renderObject->shaderProgram = render::DefaultShaderManager::GetGasProgram();

    renderObjects.push_back(renderObject);
  }

  for (size_t y = 0; y < height; ++y) {
    size_t yOffset = y * width;
    for (size_t x = 0; x < width; ++x) {
      const auto &cell = cells[y][x];
      size_t arrayPosition = (yOffset + x) * 3;

      colors[arrayPosition + 0] = cell.color.x;
      colors[arrayPosition + 1] = cell.color.y;
      colors[arrayPosition + 2] = cell.color.z;

      amountOfGas[yOffset + x] = cell.amountOfGas / 100.0;
    }
  }

  renderObjects[0]->textures[0]->write(colors.data());
  renderObjects[0]->textures[1]->write(amountOfGas.data());
}

void AbstractObject::parse() {
  auto type = physicalObject->getType();
  void *data = physicalObject->getData();

  switch (type) {
    using namespace physics;
    case IPhysicalObject::Type::SIMPLE_FLUID_CONTAINER: {
      auto &particles = *static_cast<std::vector<fluid::Particle *> *>(data);

      for (size_t pos = 0; pos < particles.size(); ++pos) {

        if (pos >= renderObjects.size()) {
          auto renderObject = new render::RenderObject;

          renderObject->material = render::material::Gold();
          auto r = particles[pos]->radius;
          auto mesh = render::mesh::Sphere(float(r), unsigned(500 * r), unsigned(500 * r));
          renderObject->bakedMesh = std::make_unique<render::mesh::BakedMesh>(&mesh);
          renderObjects.push_back(renderObject);
        }

        renderObjects[pos]
                ->modelMatrix = mat4::translation(particles[pos]->position);
      }
      break;
    }
    case IPhysicalObject::Type::SOLID_SPHERE: {
      auto solidSphere = *static_cast<solid::SolidSphere *>(data);

      if (renderObjects.empty()) {

        auto renderObject = new render::RenderObject;

        renderObject->material = render::material::Bronze();
        auto r = solidSphere.radius;
        auto mesh = render::mesh::Sphere(float(r), unsigned(500 * r), unsigned(500 * r));
        renderObject->bakedMesh = std::make_unique<render::mesh::BakedMesh>(&mesh);
        renderObjects.push_back(renderObject);
      }

      renderObjects.back()
              ->modelMatrix = mat4::translation(solidSphere.position);
      break;
    }
    case IPhysicalObject::Type::SOLID_MESH: {

      auto &triangles = *static_cast<std::vector<solid::Triangle> *>(data);

      for (size_t pos = renderObjects.size(); pos < triangles.size(); ++pos) {
        const auto &triangle = triangles[pos];
        auto renderObject = new render::RenderObject;

        if (pos == 0)
          renderObject->material = render::material::Gold();
        else if (pos == 1)
          renderObject->material = render::material::RedPlastic();
        else
          renderObject->material = render::material::GreenPlastic();
        auto mesh = render::mesh::BasicMesh({triangle.v1, triangle.v2, triangle.v3}, {0, 1, 2});
        renderObject->bakedMesh = std::make_unique<render::mesh::BakedMesh>(&mesh);
        renderObjects.push_back(renderObject);
      }
      break;
    }
    case IPhysicalObject::Type::SOLID_QUBE: {
      break;
    }
    case IPhysicalObject::Type::GAS_CONTAINER_2D: {
      parseGasContainer2d(physicalObject, renderObjects);
      break;
    }
    case IPhysicalObject::Type::DEFAULT: {
      break;
    }
  }
}

[[nodiscard]] std::vector<render::RenderObject *> &AbstractObject::getRenderObjects() {
  return renderObjects;
}

[[nodiscard]] physics::IPhysicalObject *AbstractObject::getPhysicalObject() {
  return physicalObject;
}

// end of AbstractObject.cxx
