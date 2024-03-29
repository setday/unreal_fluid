/***************************************************************
* Copyright (C) 2023
*    UnrealFluid Team (https://github.com/setday/unreal_fluid) and
*    HSE SPb (Higher school of economics in Saint-Petersburg).
***************************************************************/

/* PROJECT                 : UnrealFluid
 * AUTHORS OF THIS PROJECT : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev.
 * FILE NAME               : ShaderManager.h
 * FILE AUTHORS            : Serkov Alexander.
 * PURPOSE                 : Compile and store shaders
 *
 * No part of this file may be changed and used without
 * agreement of authors of this project.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../../../../Definitions.h"

#include "../../../render/components/shaders/ShaderProgram.h"

namespace unreal_fluid::render {
  class ShaderManager {
  private:
    std::vector<std::unique_ptr<Shader>> _shaders;
    std::vector<std::unique_ptr<ShaderProgram>> _programs;
    std::vector<std::string> _shaderPaths;

  public:
    ShaderManager();
    virtual ~ShaderManager() = default;

    /// Load shader from file
    /// @param path Path to shader file
    /// @return Shader pointer
    const Shader * LoadShader(std::string_view path);

    /// create program from shaders
    /// @param shaders Shaders to create program from
    /// @return Program pointer
    ShaderProgram * CreateProgram(const std::vector<const Shader *> &shaders);

    /// Load program from files
    /// @param paths Paths to shader files
    /// @return Program pointer
    ShaderProgram * LoadProgram(const std::vector<std::string> &paths);

    /// Load program from directory
    /// @param dir Directory with shader files
    /// @return Program pointer
    ShaderProgram * LoadProgram(std::string_view dir);

    /// Reload shader
    /// @param shader Shader to reload
    void ReloadShader(Shader *shader);

    /// Reload all shaders
    void ReloadShaders();
  };

  class DefaultShaderManager : private ShaderManager {
  private:
    static ShaderProgram *_defaultProgram;
    static ShaderProgram *_rtProgram;
    static ShaderProgram *_postProcessingProgram;

    static ShaderProgram *_gasProgram;

    static DefaultShaderManager _instance;

  public:
    /// Get default program
    /// @return Default program
    static ShaderProgram * GetDefaultProgram();

    /// Get default ray tracing program
    /// @return Default ray tracing program
    static ShaderProgram * GetRayTracingProgram();

    /// Get default post processing program
    /// @return Default post processing program
    static ShaderProgram * GetPostProcessingProgram();

    /// Get default gas program
    /// @return Default gas program
    static ShaderProgram * GetGasProgram();

    /// Reload all shaders
    static void ReloadShaders();
  };
} // namespace unreal_fluid::render

// end of ShaderManager.h
