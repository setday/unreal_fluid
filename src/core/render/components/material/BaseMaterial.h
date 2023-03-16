/***************************************************************
 * Copyright (C) 2023
 *    HSE SPb (Higher school of economics in Saint-Petersburg).
 ***************************************************************/

/* PROJECT   : ultimate_py_project
 * AUTHORS   : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev
 * FILE NAME : BaseMaterial.h
 * PURPOSE   : ${PURPOSE}
 *
 * No part of this file may be changed and used without agreement of
 * authors of this project.
 */

#pragma once

#include "../../../../Definitions.h"

namespace unreal_fluid::render::material {
  struct BaseMaterial {
    float shininess = 32.0f;
    float specular_strength = 0.5f;

    vec3f ambient_color = vec3f(0.1f, 0.1f, 0.1f);
    vec3f diffuse_color = vec3f(0.5f, 0.5f, 0.5f);
    vec3f specular_color = vec3f(1.0f, 1.0f, 1.0f);

    float reflection_strength = 0.5f;
    float refraction_strength = 0.5f;
    float refraction_index = 1.5f;
  };
} // unreal_fluid::render::material

// end of BaseMaterial.h