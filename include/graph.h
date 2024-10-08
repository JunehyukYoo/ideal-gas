#pragma once

#include "cinder/gl/gl.h"
#include "particle.h"

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

    /**
     * Draws the graphs on the BasicApp.
     */
    void DrawGraphs(std::vector<Particle> particles, std::string color, std::vector<int> num_of_particles, size_t i) const;

    /**
     * Creates a list of integers, each integer representing the number of particles with velocities within a
     * certain range. Used to create the histogram bars.
     * @param graph The respective graph.
     * @return The list of numbers per histogram bar.
     */
    std::vector<int> NumParticlesGoingCertainSpeed(std::vector<Particle> particles) const;

    /**
     * Calculates the magnitude of a velocity vector for comparison purposes.
     * @param velocity The velocity to convert.
     * @return A scalar magnitude.
     */
    float SpeedMagnitude(vec2 velocity) const;

private:
    float mass_;
    int delta_x_;
    vec2 max_vel_;
    vec2 first_point_;
    vec2 second_point_;
    vec2 title_label_;
    vec2 y_axis_point_;
    vec2 x_axis_point_;
    vec2 origin_;
    vec2 x_axis_label_;
    vec2 max_vel_display_point_;
    vec2 num_particles_display_point_;
    float change_in_x_per_bar_;
    const int kAxisMargin = 20;
    const int kTextMargin = 12;
    const int kMarginPerParticle = 12;
    const int kInfoDisplayMargin = 90;
};

}  // namespace idealgas