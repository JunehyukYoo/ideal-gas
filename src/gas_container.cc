#include "gas_container.h"
#include <random>

namespace idealgas {

using glm::vec2;

GasContainer::GasContainer(int num_particles) {
  for (int i = 0; i < num_particles; i++) {
    particles_.push_back(GenerateRandomParticle());
    std::cout << "count = " << i << std::endl;
  }
  //Print();
}

void GasContainer::Display() const {
  // This function has a lot of magic numbers; be sure to design your code in a way that avoids this.
  for (const auto& particle : particles_) {
    ci::gl::color(ci::Color("orange"));
    ci::gl::drawSolidCircle(particle.first, kRadius);
  }
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
   CheckWallContact();
   std::cout << "UPDATE\n";
}
void GasContainer::CheckWallContact() {
  for (std::pair<glm::vec2, glm::vec2> &particle : particles_) {
    glm::vec2 position = particle.first;
    glm::vec2 velocity = particle.second;
    if (position.x <= kFirstPoint.x || position.x >= kSecondPoint.x) {
      particle.second = vec2(-1 * velocity.x, velocity.y);
      particle.first += particle.second;
    } else if (position.y <= kFirstPoint.y || position.y >= kSecondPoint.y) {
      particle.second = vec2(velocity.x, -1 * velocity.y);
      particle.first += particle.second;
    } else {
      particle.first += particle.second;
    }
  }
}

void GasContainer::CheckParticleCollision() {

}

std::pair<glm::vec2, glm::vec2> GasContainer::GenerateRandomParticle() {
  vec2 position = vec2(GenerateRandomDouble(kFirstPoint.x + kRadius, kSecondPoint.x - kRadius),
                       GenerateRandomDouble(kFirstPoint.y + kRadius, kSecondPoint.y - kRadius));
  vec2 velocity = vec2(GenerateRandomDouble(1, kRadius),
                       GenerateRandomDouble(1, kRadius));
  return {position, velocity};
}

//code derived from: https://www.delftstack.com/howto/cpp/how-to-generate-random-doubles-cpp/
double GasContainer::GenerateRandomDouble(double lower_bound, double upper_bound) const {
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_real_distribution<double> distribution(lower_bound, upper_bound);
  return distribution(eng);
}

void GasContainer::Print() {
  for (auto particle : particles_) {
    std::cout << "Position = (" + std::to_string(particle.first.x) + ", " + std::to_string(particle.first.y) + ")" << std::endl;
    std::cout << "Velocity = (" + std::to_string(particle.second.x) + ", " + std::to_string(particle.second.y) + ")" << std::endl;
    std::cout << "\n";
  }
}
}  // namespace idealgas
