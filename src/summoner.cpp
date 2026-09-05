#include "summoner.hpp"

Summoner::Summoner(
    float screen_width, 
    float screen_height
) : 
    screen_width_(screen_width), 
    screen_height_(screen_height),
    rng_(std::random_device{}()) 
{
    cooldown_.start(next_interval());
}

std::unique_ptr<Obstacle> Summoner::update(float dt) {
    if (cooldown_.tick(dt)) {
        cooldown_.start(next_interval());
        return spawn();
    }

    return nullptr;
}

float Summoner::next_interval() {
    std::uniform_real_distribution<float> dist(
        MIN_INTERVAL, 
        MAX_INTERVAL
    );
    return dist(rng_);
}

std::unique_ptr<Obstacle> Summoner::spawn() {
    std::uniform_real_distribution<float> vertical_distance(
        0.f, 
        screen_height_
    );

    Vector2D position(
        screen_width_ + SCREEN_RIGHT_OFFSET, 
        vertical_distance(rng_)
    );

    return make_obstacle(position);
}

std::unique_ptr<Obstacle> Summoner::make_obstacle(Vector2D position) {
    
    using Factory = std::function<std::unique_ptr<Obstacle>(Vector2D)>;
    
    static const std::vector<Factory> factories = {
        [](Vector2D pos) { return std::make_unique<Kebab>(pos); },
        [](Vector2D pos) { return std::make_unique<Garbage>(pos); },
        [](Vector2D pos) { return std::make_unique<Scooter>(pos); },
        [](Vector2D pos) { return std::make_unique<DrunkStudent>(pos); },
        [](Vector2D pos) { return std::make_unique<Bus>(pos); }
    };

    // These do not need to sum to 1.0 as discrete_distribution 
    // normalizes them. Bus coefficient then takes a small part of
    // every probabilities, not changing much the original game's logic.
    std::discrete_distribution<size_t> dist({
        20.0, // Kebab
        30.0, // Garbage
        10.0, // Scooter
        40.0, // DrunkStudent
         5.0  // Bus
    });

    size_t index = dist(rng_);
    return factories[index](position);
}