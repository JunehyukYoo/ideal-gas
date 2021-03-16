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

  void CheckWallContact(std::vector<std::pair<glm::vec2, glm::vec2>> particles);

  std::pair<glm::vec2, glm::vec2> GenerateRandomParticle();

  double GenerateRandomDouble(double lower_bound, double upper_bound);

 private:
  //int dummy_variable_;
  std::vector<std::pair<glm::vec2, glm::vec2>> particles_;
  //vec2 p1_position = vec2(200, 200);
  //vec2 p1_velocity = vec2(1.2, 0);
  int kSmallestRadius = 10;
  //int kBiggestRadius = 15;
  vec2 kFirstPoint = vec2(100, 100);
  vec2 kSecondPoint = vec2(500, 400);
};

}  // namespace idealgas
