#include <particle.h>
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

    void Graph::DrawGraphs(std::vector<Particle> particles, std::string color) const {
        ci::gl::drawStrokedRect(ci::Rectf(first_point_, second_point_));
        ci::gl::drawString(color + " = " + std::to_string(mass_) + " mass", vec2(first_point_.x + 40, first_point_.y + 10));
        ci::gl::drawString("Speed",vec2(first_point_.x + 80, second_point_.y - 12));
        //ci::gl::drawString("Frequency",vec2(first_point_.x + 10, second_point_.y - 60));
        ci::gl::drawLine(vec2(first_point_.x + 20, second_point_.y - 20), vec2(first_point_.x + 200, second_point_.y - 20));
        ci::gl::drawLine(vec2(first_point_.x + 20, second_point_.y - 20), vec2(first_point_.x + 20, first_point_.y + 20));
        std::vector<int> histogram_nums = NumParticlesGoingCertainSpeed(particles);
        for (size_t j = 0; j < histogram_nums.size(); j++) {
            ci::gl::drawSolidRect(ci::Rectf(vec2(first_point_.x + 20 + 18 * j, second_point_.y - 20 - 12 * histogram_nums[j]), vec2(first_point_.x + 20 + 18 * (j + 1), second_point_.y - 20)));
        }
        ci::gl::drawString("Max Vel =" + std::to_string(SpeedMagnitude(max_vel_)) ,vec2(first_point_.x + 90, second_point_.y - 130));
    }

    std::vector<int> Graph::NumParticlesGoingCertainSpeed(std::vector<Particle> particles) const {
        float delta_x = SpeedMagnitude(max_vel_) / delta_x_;
        std::vector<int> histogram_nums;
        for (size_t i = 0; i < delta_x_; i++) {
            int current_num = 0;
            for (Particle particle : particles) {
                if ((SpeedMagnitude(particle.GetVelocity()) > (delta_x * i) && SpeedMagnitude(particle.GetVelocity()) <= (delta_x * (i + 1))) && particle.GetMass() == mass_) {
                    current_num++;
                }
            }
            histogram_nums.push_back(current_num);
        }
        return histogram_nums;
    }

    float Graph::SpeedMagnitude(vec2 velocity) const {
        return sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
    }
} // namespace idealgas