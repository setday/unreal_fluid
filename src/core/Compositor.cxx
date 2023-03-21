/***************************************************************
 * Copyright (C) 2023
 *    HSE SPb (Higher school of economics in Saint-Petersburg).
 ***************************************************************/

/* PROJECT   : ultimate_py_project
 * AUTHORS   : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev
 * FILE NAME : Compositor.cxx
 * PURPOSE   : This class is responsible for rendering.
 *
 * No part of this file may be changed and used without agreement of
 * authors of this project.
 */

#include "Compositor.h"
#include "../scenes/Control.cxx"
#include "../scenes/GlTestScene.cxx"
#include "../scenes/TestScene.cxx"
#include "../scenes/SimpleFluidScene.cxx"

using namespace unreal_fluid::compositor;

Compositor::Compositor(Core *core) : _core(core) {
  Logger::logInfo("Creating compositor...");
  _simulator = new physics::Simulator;
  _renderer = new render::Renderer();
  Logger::logInfo("Compositor created!");
}

Compositor::~Compositor() {
  delete _renderer;
}

void Compositor::init() {
  Logger::logInfo("Initializing compositor...");

  _renderer->Init();
  //_scenes.push_back(new TestScene(this));
  //_scenes.push_back(new GlTestScene(this));
  _scenes.push_back(new SimpleFluidScene(this));
  _scenes.push_back(new Control(this));

  Logger::logInfo("Compositor initialized!");
}

void Compositor::update() {
  for (auto scene: _scenes) {
    scene->update();
  }
}

void Compositor::render() {
  _renderer->StartFrame();
  for (auto scene: _scenes) {
    scene->render();
  }
  _renderer->EndFrame();
}

void Compositor::destroy() {
  _renderer->Destroy();
  for (auto scene: _scenes) {
    scene->clear();
    delete scene;
  }
}

Core *Compositor::getCore() const {
  return _core;
}

render::Renderer *Compositor::getRenderer() const {
  return _renderer;
}

physics::Simulator *Compositor::getSimulator() const {
    return _simulator;
}

// end of Compositor.cxx
