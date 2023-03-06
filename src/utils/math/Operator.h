/***************************************************************
 * Copyright (C) 2023
 *    HSE SPb (Higher school of economics in Saint-Petersburg).
 ***************************************************************/

/* PROJECT   : UnrealFluidPhysics
 * AUTHORS   : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev
 * FILE NAME : Operator.h
 * PURPOSE   : math operatots to use
 *
 * No part of this file may be changed and used without agreement of
 * authors of this project.
 */

#pragma once
#include "cmath"

namespace unreal_fluid::math {

    template<typename T>
    double inverseSqrt(T x) {
        //TODO make pure double sqrt calculation
        auto temp = static_cast<float>(x);
        int i = *(int*)&temp;
        i = 0x5f3759df - (i >> 1);
        float y = *(float*)&i;
        y *= (1.5F - 0.5F * x * y * y);
        return y * (1.5F - 0.5F * x * y * y);
    }

} // namespace maths