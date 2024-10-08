#include "gas_container.h"
#include <random>

namespace idealgas {

using glm::vec2;

GasContainer::GasContainer(int num_particles) {
  for (size_t i = 0; i < kPossibleMasses.size(); i++) {
    num_particles_per_mass_.push_back(0);
  }
  for (size_t i = 0; i < num_particles; i++) {
    Particle particle = GenerateRandomParticle();
    particles_.push_back(particle);
    std::pair<bool, int> exists = {false, -1};
    for (size_t index = 0; index < kPossibleMasses.size(); index++) {
        if (kPossibleMasses[index] == particle.GetMass()) {
            exists = {true, index};
            break;
        }
    }
    if (exists.first) {
        num_particles_per_mass_[exists.second] += 1;
    }
  }
  InitializeGraphs();
}

GasContainer::GasContainer() {}

void GasContainer::Display() const {
  for (const Particle& particle : particles_) {
    ci::gl::color(ci::Color(particle.GetColor()));
    ci::gl::drawSolidCircle(particle.GetPosition(), particle.GetRadius());
  }
  ci::gl::color(ci::Color(kBorderColor));
  ci::gl::drawStrokedRect(ci::Rectf(kFirstPoint, kSecondPoint));
  for (size_t i = 0; i < graphs_.size(); i++) {
      graphs_[i].DrawGraphs(particles_, kPossibleColorsString[i], num_particles_per_mass_, i);
  }
}

void GasContainer::AdvanceOneFrame() {
   CheckWallContact();
   CheckParticleCollision();
   UpdateGraphInformation();
}

void GasContainer::CheckWallContact() {
  for (Particle &particle : particles_) {
    vec2 position = particle.GetPosition();
    vec2 velocity = particle.GetVelocity();
    if ((position.x <= kFirstPoint.x + particle.GetRadius() || position.x >= kSecondPoint.x - particle.GetRadius()) &&
        (position.y <= kFirstPoint.y + particle.GetRadius() || position.y >= kSecondPoint.y - particle.GetRadius())) {
      CornerCollision(position, velocity, particle);
    } else if (position.x <= kFirstPoint.x + kDefaultRadius || position.x >= kSecondPoint.x - kDefaultRadius) {
      EastWestWallCollision(position, velocity, particle);
    } else if (position.y <= kFirstPoint.y + kDefaultRadius || position.y >= kSecondPoint.y - kDefaultRadius) {
      NorthSouthWallCollision(position, velocity, particle);
    } else {
      particle.SetPosition(position + velocity);
    }
  }
}

void GasContainer::CornerCollision(const vec2 position, const vec2 velocity, Particle &particle) {
  vec2 new_vel = vec2(-1 * velocity.x, -1 * velocity.y);
  particle.SetVelocity(new_vel);
  vec2 new_pos = position + new_vel;
  particle.SetPosition(new_pos);
}

void GasContainer::EastWestWallCollision(const vec2 position, const vec2 velocity, Particle &particle) {
  vec2 new_vel = vec2(-1 * velocity.x, velocity.y);
  vec2 new_pos = position + new_vel;
  particle.SetVelocity(new_vel);
  particle.SetPosition(new_pos);
}

void GasContainer::NorthSouthWallCollision(const vec2 position, const vec2 velocity, Particle &particle) {
  vec2 new_vel = vec2(velocity.x, -1 * velocity.y);
  vec2 new_pos = position + new_vel;
  particle.SetVelocity(new_vel);
  particle.SetPosition(new_pos);
}

void GasContainer::CheckParticleCollision() {
  for (Particle &particle : particles_) {
    for (Particle &particle_compare : particles_) {
      if (ParticlesAreColliding(particle, particle_compare)) {
        std::pair<vec2, vec2> new_v = CalculateVelocityAfterCollision(particle, particle_compare);
        particle.SetVelocity(new_v.first);
        particle_compare.SetVelocity(new_v.second);
      }
    }
  }
}

void GasContainer::UpdateGraphInformation() {
  for (Graph &graph : graphs_) {
    graph.SetMaxVel(vec2(0,0));
    for (Particle &particle : particles_) {
      if (particle.GetMass() == graph.GetMass()) {
        if (SpeedMagnitude(particle.GetVelocity()) > SpeedMagnitude(graph.GetMaxVel())) {
          graph.SetMaxVel(particle.GetVelocity());
        }
      }
    }
  }
}

bool GasContainer::ParticlesAreColliding(const Particle p1, const Particle p2) const {
  bool collides = false;
  bool go_same_dir = false;
  if (std::abs(glm::distance(p1.GetPosition(), p2.GetPosition())) <= 2 * kDefaultRadius) {
    collides = true;
  }
  if (glm::dot((p1.GetVelocity() - p2.GetVelocity()),(p1.GetPosition() - p2.GetPosition())) < 0) {
    go_same_dir = true;
  }
  if (collides && go_same_dir) {
    return true;
  } else {
    return false;
  }
}

std::pair<vec2, vec2> GasContainer::CalculateVelocityAfterCollision(const Particle p1, const Particle p2) const{
  float dot_product_p1 = glm::dot((p1.GetPosition() - p2.GetPosition()),(p1.GetVelocity() - p2.GetVelocity()));
  double magnitude_p1 = static_cast<float>(std::pow(glm::length(p1.GetPosition() - p2.GetPosition()),2));
  float dot_product_over_magnitude_p1 = dot_product_p1 / magnitude_p1;
  float mass_eq_p1 = 2 * p2.GetMass() / (p1.GetMass() + p2.GetMass());
  vec2 right_side_p1 = mass_eq_p1 * dot_product_over_magnitude_p1 * (p1.GetPosition() - p2.GetPosition());
  vec2 final_v_p1 = p1.GetVelocity() - right_side_p1;

  float dot_product_p2 = glm::dot((p2.GetPosition() - p1.GetPosition()),(p2.GetVelocity() - p1.GetVelocity()));
  float magnitude_p2 = static_cast<float>(std::pow(glm::length(p2.GetPosition() - p1.GetPosition()),2));
  float dot_product_over_magnitude_p2 = dot_product_p2 / magnitude_p2;
  float mass_eq_p2 = 2 * p1.GetMass() / (p1.GetMass() + p2.GetMass());
  vec2 right_side_p2 = mass_eq_p2 * dot_product_over_magnitude_p2 * (p2.GetPosition() - p1.GetPosition());
  vec2 final_v_p2 = p2.GetVelocity() - right_side_p2;

  return {final_v_p1, final_v_p2};
}

Particle GasContainer::GenerateRandomParticle() const {
  double mass = RandomMassPicker();
  double radius = RandomRadiusPicker();
  vec2 position = vec2(GenerateRandomDouble(kFirstPoint.x + radius, kSecondPoint.x - radius),
                       GenerateRandomDouble(kFirstPoint.y + radius, kSecondPoint.y - radius));
  vec2 velocity = vec2(GenerateRandomDouble(1, kMaxVelocity),
                       GenerateRandomDouble(1, kMaxVelocity));
  ci::Color color = ColorPicker(mass);
  return Particle(position, velocity, mass, radius, color);
}

void GasContainer::InitializeGraphs() {
  for (size_t i = 0; i < kPossibleMasses.size(); i++) {
    Graph graph = Graph(vec2(650, kFirstPoint.y + 200 * i), vec2(870, kFirstPoint.y + 200 * (i + 1)), vec2(0, 0), kDefaultDeltaX, kPossibleMasses[i]);
    graphs_.push_back(graph);
  }
}

float GasContainer::SpeedMagnitude(vec2 velocity) const {
    return sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
}

//code derived from: https://www.delftstack.com/howto/cpp/how-to-generate-random-doubles-cpp/
double GasContainer::GenerateRandomDouble(double lower_bound, double upper_bound) const {
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_real_distribution<double> distribution(lower_bound, upper_bound);
  return distribution(eng);
}

ci::Color GasContainer::ColorPicker(float mass) const {
  for (size_t i = 0; i < kPossibleMasses.size(); i++) {
    if (mass == kPossibleMasses[i]) {
      return kPossibleColors[i];
    }
  }
  return ci::Color("white");
}

float GasContainer::RandomMassPicker() const {
  return kPossibleMasses[static_cast<int>(GenerateRandomDouble(0, kPossibleMasses.size()))];
}

float GasContainer::RandomRadiusPicker() const {
  return kPossibleRadii[static_cast<int>(GenerateRandomDouble(0, kPossibleRadii.size()))];
}

std::vector<Particle> GasContainer::GetParticles() const {
  return particles_;
}

void GasContainer::AddParticle(Particle particle) {
  particles_.push_back(particle);
}

double GasContainer::Round(float value, int decimal_places) const {
    const double multiplier = std::pow(10.0, decimal_places);
    return std::ceil(value * multiplier) / multiplier;
}
}  // namespace idealgas
