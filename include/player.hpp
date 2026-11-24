#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "collision.hpp"
#include "cooldown.hpp"
#include "entity.hpp"
#include "Window.hpp"

#include <iostream>

class Player : public Entity {
public:
    Player(Vector2D position, float board_width, float board_height) : 
        Entity(
            position,
            {0.f, 0.f},
            {0.f, 0.f},
            std::make_unique<Rectangle>(SIZE, SIZE), 
            COLOR
        ),
        board_width_(board_width),
        board_height_(board_height)
    {
        initial_position_ = position;
        stagger_interval_.start(next_stagger_gap());
    }

    /// @brief Update the player time-based state.
    void update(float dt);

    /// @brief Deals damage to the player
    /// @param amount a positive number of damage.
    void take_damage(int amount);

    /// @brief Restore some player health.
    /// @param amount a positive number of health points.
    void heal(int amount);
    
    /// @return The player's health as a percentage.
    float health_percentage() const;

    /// @brief Makes the player move left for one step.
    void move_left();

    /// @brief Makes the player move right for one step.
    void move_right();

    /// @brief Makes the player move up for one step.
    void move_up();

    /// @brief Makes the player move down for one step.
    void move_down();

    void accept(CollisionVisitor& visitor) override;
    
    /// @brief Makes the player move the other way around for a given duration.
    void reverse_movement();

    /// @returns True after the player has been hit, only once per hit.
    bool consume_hit_event();

    /// @brief Makes the player move to its default position.
    void reset();

private:
    static constexpr int    SIZE = 40.f;
    static constexpr gfx::Color COLOR = {255, 255, 255, 255};
    static constexpr int    MAX_HEALTH = 100;
    
    static constexpr float  INVINCIBILITY_DURATION = 2.f;
    static constexpr float  BLINK_INTERVAL = 0.1f;

    static constexpr float  MIN_REVERSED_DURATION = 1.f;
    static constexpr float  MAX_REVERSED_DURATION = 5.f;
    
    static constexpr float  STEP_DISTANCE = 20.f;
    static constexpr int    MIN_STAGGER_STEPS = 1;
    static constexpr int    MAX_STAGGER_STEPS = 5;
    static constexpr float  MIN_STAGGER_GAP = 0.5f;
    static constexpr float  STAGGER_MEAN_GAP = 2.f;
    static constexpr float  STAGGER_GAP_STDDEV = 0.4f;
    static constexpr float  STAGGER_STEP_INTERVAL = 0.1f;
    
    bool    has_been_hit_ = false;
    float   board_width_  = 0.f;
    float   board_height_ = 0.f;
    int     healthpoints_ = MAX_HEALTH;
    Vector2D initial_position_{};

    Cooldown blinking_{};
    Cooldown invincibility_{};
    Cooldown reversed_{};
    
    Cooldown    stagger_interval_{};
    Cooldown    stagger_step_{};
    int         stagger_steps_left_ = 0;
    float       stagger_direction_  = 1.f;  // +1 = down, -1 = up

    /// @brief Makes the player move down for one step along the given direction.
    void move_step(Vector2D direction);

    /// @brief Clamps tthe player position to the board sizes.
    Vector2D clamp_to_board(Vector2D position) const;

    /// @brief Updates the time-related variables linked to staggering.
    void update_stagger(float dt);

    /// @brief Computes the time interval until the next stagger.
    /// @return A time interval at least as big as MIN_STAGGER_GAP. 
    float next_stagger_gap() const; 
};

#endif
