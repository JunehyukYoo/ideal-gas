#pragma once

#include "cinder/gl/gl.h"

using glm::vec2;

namespace idealgas {

    class Particle {
    public:
        Particle(vec2 position, vec2 velocity, float mass, float radius, ci::Color color);

        vec2 GetPosition() const;

        vec2 GetVelocity() const;

        float GetRadius() const;

        float GetMass() const;

        ci::Color GetColor() const;

        void SetPosition(vec2 position);

        void SetVelocity(vec2 velocity);

    private:
        vec2 position_;
        vec2 velocity_;
        float radius_;
        ci::Color color_;
        float mass_;
    };

}  // namespace idealgas