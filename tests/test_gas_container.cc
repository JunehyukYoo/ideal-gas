#include <catch2/catch.hpp>

#include <gas_container.h>

using idealgas::GasContainer;

TEST_CASE("No wall no collision") {
    GasContainer container;
    idealgas::Particle test_particle(vec2(200,200),vec2(5,5), 5, 10, ci::Color("red"));
    container.AddParticle(test_particle);
    container.AdvanceOneFrame();
    REQUIRE(vec2(5,5) == container.GetParticles()[0].GetVelocity());
    REQUIRE(vec2(205,205) == container.GetParticles()[0].GetPosition());
}

TEST_CASE("Collides with walls") {
    SECTION("Hits North wall") {
        GasContainer container;
        idealgas::Particle test_particle(vec2(200,101),vec2(0,-5), 5, 10, ci::Color("red"));
        container.AddParticle(test_particle);
        container.AdvanceOneFrame();
        REQUIRE(vec2(0,5) == container.GetParticles()[0].GetVelocity());
        REQUIRE(vec2(200,106) == container.GetParticles()[0].GetPosition());
    }
    SECTION("Hits South wall") {
        GasContainer container;
        idealgas::Particle test_particle(vec2(200,599),vec2(0,5), 5, 10, ci::Color("red"));
        container.AddParticle(test_particle);
        container.AdvanceOneFrame();
        REQUIRE(vec2(0,-5) == container.GetParticles()[0].GetVelocity());
        REQUIRE(vec2(200,594) == container.GetParticles()[0].GetPosition());
    }
    SECTION("Hits East wall") {
        GasContainer container;
        idealgas::Particle test_particle(vec2(599,200),vec2(5,0), 5, 10, ci::Color("red"));
        container.AddParticle(test_particle);
        container.AdvanceOneFrame();
        REQUIRE(vec2(-5,0) == container.GetParticles()[0].GetVelocity());
        REQUIRE(vec2(594,200) == container.GetParticles()[0].GetPosition());
    }
    SECTION("Hits West wall") {
        GasContainer container;
        idealgas::Particle test_particle(vec2(101,200),vec2(-5,0), 5, 10, ci::Color("red"));
        container.AddParticle(test_particle);
        container.AdvanceOneFrame();
        REQUIRE(vec2(5,0) == container.GetParticles()[0].GetVelocity());
        REQUIRE(vec2(106,200) == container.GetParticles()[0].GetPosition());
    }
    SECTION("Collides in the corner") {
        GasContainer container;
        idealgas::Particle test_particle(vec2(101,101),vec2(-3,-3), 5, 10, ci::Color("red"));
        container.AddParticle(test_particle);
        container.AdvanceOneFrame();
        REQUIRE(vec2(3,3) == container.GetParticles()[0].GetVelocity());
        REQUIRE(vec2(104,104) == container.GetParticles()[0].GetPosition());
    }
}

TEST_CASE("Particle collisions") {
    SECTION("Standard collision horizontal") {
        GasContainer container;
        idealgas::Particle test_p1(vec2(200,200),vec2(1,0), 5, 10, ci::Color("red"));
        idealgas::Particle test_p2(vec2(208,200),vec2(-1,0), 5, 10, ci::Color("red"));
        container.AddParticle(test_p1);
        container.AddParticle(test_p2);
        container.AdvanceOneFrame();
        REQUIRE(vec2(-1, 0) == container.GetParticles()[0].GetVelocity());
        REQUIRE(vec2(1,0) == container.GetParticles()[1].GetVelocity());
    }
    SECTION("Standard collision vertical") {
        GasContainer container;
        idealgas::Particle test_p1(vec2(200,200),vec2(0,1), 5, 10, ci::Color("red"));
        idealgas::Particle test_p2(vec2(200,209),vec2(0,-1), 5, 10, ci::Color("red"));
        container.AddParticle(test_p1);
        container.AddParticle(test_p2);
        container.AdvanceOneFrame();
        REQUIRE(vec2(0,-1) == container.GetParticles()[0].GetVelocity());
        REQUIRE(vec2(0,1) == container.GetParticles()[1].GetVelocity());
    }
    SECTION("Standard collision diagonal") {
        GasContainer container;
        idealgas::Particle test_p1(vec2(200,200),vec2(1,1), 1, 10, ci::Color("red"));
        idealgas::Particle test_p2(vec2(208,208),vec2(-1,-1), 1, 10, ci::Color("red"));
        container.AddParticle(test_p1);
        container.AddParticle(test_p2);
        container.AdvanceOneFrame();
        REQUIRE(-1.0 == container.Round(container.GetParticles()[0].GetVelocity().x, 2));
        REQUIRE(-1.0 == container.Round(container.GetParticles()[0].GetVelocity().y, 2));
        REQUIRE(1.01 == container.Round(container.GetParticles()[1].GetVelocity().x, 2));
        REQUIRE(1.01 == container.Round(container.GetParticles()[1].GetVelocity().x, 2));
    }
    SECTION("Collision distance but going opposite directions") {
        GasContainer container;
        idealgas::Particle test_p1(vec2(200,200),vec2(-1,0), 5, 10, ci::Color("red"));
        idealgas::Particle test_p2(vec2(208,200),vec2(1,0), 5, 10, ci::Color("red"));
        container.AddParticle(test_p1);
        container.AddParticle(test_p2);
        container.AdvanceOneFrame();
        REQUIRE(vec2(-1, 0) == container.GetParticles()[0].GetVelocity());
        REQUIRE(vec2(1,0) == container.GetParticles()[1].GetVelocity());
    }
}