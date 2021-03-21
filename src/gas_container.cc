#include "gas_container.h"
#include <random>

namespace idealgas {

using glm::vec2;

GasContainer::GasContainer(int num_particles) {
  for (size_t i = 0; i < num_particles; i++) {
    particles_.push_back(GenerateRandomParticle());
  }
  InitializeGraphs();
}

GasContainer::GasContainer() {}

void GasContainer::Display() const {
  //Draws particles
  for (const Particle& particle : particles_) {
    ci::gl::color(ci::Color(particle.GetColor()));
    ci::gl::drawSolidCircle(particle.GetPosition(), particle.GetRadius());
  }
  //Draws container
  ci::gl::color(ci::Color(kBorderColor));
  ci::gl::drawStrokedRect(ci::Rectf(kFirstPoint, kSecondPoint));
  DrawGraphs();
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
      vec2 new_vel = vec2(-1 * velocity.x, -1 * velocity.y);
      particle.SetVelocity(new_vel);
      vec2 new_pos = position + new_vel;
      particle.SetPosition(new_pos);
    } else if (position.x <= kFirstPoint.x + kDefaultRadius || position.x >= kSecondPoint.x - kDefaultRadius) {
      vec2 new_vel = vec2(-1 * velocity.x, velocity.y);
      vec2 new_pos = position + new_vel;
      particle.SetVelocity(new_vel);
      particle.SetPosition(new_pos);
    } else if (position.y <= kFirstPoint.y + kDefaultRadius || position.y >= kSecondPoint.y - kDefaultRadius) {
      vec2 new_vel = vec2(velocity.x, -1 * velocity.y);
      vec2 new_pos = position + new_vel;
      particle.SetVelocity(new_vel);
      particle.SetPosition(new_pos);
    } else {
      particle.SetPosition(position + velocity);
    }
  }
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

void GasContainer::DrawGraphs() const {
  for (size_t i = 0; i < graphs_.size(); i++) {
    ci::gl::drawStrokedRect(ci::Rectf(graphs_[i].GetFirstPoint(), graphs_[i].GetSecondPoint()));
    ci::gl::drawString(kPossibleColorsString[i] + " = " + std::to_string(graphs_[i].GetMass()) + " mass", vec2(graphs_[i].GetFirstPoint().x + 40, graphs_[i].GetFirstPoint().y + 10));
    ci::gl::drawString("Speed",vec2(graphs_[i].GetFirstPoint().x + 80, graphs_[i].GetSecondPoint().y - 12));
    //ci::gl::drawString("Frequency",vec2(graphs_[i].GetFirstPoint().x + 10, graphs_[i].GetSecondPoint().y - 60));
    ci::gl::drawLine(vec2(graphs_[i].GetFirstPoint().x + 20, graphs_[i].GetSecondPoint().y - 20), vec2(graphs_[i].GetFirstPoint().x + 200, graphs_[i].GetSecondPoint().y - 20));
    ci::gl::drawLine(vec2(graphs_[i].GetFirstPoint().x + 20, graphs_[i].GetSecondPoint().y - 20), vec2(graphs_[i].GetFirstPoint().x + 20, graphs_[i].GetFirstPoint().y + 20));
    std::vector<int> histogram_nums = NumParticlesGoingCertainSpeed(graphs_[i]);
    for (size_t j = 0; j < histogram_nums.size(); j++) {
      ci::gl::drawSolidRect(ci::Rectf(vec2(graphs_[i].GetFirstPoint().x + 20 + 18 * j, graphs_[i].GetSecondPoint().y - 20 - 12 * histogram_nums[j]), vec2(graphs_[i].GetFirstPoint().x + 20 + 18 * (j + 1), graphs_[i].GetSecondPoint().y - 20)));
    }
    ci::gl::drawString("Max Vel =" + std::to_string(SpeedMagnitude(graphs_[i].GetMaxVel())) ,vec2(graphs_[i].GetFirstPoint().x + 90, graphs_[i].GetSecondPoint().y - 110));
  }
}

std::vector<int> GasContainer::NumParticlesGoingCertainSpeed(Graph graph) const {
  float delta_x = SpeedMagnitude(graph.GetMaxVel()) / graph.GetDeltaX();
  std::vector<int> histogram_nums;
  for (size_t i = 0; i < graph.GetDeltaX(); i++) {
    int current_num = 0;
    for (Particle particle : particles_) {
      if ((SpeedMagnitude(particle.GetVelocity()) > (delta_x * i) && SpeedMagnitude(particle.GetVelocity()) <= (delta_x * (i + 1))) && particle.GetColor() == ColorPicker(graph.GetMass())) {
        current_num++;
      }
    }
    histogram_nums.push_back(current_num);
  }
  return histogram_nums;
}

float GasContainer::SpeedMagnitude(vec2 velocity) const {
  return sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
}

bool GasContainer::ParticlesAreColliding(Particle p1, Particle p2) const {
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

std::pair<vec2, vec2> GasContainer::CalculateVelocityAfterCollision(Particle p1, Particle p2) const{
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

Particle GasContainer::GenerateRandomParticle() {
  double mass = RandomMassPicker();
  double radius = RandomRadiusPicker();
  vec2 position = vec2(GenerateRandomDouble(kFirstPoint.x + radius, kSecondPoint.x - radius),
                       GenerateRandomDouble(kFirstPoint.y + radius, kSecondPoint.y - radius));
  vec2 velocity = vec2(GenerateRandomDouble(1, kMaxVelocity),
                       GenerateRandomDouble(1, kMaxVelocity));
  ci::Color color = ColorPicker(mass);
  return Particle(position, velocity, mass, kDefaultRadius, color);
}

void GasContainer::InitializeGraphs() {
  for (size_t i = 0; i < kPossibleMasses.size(); i++) {
    Graph graph = Graph(vec2(650, kFirstPoint.y + 200 * i), vec2(870, kFirstPoint.y + 200 * (i + 1)), vec2(0, 0), kDefaultDeltaX, kPossibleMasses[i]);
    graphs_.push_back(graph);
  }
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

std::vector<Particle> GasContainer::GetParticles() {
  return particles_;
}

void GasContainer::AddParticle(Particle particle) {
    particles_.push_back(particle);
}
}  // namespace idealgas
