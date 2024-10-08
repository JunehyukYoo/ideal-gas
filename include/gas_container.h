#pragma once

#include "cinder/gl/gl.h"
#include "graph.h"
#include "particle.h"

using glm::vec2;

namespace idealgas {

/**
 * The container in which all of the gas particles are contained. This class
 * stores all of the particles and updates them on each frame of the simulation.
 */
class GasContainer {
public:
    GasContainer(int num_particles);

    GasContainer();

    /**
     * Displays the container walls and the current positions of the particles.
     */
    void Display() const;

    /**
     * Updates the positions and velocities of all particles (based on the rules
     * described in the assignment documentation).
     */
    void AdvanceOneFrame();

    /**
     * Checks if any of the particles are about to hit the container. If so,
     * change its velocity the moment it hits a wall.
     */
    void CheckWallContact();

    /**
     * Changes position and velocity of particle colliding perfectly into the corner.
     * @param position The position.
     * @param velocity The velocity.
     * @param particle The particle to analyze.
     */
    void CornerCollision(vec2 position, vec2 velocity, Particle &particle);

    /**
     * Changes position and velocity of particle colliding into the East or West walls.
     * @param position The position.
     * @param velocity The velocity.
     * @param particle The particle to analyze.
     */
    void EastWestWallCollision(vec2 position, vec2 velocity, Particle &particle);

    /**
     * Changes position and velocity of particle colliding into the North or South walls.
     * @param position The position.
     * @param velocity The velocity.
     * @param particle The particle to analyze.
     */
    void NorthSouthWallCollision(vec2 position, vec2 velocity, Particle &particle);

    /**
     * Checks if any particles are colliding with each other. If so, change the
     * velocity of colliding particles.
     */
    void CheckParticleCollision();

    /**
     * Checks if two particles are colliding.
     * @param p1 The first particle to compare.
     * @param p2 The second particle to compare.
     * @return A boolean value.
     */
    bool ParticlesAreColliding(Particle p1, Particle p2) const;

    /**
     * Changes the velocities of colliding particles.
     * @param p1 The first particle.
     * @param p2 The second particle.
     * @return New velocity of p1 and p2 as a pair in that order.
     */
    std::pair<vec2, vec2> CalculateVelocityAfterCollision(Particle p1, Particle p2) const;

    /**
     * Generates a particle with a random position in the container and random
     * velocity.
     * @return A pair, the first being the position vector, the second being the
     * velocity vector.
     */
    Particle GenerateRandomParticle() const;

    /**
     * Initializes the graphs.
     */
    void InitializeGraphs();

    /**
     * Calculates the magnitude of a velocity vector for comparison purposes.
     * @param velocity The velocity to convert.
     * @return A scalar magnitude.
     */
    float SpeedMagnitude(vec2 velocity) const;

    /**
     * Updates relevant graph information. Called every frame.
     */
    void UpdateGraphInformation();

    /**
     * Picks a random mass out of a list.
     * @return The mass.
     */
    float RandomMassPicker() const;

    /**
     * Picks a random radius out of a list.
     * @return The list.
     */
    float RandomRadiusPicker() const;

    /**
     * Picks a random color out of a list respective to the particles' mass.
     * @param mass The particles' mass.
     * @return The color.
     */
    ci::Color ColorPicker(float mass) const;

    /**
     * Generates a random double from a lower and upper bound.
     * @param lower_bound is the lower bound.
     * @param upper_bound is the upper bound.
     * @return A random double within the bounds.
     */
    double GenerateRandomDouble(double lower_bound, double upper_bound) const;

    /**
     * Gets the list of all particles in the container for testing purposes.
     * @return the list of all particles.
     */
    std::vector<Particle> GetParticles() const;

    /**
     * Adds particle to list. For testing purposes.
     * @param particle The particle to add to list.
     */
    void AddParticle(Particle particle);

    double Round(float value, int decimal_places) const;


private:
    std::vector<Particle> particles_;
    std::vector<Graph> graphs_;
    std::vector<int> num_particles_per_mass_;
    float kDefaultRadius = 10;
    float kMaxVelocity = 2.5;
    int kDefaultDeltaX = 10;
    const vec2 kFirstPoint = vec2(100, 100);
    const vec2 kSecondPoint = vec2(600, 600);
    const ci::Color kBorderColor = ci::Color("white");

    /**
     * BELOW VECTORS MUST HAVE THE SAME SIZE!
     */
    const std::vector<ci::Color> kPossibleColors = {ci::Color("red"), ci::Color("green"), ci::Color("blue")};
    const std::vector<float> kPossibleMasses = {1, 2.5 , 5};
    const std::vector<float> kPossibleRadii = {8, 10, 12};
    const std::vector<std::string> kPossibleColorsString = {"red", "green", "blue"};
};

}  // namespace idealgas