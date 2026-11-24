#ifndef COOLDOWN_HPP
#define COOLDOWN_HPP

/// @brief A cooldown timer.
class Cooldown {
public:
    /// @brief Sets up the timer count.
    /// @param seconds A positive number of seconds.
    void start(float seconds);

    /// @brief Removes some time from the cooldown.
    /// @param dt The timestep to substract.
    /// @return true iff the timer succesfully ticked and reached zero, false otherwise. 
    bool tick(float dt);

    /// @brief Whether the timer has reached zero.
    /// @return true when the timer is still running, false when it reached zero.
    bool is_active() const;

private:
    float remaining_ = 0.f;
};

#endif
