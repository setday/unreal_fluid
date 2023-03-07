/***************************************************************
 * Copyright (C) 2023
 *    HSE SPb (Higher school of economics in Saint-Petersburg).
 ***************************************************************/

/* PROJECT   : ultimate_py_project
 * AUTHORS   : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev
 * FILE NAME : GLTestScene.cxx
 *
 * No part of this file may be changed and used without agreement of
 * authors of this project.
 */

#include <cmath>

#include "../src/core/Core.h"
#include "../src/core/components/Scene.h"
#include "../src/core/render/components/material/MaterialFactory.h"
#include "../src/core/render/components/mesh/presets/Sphere.h"
#include "../src/core/render/components/mesh/presets/Plane.h"
#include "../src/core/render/components/mesh/presets/Cube.h"

using namespace unreal_fluid;

class GlTestScene : public Scene {
public:
  render::RenderObject *sphere;
  render::RenderObject *plane;
  render::RenderObject *cube;
  const compositor::Compositor *compositor;
  const render::ShaderProgram *program;

  explicit GlTestScene(compositor::Compositor const * compositor) : Scene(compositor), compositor(compositor) {
    program = compositor->GetRenderer()->GetShaderManager()->CreateProgram({
      compositor->GetRenderer()->GetShaderManager()->LoadShader("test/triangle.vert"),
      compositor->GetRenderer()->GetShaderManager()->LoadShader("test/triangle.frag")
    });
    sphere = new render::RenderObject();
    sphere->mesh = render::mesh::Sphere(.5f, 50, 50);
    sphere->position = {-.75f, 0.f, -5.f};
    sphere->zAxisAngle = 0.f;
    sphere->modelMatrix = mat4::rotation(sphere->zAxisAngle, {0.f, 0.f, 1.f});
    sphere->modelMatrix *= mat4::translation(sphere->position);
    sphere->shaderProgram = program;
    sphere->material = render::material::MaterialFactory::createMaterial(
            render::material::MaterialFactory::MaterialType::GOLD
            );

    plane = new render::RenderObject();
    plane->mesh = render::mesh::Plane(2.5f, 2.5f, 10, 10);
    plane->position = {0.f, -1.f, -5.f};
    plane->zAxisAngle = 0.f;
    plane->modelMatrix = mat4::rotation(plane->zAxisAngle, {0.f, 0.f, 1.f});
    plane->modelMatrix *= mat4::translation(plane->position);
    plane->shaderProgram = program;
    plane->material = render::material::MaterialFactory::createMaterial(
            render::material::MaterialFactory::MaterialType::CYAN_PLASTIC
    );

    cube = new render::RenderObject();
    cube->mesh = render::mesh::Cube(.5f);
    cube->position = {.75f, 0.f, -5.f};
    cube->zAxisAngle = 0.f;
    cube->modelMatrix = mat4::rotation(cube->zAxisAngle, {0.f, 0.f, 1.f});
    cube->modelMatrix *= mat4::translation(cube->position);
    cube->shaderProgram = program;
    cube->material = render::material::MaterialFactory::createMaterial(
            render::material::MaterialFactory::MaterialType::RUBY
    );
  }

  void Update() override {
    auto time = std::chrono::system_clock::now().time_since_epoch().count() / 1000000000.0;

    sphere->zAxisAngle = std::sin(time / 10) * 100;
    cube->zAxisAngle = -std::sin(time / 10) * 100;

    sphere->modelMatrix = mat4::rotationY(sphere->zAxisAngle);
    sphere->modelMatrix *= mat4::translation(sphere->position);

    cube->modelMatrix = mat4::rotationY(cube->zAxisAngle);
    cube->modelMatrix *= mat4::translation(cube->position);
  }

  void Render() override {
    static int timer = 0;
    timer++;

    program->Execute();

    compositor->GetRenderer()->RenderObject(sphere);
    compositor->GetRenderer()->RenderObject(plane);
    compositor->GetRenderer()->RenderObject(cube);

    if (timer % 200 == 0) {
      // compositor->GetRenderer()->GetShaderManager()->ReloadShaders();

      // Logger::logInfo("All shaders have been reloaded");
    }
  }

  ~GlTestScene() override = default;
};

// end of GLTestScene.cxx
