#include "particle.h"

namespace idealgas {

    Particle::Particle(vec2 position, vec2 velocity, float mass, float radius, ci::Color color) {
        position_ = position;
        velocity_ = velocity;
        radius_ = radius;
        mass_ = mass;
        color_ = color;
    }

    vec2 Particle::GetVelocity() const {
        return velocity_;
    }

    vec2 Particle::GetPosition() const {
        return position_;
    }

    float Particle::GetRadius() const {
        return radius_;
    }

    float Particle::GetMass() const {
        return mass_;
    }

    ci::Color Particle::GetColor() const {
        return color_;
    }

    void Particle::SetVelocity(vec2 velocity) {
        velocity_ = velocity;
    }

    void Particle::SetPosition(vec2 position) {
        position_ = position;
    }
} // namespace idealgas