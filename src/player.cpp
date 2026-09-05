#include "player.hpp"
#include "helpers.hpp"

#include <algorithm>

using namespace std;

void Player::update(float dt) {
    update_stagger(dt);

    if (invincibility_.tick(dt)) {
        visible_ = true;
    }

    if (invincibility_.is_active()) {
        if (blinking_.tick(dt)) {
            visible_ = !visible_;
            blinking_.start(BLINK_INTERVAL);
        }
    }

    reversed_.tick(dt);
}

void Player::take_damage(int amount) {
    if (invincibility_.is_active())
        return;

    healthpoints_ = clamp(healthpoints_ - amount, 0, MAX_HEALTH);
    invincibility_.start(INVINCIBILITY_DURATION);
    blinking_.start(BLINK_INTERVAL);
    has_been_hit_ = true;
}

void Player::heal(int amount) {
    healthpoints_ = clamp(healthpoints_ + amount, 0, MAX_HEALTH);
}

float Player::health_percentage() const {
    return float(healthpoints_) / MAX_HEALTH;
}

void Player::move_left() {
    move_step(Vector2D{-STEP_DISTANCE, 0.f});
}

void Player::move_right() {
    move_step(Vector2D{STEP_DISTANCE, 0.f});
}

void Player::move_up() {
    move_step(Vector2D{0.f, -STEP_DISTANCE});
}

void Player::move_down() {
    move_step(Vector2D{0.f, STEP_DISTANCE});
}

void Player::accept(CollisionVisitor& visitor) {
    return visitor.visit(*this);
}

void Player::reverse_movement() {
    reversed_.start(random_float(MIN_REVERSED_DURATION, MAX_REVERSED_DURATION));
}

bool Player::consume_hit_event() {
    if (has_been_hit_) {
        has_been_hit_ = !has_been_hit_;
        return true;
    }

    return false;
}

void Player::move_step(Vector2D direction) {
    if (reversed_.is_active())
        direction *= -1.f;
    
    Vector2D target = clamp_to_board(position_ + direction);
    translate(target - position_);
}

Vector2D Player::clamp_to_board(Vector2D position) const {
    return Vector2D(
        clamp(position.x(), 0.f, board_width_ - SIZE),
        clamp(position.y(), 0.f, board_height_ - SIZE)
    );
}

void Player::update_stagger(float dt) {
    // Not currently staggering. Deciding new burst parameters.
    if (stagger_interval_.tick(dt)) {
        stagger_steps_left_ = random_int(MIN_STAGGER_STEPS, MAX_STAGGER_STEPS);
        stagger_direction_  *= random_bool() ? 1.f : -1.f;
        stagger_step_.start(STAGGER_STEP_INTERVAL);
    }

    // Currently staggering.
    if (stagger_steps_left_ > 0 && stagger_step_.tick(dt)) {
        Vector2D vertical_step(
            0.f,
            stagger_direction_ * STEP_DISTANCE
        );

        Vector2D target = clamp_to_board(position_ + vertical_step);
        translate(target - position_);
        
        --stagger_steps_left_;
        if (stagger_steps_left_ > 0)
            stagger_step_.start(STAGGER_STEP_INTERVAL);
        else
            stagger_interval_.start(next_stagger_gap());
    }
}

float Player::next_stagger_gap() const {
    return max(MIN_STAGGER_GAP, random_normal(STAGGER_MEAN_GAP, STAGGER_GAP_STDDEV));
}

void Player::reset() {
    set_position(initial_position_);
    healthpoints_ = 100;
    has_been_hit_ = false;   
}