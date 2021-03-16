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

  GasContainer();

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

  /**
   * Checks if any particles are colliding with each other. If so, change the
   * velocity of colliding particles.
   */
  void CheckParticleCollision();

  /**
   * Checks if two particles are colliding.
   * @param p1 The first particle to compare.
   * @param p2 The second particle to compare.
   * @return A boolean value.
   */
  bool ParticlesAreColliding(std::pair<vec2, vec2> p1, std::pair<vec2, vec2> p2);

  /**
   * Changes the velocities of colliding particles.
   * @param p1 The first particle.
   * @param p2 The second particle.
   * @return New velocity of p1 and p2 as a pair in that order.
   */
  std::pair<vec2, vec2> CalculateVelocityAfterCollision(std::pair<vec2, vec2> p1, std::pair<vec2, vec2> p2);

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
   * Returns a random color for a particle.
   * @return Random color.
   */
  ci::Color RandomColorPicker() const;

  /**
   * Gets the list of all particles in the container for testing purposes.
   * @return the list of all particles.
   */
  std::vector<std::pair<glm::vec2, glm::vec2>> GetParticles();

  void AddParticles(std::pair<glm::vec2, glm::vec2> particle);

 private:
  std::vector<std::pair<glm::vec2, glm::vec2>> particles_;
  int kRadius = 10;
  vec2 kFirstPoint = vec2(100, 100);
  vec2 kSecondPoint = vec2(600, 600);
  ci::Color kBorderColor = ci::Color("white");
  std::vector<ci::Color> kPossibleColors = {ci::Color("orange"), ci::Color("red"), ci::Color("green"), ci::Color("blue")};
};

}  // namespace idealgas
