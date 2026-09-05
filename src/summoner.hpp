#ifndef SUMMONER_HPP
#define SUMMONER_HPP

#include "cooldown.hpp"
#include "obstacles.hpp"

#include <random>
#include <memory>
#include <vector>
#include <functional>

class Summoner {
public:
    Summoner(float screen_width, float screen_height);

    /// @brief Updates the Summoner's state, maybe resulting in the spawning of a new Obstacle.
    /// @param dt The timestep update.
    /// @return A newly generated Obstacle or nullptr if none was generated.
    std::unique_ptr<Obstacle> update(float dt);

private:
    static constexpr float MIN_INTERVAL = 0.01f;
    static constexpr float MAX_INTERVAL = 0.8f;
    static constexpr float SCREEN_RIGHT_OFFSET = 200.f;
    float screen_width_ = 0.f, screen_height_ = 0.f;
    
    Cooldown cooldown_{};
    std::mt19937 rng_;

    /// @return The next time interval before spawning an Obstacle.
    float next_interval();

    /// @brief Computes the position of the newly spawned Obstacle.
    /// @return The newly generated Obstacle.
    std::unique_ptr<Obstacle> spawn();

    /// @brief Helper deciding which Obstacle type is generated at this position.
    /// @return The newly generated Obstacle.
    std::unique_ptr<Obstacle> make_obstacle(Vector2D position);
};

#endif
