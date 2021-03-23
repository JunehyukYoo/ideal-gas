#include <particle.h>
#include "graph.h"

namespace idealgas {

Graph::Graph(vec2 first_point, vec2 second_point, vec2 max_vel, int delta_x, float mass) {
    first_point_ = first_point;
    second_point_ = second_point;
    max_vel_ = max_vel;
    delta_x_ = delta_x;
    mass_ = mass;
    origin_ = vec2(first_point.x + kAxisMargin, second_point.y - kAxisMargin);
    x_axis_point_ = vec2(second_point.x - kAxisMargin, second_point.y - kAxisMargin);
    y_axis_point_ = vec2(first_point.x + kAxisMargin, first_point.y + kAxisMargin);
    x_axis_label_ = vec2(first_point.x + 4 * kAxisMargin, second_point.y - kTextMargin);
    title_label_ = vec2(first_point.x + 2 * kAxisMargin, first_point.y + kTextMargin);
    change_in_x_per_bar_ = (x_axis_point_.x - origin_.x) / delta_x;
    max_vel_display_point_ = vec2(first_point.x + kInfoDisplayMargin, first_point.y + 3 * kAxisMargin);
    num_particles_display_point_ = vec2(first_point.x + kInfoDisplayMargin, first_point.y + 2 * kAxisMargin);
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

void Graph::DrawGraphs(std::vector<Particle> particles, std::string color, std::vector<int> num_of_particles, size_t i) const {
    ci::gl::drawStrokedRect(ci::Rectf(first_point_, second_point_));
    ci::gl::drawString(color + " = " + std::to_string(mass_) + " mass", title_label_);
    ci::gl::drawString("Speed", x_axis_label_);
    ci::gl::drawLine(origin_, x_axis_point_);
    ci::gl::drawLine(origin_, y_axis_point_);
    std::vector<int> histogram_nums = NumParticlesGoingCertainSpeed(particles);
    for (size_t j = 0; j < histogram_nums.size(); j++) {
        ci::gl::drawSolidRect(ci::Rectf(vec2(first_point_.x + kAxisMargin + change_in_x_per_bar_ * j, second_point_.y - kAxisMargin - kMarginPerParticle * histogram_nums[j]),
                                           vec2(first_point_.x + kAxisMargin + change_in_x_per_bar_ * (j + 1), second_point_.y - kAxisMargin)));
    }
    ci::gl::drawString("Max Vel =" + std::to_string(SpeedMagnitude(max_vel_)), max_vel_display_point_);
    ci::gl::drawString("Num of particles =" + std::to_string(num_of_particles[i]), num_particles_display_point_);
    ci::gl::pushModelMatrix();
    ci::gl::translate(first_point_.x + 10, first_point_.y + 120);
    ci::gl::rotate(-1* M_PI_2);
    ci::gl::drawString("Frequency", vec2(0, 0));
    ci::gl::popModelMatrix();
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