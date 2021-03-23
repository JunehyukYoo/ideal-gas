#include <catch2/catch.hpp>

#include <gas_container.h>
#include <particle.h>
#include <graph.h>

using idealgas::GasContainer;
using idealgas::Particle;
using idealgas::Graph;

TEST_CASE("Edge cases") {
    SECTION("Ensure graph doesn't take into account particles of different masses") {
        GasContainer container;
        Graph graph = Graph(vec2(500, 100), vec2(800, 400), vec2(1, 0), 10, 1.0);
        Particle p1 = Particle(vec2(200, 200), vec2(1,0), 1.0, 10, ci::Color("red"));
        Particle p2 = Particle(vec2(200, 300), vec2(1,0), 2.5, 10, ci::Color("green"));
        container.AddParticle(p1);
        container.AddParticle(p2);
        std::vector<int> num_particles_in_bars = graph.NumParticlesGoingCertainSpeed(container.GetParticles());
        //Only 1 particle going between 0.9-1 speed where index in list is 9, where p2 isn't counted as its mass is different
        REQUIRE(num_particles_in_bars[9] == 1);
    }
    SECTION("No particles added") {
        GasContainer container;
        Graph graph = Graph(vec2(500, 100), vec2(800, 400), vec2(1, 0), 10, 1.0);
        std::vector<int> num_particles_in_bars = graph.NumParticlesGoingCertainSpeed(container.GetParticles());
        //Only 1 particle going between 0.9-1 speed where index in list is 9, where p2 isn't counted as its mass is different
        for (int num_particles : num_particles_in_bars) {
            REQUIRE(num_particles == 0);
        }
    }
    SECTION("Velocity of particles is 0") {
        GasContainer container;
        Graph graph = Graph(vec2(500, 100), vec2(800, 400), vec2(3, 0), 10, 1.0);
        Particle p1 = Particle(vec2(200, 200), vec2(0,0), 1.0, 10, ci::Color("red"));
        Particle p2 = Particle(vec2(200, 300), vec2(0,0), 1.0, 10, ci::Color("red"));
        Particle p3 = Particle(vec2(200, 400), vec2(0,0), 1.0, 10, ci::Color("red"));
        container.AddParticle(p1);
        container.AddParticle(p2);
        container.AddParticle(p3);
        std::vector<int> num_particles_in_bars = graph.NumParticlesGoingCertainSpeed(container.GetParticles());
        //3 particles with a speed of 0 where index in list is 0
        REQUIRE(num_particles_in_bars[0] == 3);
    }
}

TEST_CASE("Number of particles per histogram bar is correct") {
    SECTION("Scenario 1: Three different velocity particles") {
        GasContainer container;
        Graph graph = Graph(vec2(500, 100), vec2(800, 400), vec2(3, 0), 10, 1.0);
        Particle p1 = Particle(vec2(200, 200), vec2(1,0), 1.0, 10, ci::Color("red"));
        Particle p2 = Particle(vec2(200, 300), vec2(0,2), 1.0, 10, ci::Color("red"));
        Particle p3 = Particle(vec2(200, 400), vec2(3,0), 1.0, 10, ci::Color("red"));
        container.AddParticle(p1);
        container.AddParticle(p2);
        container.AddParticle(p3);
        std::vector<int> num_particles_in_bars = graph.NumParticlesGoingCertainSpeed(container.GetParticles());
        //Only 1 particle going between 2.7-3 speed where index in list is 9
        REQUIRE(num_particles_in_bars[9] == 1);
        //Only 1 particle going between 0.9-1.2 speed where index in list is 3
        REQUIRE(num_particles_in_bars[3] == 1);
        //Only 1 particle going between 1.8-2.1 speed where index in list is 6
        REQUIRE(num_particles_in_bars[6] == 1);
    }
    SECTION("Scenario 2: Three same velocity particles") {
        GasContainer container;
        Graph graph = Graph(vec2(500, 100), vec2(800, 400), vec2(1, 0), 10, 1.0);
        Particle p1 = Particle(vec2(200, 200), vec2(1,0), 1.0, 10, ci::Color("red"));
        Particle p2 = Particle(vec2(200, 300), vec2(0,1), 1.0, 10, ci::Color("red"));
        Particle p3 = Particle(vec2(200, 400), vec2(1,0), 1.0, 10, ci::Color("red"));
        container.AddParticle(p1);
        container.AddParticle(p2);
        container.AddParticle(p3);
        std::vector<int> num_particles_in_bars = graph.NumParticlesGoingCertainSpeed(container.GetParticles());
        //Only 1 particle going between 0.9-1 speed where index in list is 9
        REQUIRE(num_particles_in_bars[9] == 3);
    }
    SECTION("Scenario 3: Different delta x") {
        GasContainer container;
        Graph graph = Graph(vec2(500, 100), vec2(800, 400), vec2(3, 0), 5, 1.0);
        Particle p1 = Particle(vec2(200, 200), vec2(1,0), 1.0, 10, ci::Color("red"));
        Particle p2 = Particle(vec2(500, 300), vec2(-2,0), 1.0, 10, ci::Color("red"));
        Particle p3 = Particle(vec2(200, 400), vec2(3,0), 1.0, 10, ci::Color("red"));
        container.AddParticle(p1);
        container.AddParticle(p2);
        container.AddParticle(p3);
        std::vector<int> num_particles_in_bars = graph.NumParticlesGoingCertainSpeed(container.GetParticles());
        //Only 1 particle going between 0.6-1.2 speed where index in list is 1
        REQUIRE(num_particles_in_bars[1] == 1);
        //Only 1 particle going between 1.8-2.4 speed where index in list is 3
        REQUIRE(num_particles_in_bars[3] == 1);
        //Only 1 particle going between 2.4-3.0 speed where index in list is 4
        REQUIRE(num_particles_in_bars[4] == 1);
        REQUIRE(num_particles_in_bars.size() == 5);
    }
}