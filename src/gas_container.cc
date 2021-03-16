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
   CheckWallContact();
   CheckParticleCollision();
}

void GasContainer::CheckWallContact() {
  for (std::pair<vec2, vec2> &particle : particles_) {
    vec2 position = particle.first;
    vec2 velocity = particle.second;
    if ((position.x <= kFirstPoint.x + kRadius || position.x >= kSecondPoint.x - kRadius) &&
        (position.y <= kFirstPoint.y + kRadius || position.y >= kSecondPoint.y - kRadius)) {
      particle.second = vec2(-1 * velocity.x, -1 * velocity.y);
      particle.first += particle.second;
    } else if (position.x <= kFirstPoint.x + kRadius || position.x >= kSecondPoint.x - kRadius) {
      particle.second = vec2(-1 * velocity.x, velocity.y);
      particle.first += particle.second;
    } else if (position.y <= kFirstPoint.y + kRadius || position.y >= kSecondPoint.y - kRadius) {
      particle.second = vec2(velocity.x, -1 * velocity.y);
      particle.first += particle.second;
    } else {
      particle.first += particle.second;
    }
  }
}

void GasContainer::CheckParticleCollision() {
  for (std::pair<vec2, vec2> &particle : particles_) {
    for (std::pair<vec2, vec2> &particle_compare : particles_) {
      if (ParticlesAreColliding(particle, particle_compare)) {
        std::pair<vec2, vec2> new_v = CalculateVelocityAfterCollision(particle, particle_compare);
        particle.second = new_v.first;
        particle_compare.second = new_v.second;
      }
    }
  }
}

bool GasContainer::ParticlesAreColliding(std::pair<vec2, vec2> p1, std::pair<vec2, vec2> p2) {
  bool collides = false;
  bool go_same_dir = false;
  if (std::abs(glm::distance(p1.first, p2.first)) <= 2 * kRadius) {
    collides = true;
  }
  if (glm::dot((p1.second - p2.second),(p1.first - p2.first)) < 0) {
    go_same_dir = true;
  }
  if (collides && go_same_dir) {
    return true;
  } else {
    return false;
  }
}

std::pair<vec2, vec2> GasContainer::CalculateVelocityAfterCollision(std::pair<vec2, vec2> p1, std::pair<vec2, vec2> p2) {
  float dot_product_p1 = glm::dot((p1.first - p2.first),(p1.second - p2.second));
  double magnitude_p1 = static_cast<float>(std::pow(glm::length(p1.first - p2.first),2));
  float dot_product_over_magnitude_p1 = dot_product_p1 / magnitude_p1;
  vec2 right_side_p1 = dot_product_over_magnitude_p1 * (p1.first - p2.first);
  vec2 final_v_p1 = p1.second - right_side_p1;

  float dot_product_p2 = glm::dot((p2.first - p1.first),(p2.second - p1.second));
  float magnitude_p2 = static_cast<float>(std::pow(glm::length(p2.first - p1.first),2));
  float dot_product_over_magnitude_p2 = dot_product_p2 / magnitude_p2;
  vec2 right_side_p2 = dot_product_over_magnitude_p2 * (p2.first - p1.first);
  vec2 final_v_p2 = p2.second - right_side_p2;

  return {final_v_p1, final_v_p2};
}

std::pair<glm::vec2, glm::vec2> GasContainer::GenerateRandomParticle() {
  vec2 position = vec2(GenerateRandomDouble(kFirstPoint.x + kRadius, kSecondPoint.x - kRadius),
                       GenerateRandomDouble(kFirstPoint.y + kRadius, kSecondPoint.y - kRadius));
  vec2 velocity = vec2(GenerateRandomDouble(1, kRadius/2),
                       GenerateRandomDouble(1, kRadius/2));
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
