#include <catch2/catch.hpp>

#include <gas_container.h>

using idealgas::GasContainer;

TEST_CASE("No wall no collision") {
  GasContainer container(0);
  std::pair<glm::vec2, glm::vec2> test_particle(vec2(200,200),vec2(5,5));
  container.AddParticles(test_particle);
  container.AdvanceOneFrame();
  REQUIRE(vec2(5,5) == container.GetParticles()[0].second);
  REQUIRE(vec2(205,205) == container.GetParticles()[0].first);
}

TEST_CASE("Collides with walls") {
  SECTION("Hits North wall") {
    GasContainer container(0);
    std::pair<glm::vec2, glm::vec2> test_particle(vec2(200,101),vec2(0,-5));
    container.AddParticles(test_particle);
    container.AdvanceOneFrame();
    REQUIRE(vec2(0,5) == container.GetParticles()[0].second);
    REQUIRE(vec2(200,106) == container.GetParticles()[0].first);
  }
  SECTION("Hits South wall") {
    GasContainer container(0);
    std::pair<glm::vec2, glm::vec2> test_particle(vec2(200,599),vec2(0,5));
    container.AddParticles(test_particle);
    container.AdvanceOneFrame();
    REQUIRE(vec2(0,-5) == container.GetParticles()[0].second);
    REQUIRE(vec2(200,594) == container.GetParticles()[0].first);
  }
  SECTION("Hits East wall") {
    GasContainer container(0);
    std::pair<glm::vec2, glm::vec2> test_particle(vec2(599,200),vec2(5,0));
    container.AddParticles(test_particle);
    container.AdvanceOneFrame();
    REQUIRE(vec2(-5,0) == container.GetParticles()[0].second);
    REQUIRE(vec2(594,200) == container.GetParticles()[0].first);
  }
  SECTION("Hits West wall") {
    GasContainer container(0);
    std::pair<glm::vec2, glm::vec2> test_particle(vec2(101,200),vec2(-5,0));
    container.AddParticles(test_particle);
    container.AdvanceOneFrame();
    REQUIRE(vec2(5,0) == container.GetParticles()[0].second);
    REQUIRE(vec2(106,200) == container.GetParticles()[0].first);
  }
  SECTION("Collides in the corner") {
    GasContainer container(0);
    std::pair<glm::vec2, glm::vec2> test_particle(vec2(101,101),vec2(-3,-3));
    container.AddParticles(test_particle);
    container.AdvanceOneFrame();
    REQUIRE(vec2(3,3) == container.GetParticles()[0].second);
    REQUIRE(vec2(104,104) == container.GetParticles()[0].first);
  }
}