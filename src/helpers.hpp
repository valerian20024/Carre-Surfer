#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <random>

inline float random_float(float min, float max) {
    static std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> dist(min, max);
    
    return dist(rng);
}

inline float random_int(int min, int max) {
    return int(random_float(float(min), float(max)));
}

inline float random_normal(float mean, float stddev) {
    static std::mt19937 rng{std::random_device{}()};
    std::normal_distribution<float> dist(mean, stddev);

    return dist(rng);
}

inline bool random_bool() {
    static std::mt19937 rng{std::random_device{}()};
    std::bernoulli_distribution dist(0.5);
    return dist(rng);
}

#endif