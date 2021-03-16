#include "gas_container.h"
#include <random>

namespace idealgas {

using glm::vec2;

GasContainer::GasContainer(int num_particles) {
  for (int i = 0; i <= num_particles; i++) {
    particles_.push_back(GenerateRandomParticle());
  }
}

void GasContainer::Display() const {
  // This function has a lot of magic numbers; be sure to design your code in a way that avoids this.
  ci::gl::color(ci::Color("orange"));
  ci::gl::drawSolidCircle(p1_position, 10);
  ci::gl::color(ci::Color("white"));
  ci::gl::drawStrokedRect(ci::Rectf(kFirstPoint, kSecondPoint));
}

void GasContainer::AdvanceOneFrame() {
  /**
  if (p1_position.x <= kFirstPoint.x || p1_position.x >= kSecondPoint.x) {
    std::cout << "AHHH" << std::endl;
    p1_velocity = vec2(-1 * p1_velocity.x, p1_velocity.y);
    p1_position += p1_velocity;
  } else if (p1_position.y <= kFirstPoint.y || p1_position.y >= kSecondPoint.y) {
    std::cout << "AHHH" << std::endl;
    p1_velocity = vec2(p1_velocity.x, -1 * p1_velocity.y);
    p1_position += p1_velocity;
  } else {
    p1_position += p1_velocity;
  }
   */
   CheckWallContact(particles_);
}
void GasContainer::CheckWallContact(
    std::vector<std::pair<glm::vec2, glm::vec2>> particles) {
  for (auto& particle : particles) {
    if (particle.first.x <= kFirstPoint.x || particle.first.x >= kSecondPoint.x) {
      particle.second = vec2(-1 * particle.second.x, particle.second.y);
      particle.first += particle.second;
    } else if (particle.first.y <= kFirstPoint.y || particle.first.y >= kSecondPoint.y) {
      particle.second = vec2(particle.second.x, -1 * particle.second.y);
      particle.first += particle.second;
    } else {
      particle.first += particle.second;
    }
  }
}

std::pair<glm::vec2, glm::vec2> GasContainer::GenerateRandomParticle() {
  vec2 position(GenerateRandomDouble(kFirstPoint.x, kSecondPoint.x), GenerateRandomDouble(kFirstPoint.y, kSecondPoint.y));
  vec2 velocity(GenerateRandomDouble(0, kSmallestRadius/4), GenerateRandomDouble(0, kSmallestRadius/4));
  return {position, velocity};
}

//code derived from: https://stackoverflow.com/questions/2704521/generate-random-double-numbers-in-c/9324796
double GasContainer::GenerateRandomDouble(double lower_bound, double upper_bound) {
  std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
  std::default_random_engine re;
  return unif(re);
}

}  // namespace idealgas
