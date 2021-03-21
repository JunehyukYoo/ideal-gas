#pragma once

#include "cinder/gl/gl.h"

using glm::vec2;

namespace idealgas {

    class Graph {
    public:
        Graph(vec2 first_point, vec2 second_point, vec2 max_vel, int delta_x, float mass);

        vec2 GetMaxVel() const;
        vec2 GetFirstPoint() const;
        vec2 GetSecondPoint() const;
        float GetMass() const;
        int GetDeltaX() const;
        void SetMaxVel(vec2 max_vel);

    private:
        float mass_;
        int delta_x_;
        vec2 max_vel_;
        vec2 first_point_;
        vec2 second_point_;
    };

}  // namespace idealgas