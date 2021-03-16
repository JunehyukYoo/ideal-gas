#pragma once

#include "cinder/gl/gl.h"

using glm::vec2;

namespace idealgas {

/**
 * The container in which all of the gas particles are contained. This class
 * stores all of the particles and updates them on each frame of the simulation.
 */
class GasContainer {
 public:
  GasContainer(int num_particles);

  /**
   * Displays the container walls and the current positions of the particles.
   */
  void Display() const;

  /**
   * Updates the positions and velocities of all particles (based on the rules
   * described in the assignment documentation).
   */
  void AdvanceOneFrame();

  /**
   * Checks if any of the particles are about to hit the container. If so,
   * change its velocity the moment it hits a wall.
   */
  void CheckWallContact();

  void CheckParticleCollision();

  /**
   * Generates a particle with a random position in the container and random
   * velocity.
   * @return A pair, the first being the position vector, the second being the
   * velocity vector.
   */
  std::pair<glm::vec2, glm::vec2> GenerateRandomParticle();

  /**
   * Generates a random double from a lower and upper bound.
   * @param lower_bound is the lower bound.
   * @param upper_bound is the upper bound.
   * @return A random double within the bounds.
   */
  double GenerateRandomDouble(double lower_bound, double upper_bound) const;

  /**
   * Prints the current position and velocity of all particles in the
   * container.
   */
  void Print();

 private:
  std::vector<std::pair<glm::vec2, glm::vec2>> particles_;
  //vec2 p1_position = vec2(200, 200);
  //vec2 p1_velocity = vec2(1.2, 0);
  int kRadius = 10;
  vec2 kFirstPoint = vec2(100, 100);
  vec2 kSecondPoint = vec2(600, 400);
};

}  // namespace idealgas
