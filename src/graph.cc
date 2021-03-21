#include "graph.h"

namespace idealgas {
    Graph::Graph(vec2 first_point, vec2 second_point, vec2 max_vel, int delta_x, float mass) {
        first_point_ = first_point;
        second_point_ = second_point;
        max_vel_ = max_vel;
        delta_x_ = delta_x;
        mass_ = mass;
    }

    vec2 Graph::GetMaxVel() const {
        return max_vel_;
    }

    vec2 Graph::GetFirstPoint() const {
        return first_point_;
    }

    vec2 Graph::GetSecondPoint() const {
        return second_point_;
    }

    int Graph::GetDeltaX() const {
        return delta_x_;
    }

    float Graph::GetMass() const {
        return mass_;
    }

    void Graph::SetMaxVel(vec2 max_vel) {
        max_vel_ = max_vel;
    }
} // namespace idealgas