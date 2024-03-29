/***************************************************************
 * Copyright (C) 2023
 *    UnrealFluid Team (https://github.com/setday/unreal_fluid) and
 *    HSE SPb (Higher school of economics in Saint-Petersburg).
 ***************************************************************/

/* PROJECT                 : UnrealFluid
 * AUTHORS OF THIS PROJECT : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev.
 * FILE NAME               : ShaderProgram.h
 * FILE AUTHORS            : Serkov Alexander.
 *
 * No part of this file may be changed and used without
 * agreement of authors of this project.
 */

#pragma once

#include "Shader.h"
#include "../texture/Texture.h" // For Texture binding

namespace unreal_fluid::render {
  class ShaderProgram {
  private:
    int _currentTextureId = 0;
    unsigned int _programID;
    std::vector<const Shader *> _attachedShaders;

  public:
    explicit ShaderProgram();
    ~ShaderProgram();

    /// Attach shader
    /// @param shader Shader
    void attachShader(const Shader *shader);

    /// Link program
    /// @return True if success
    bool linkProgram() const;

    /// Reattach shaders
    void reattachShaders();

    /// Activate this program
    void activate();

    /// Get program ID
    /// @return Program ID
    [[nodiscard]]
    unsigned int getId() const;

    /// Get log
    /// @param log Log
    void getLog(std::string &log) const;

    /// Bind uniform attribute
    /// @param attribute Attribute
    /// @param name Name
    void bindUniformAttribute(std::string_view name, int attribute) const;

    /// Bind uniform attribute
    /// @param attribute Attribute
    /// @param name Name
    void bindUniformAttribute(std::string_view name, float attribute) const;

    /// Bind uniform attribute
    /// @param attribute Attribute
    /// @param name Name
    void bindUniformAttribute(std::string_view name, const vec2f &attribute) const;

    /// Bind uniform attribute
    /// @param attribute Attribute
    /// @param name Name
    void bindUniformAttribute(std::string_view name, const vec3f &attribute) const;

    /// Bind uniform attribute
    /// @param attribute Attribute
    /// @param name Name
    /// @param inverseView Defines what part of matrix should be used. Inverse if true, normal if false.
    void bindUniformAttribute(std::string_view name, const mat4 &attribute, bool inverseView = false) const;

    /// Bind uniform attribute
    /// @param attribute Attribute
    /// @param name Name.
    void bindUniformAttribute(std::string_view name, const Texture *attribute);

    /// Bind textures number
    /// @attention Should be called exactly before drawing all vertices
    void bindTexturesNumber() const;
  };
} // namespace unreal_fluid::render

// end of ShaderProgram.h
