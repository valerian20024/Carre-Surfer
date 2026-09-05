#include "cooldown.hpp"

#include <iostream>

void Cooldown::start(float seconds) {
    if (seconds < 0.f) {
        std::cerr << "seconds must be greater than 0.0f" << std::endl;
        remaining_ = 0.f;
    }
    remaining_ = seconds; 
}

bool Cooldown::tick(float dt) {
    if (remaining_ <= 0.f) 
        return false;
    
    remaining_ -= dt;

    // Cooldown succesfully ticked and reached zero.
    if (remaining_ <= 0.f) { 
        remaining_ = 0.f; 
        return true;
    }
    
    return false;
}

bool Cooldown::is_active() const { 
    return remaining_ > 0.f; 
}