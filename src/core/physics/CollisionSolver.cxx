/***************************************************************
 * Copyright (C) 2023
 *    HSE SPb (Higher school of economics in Saint-Petersburg).
 ***************************************************************/

/* PROJECT   : UnrealFluidPhysics
 * AUTHORS   : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev
 * FILE NAME : CollisionSolver.cxx
 * PURPOSE   : Implements collision between different objects
 *
 * No part of this file may be changed and used without agreement of
 * authors of this project.
 */

#include "CollisionSolver.h"

using namespace unreal_fluid::physics;

void CollisionSolver::particleWithParticleCollision(fluid::Particle *p1, fluid::Particle *p2, double k) {
  vec3 diff = p1->position - p2->position;

  if (diff.len2() == 0) return;

  double diffLen = diff.len();
  vec3 direction = diff / diffLen;

  double pushValue =
          (p1->radius + p2->radius - diffLen) /
          (p1->mass + p2->mass);

  if (pushValue < 0) return;

  vec3 pushVector = direction * pushValue;

  p1->position += pushVector * p2->mass;
  p2->position -= pushVector * p1->mass;

  double momentumValue =
          (1 + k) *
          (p1->velocity.dot(direction) - p2->velocity.dot(direction)) /
          (p1->mass + p2->mass);
  vec3 momentum = direction * momentumValue;

  p1->velocity -= momentum * p2->mass;
  p2->velocity += momentum * p1->mass;
}

void CollisionSolver::particleWithSphereCollision(fluid::Particle *p, solid::SolidSphere *s, double k) {
  vec3 diff = s->position - p->position;

  if (diff.len2() == 0) return;

  double diffLen = diff.len();

  if (diffLen > p->radius + s->radius) return;

  diff /= diffLen;

  double pushValue = s->radius + p->radius - diffLen;

  p->position -= diff * pushValue;
  p->velocity -= diff * (1 + k) * p->velocity.dot(diff);
}

// end of CollisionSolver.cxx
