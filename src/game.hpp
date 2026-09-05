#ifndef GAME_HPP
#define GAME_HPP

#include "cooldown.hpp"
#include "obstacles.hpp"
#include "player.hpp"
#include "summoner.hpp"
#include "Window.hpp"

#include <memory>
#include <vector>

/// @brief Manages the state of the game.
class Game {
public:
    /// @brief Creates a new game.
    /// @param width The game window width. 
    /// @param height The game window width.
    /// @param fps The game frame per second.
    Game(int width, int height, float fps);

    /// @brief Runs the game.
    void run();

private:
    enum class State { Playing, GameOver };

    static constexpr float      SHAKE_DURATION = 1.f;
    static constexpr float      SHAKE_MAG = 10.f;
    static constexpr gfx::Color BACKGROUND_COLOR{255, 255, 255};
    static constexpr gfx::Color TEXT_COLOR{200, 20, 20};

    int W_{}, H_{};
    float fps_{}, dt_{};

    State state_ = State::Playing;
    float score_ = 0.f;

    gfx::Window win_;
    Player player_;
    Summoner summoner_;
    std::vector<std::unique_ptr<Entity>> obstacles_{};
    Cooldown shake_{};

    void process_input();

    void update(float dt);
    
    void handle_player_obstacle_collisions();
    
    void handle_obstacle_obstacle_collisions();
    
    void remove_dead_obstacles();
    
    Vector2D compute_shake_offset(float dt);
    
    void render(Vector2D shake_offset);

    void reset();
};

#endif
