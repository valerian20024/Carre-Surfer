#include "obstacles.hpp"

#include "helpers.hpp"

#include <algorithm>
#include <iostream>

using namespace std;

Obstacle::Obstacle(
    Vector2D position, 
    Vector2D speed, 
    Vector2D acceleration,
    std::unique_ptr<Shape> shape, 
    gfx::Color color, 
    int damage
) : 
    Entity(
        position, 
        speed, 
        acceleration, 
        std::move(shape), 
        color
    ), 
    damage_(damage) 
{}

Obstacle::Obstacle(
    Vector2D position, 
    unique_ptr<Shape> shape, 
    gfx::Color color, 
    int damage
) : 
    Obstacle(
        position, 
        Vector2D(DEFAULT_HORIZONTAL_SPEED, 0.f), 
        Vector2D(0.f, 0.f), 
        std::move(shape), 
        color,
        damage
    )
{}

void Obstacle::accept(CollisionVisitor& visitor) { 
    visitor.visit(*this); 
}

int Obstacle::damage() const { 
    return damage_; 
}

Kebab::Kebab(
    Vector2D position
) : 
    Obstacle(
        position, 
        std::make_unique<Circle>(RADIUS), 
        COLOR, 
        DAMAGE
    ) 
{}

Kebab::Kebab() : Kebab(Vector2D(0.f, 0.f)) {} 

void Kebab::accept(CollisionVisitor& visitor) {
    visitor.visit(*this); 
}

vector<TextLayer> Kebab::text_layers() const {
    const string main = "k";
    constexpr gfx::Color TEXT_COLOR{150, 110, 5};
    constexpr int MAIN_PIXEL_SIZE = 1;

    return {
        TextLayer{
            main,
            Direction::Right,
            MAIN_PIXEL_SIZE,
            TEXT_COLOR
        }
    };
}
    
Garbage::Garbage(
    Vector2D position
) : 
    Obstacle(
        position, 
        std::make_unique<Rectangle>(WIDTH, HEIGHT), 
        COLOR, 
        DAMAGE
    ) 
{}

Garbage::Garbage() : Garbage(Vector2D(0.f, 0.f)) {} 

void Garbage::accept(CollisionVisitor& visitor) { 
    visitor.visit(*this); 
}

vector<TextLayer> Garbage::text_layers() const {
    const string main = "|||";
    constexpr gfx::Color TEXT_COLOR{35, 35, 35};
    constexpr int MAIN_PIXEL_SIZE = 3;

    return {
        TextLayer{
            main,
            Direction::Down,
            MAIN_PIXEL_SIZE,
            TEXT_COLOR
        }
    };
}

Scooter::Scooter(
    Vector2D position
) : 
    Obstacle(
        position, 
        std::make_unique<Rectangle>(WIDTH, HEIGHT),
        COLOR, 
        DAMAGE
    ) 
{}

Scooter::Scooter() : Scooter(Vector2D(0.f, 0.f)) {} 

void Scooter::accept(CollisionVisitor& visitor) { 
    visitor.visit(*this); 
}

vector<TextLayer> Scooter::text_layers() const {
    const string main = "|||o";
    constexpr gfx::Color TEXT_COLOR{5, 120, 5};
    constexpr int MAIN_PIXEL_SIZE = 2;

    return {
        TextLayer{
            main,
            Direction::Down,
            MAIN_PIXEL_SIZE,
            TEXT_COLOR
        }
    };
}

DrunkStudent::DrunkStudent(
    Vector2D position
) : 
    DrunkStudent(
        position, 
        random_float(MIN_DIMENSION, MAX_DIMENSION)
    )
{}

DrunkStudent::DrunkStudent(Vector2D position, float dimension) :
    Obstacle(
        position,
        std::make_unique<Rectangle>(dimension, dimension),
        COLOR,
        DAMAGE
    ),
    dimension_(dimension)
{}


DrunkStudent::DrunkStudent() : DrunkStudent(Vector2D(0.f, 0.f)) {} 

void DrunkStudent::accept(CollisionVisitor& visitor) { 
    visitor.visit(*this); 
}

unique_ptr<Rectangle> DrunkStudent::random_size_shape() {
    float size = random_float(MIN_DIMENSION, MAX_DIMENSION);
    return make_unique<Rectangle>(size, size);
}

vector<TextLayer> DrunkStudent::text_layers() const {
    const string main = ":)";
    constexpr gfx::Color TEXT_COLOR{25, 25, 190};
    
    const int MAIN_PIXEL_SIZE = calculate_pixel_size(
        main, 
        int(dimension_),
        int(dimension_)
    );

    return {
        TextLayer{
            main,
            Direction::Left,
            MAIN_PIXEL_SIZE,
            TEXT_COLOR
        }
    };
}

int DrunkStudent::calculate_pixel_size(
    const string& text, 
    int max_width, 
    int max_height
) const {
    if (text.empty() || max_width <= 0 || max_height <= 0)
        return 1;

    const int base_width = static_cast<int>(text.length()) * 6 - 1;
    const int base_height = 7;

    const int scale_x = max_width / base_width;
    const int scale_y = max_height / base_height;

    return max(1, min(scale_x, scale_y));
}

Bus::Bus(
    Vector2D position, 
    float vertical_speed, 
    float vertical_acceleration
) :
    Obstacle(
        position,
        Vector2D(DEFAULT_HORIZONTAL_SPEED, vertical_speed),
        Vector2D(0.f, vertical_acceleration),
        std::make_unique<Rectangle>(WIDTH, HEIGHT),
        COLOR, 
        DAMAGE
    ) 
{
    line_number_ = random_int(0, 99);
}

Bus::Bus(
    Vector2D position
) : 
    Bus(
        position,
        random_float(MIN_VERTICAL_SPEED, MAX_VERTICAL_SPEED),
        random_float(MIN_VERTICAL_ACCELERATION, MAX_VERTICAL_ACCELERATION)
    )
{}

Bus::Bus() : Bus(Vector2D(0.f, 0.f)) {}

void Bus::accept(CollisionVisitor& visitor) { 
    visitor.visit(*this); 
}

void Bus::move(float dt) {
    const float previous_vertical_speed = speed_.y();
    
    Entity::move(dt);

    const bool crossed_zero =
        (previous_vertical_speed > 0.f && speed_.y() < 0.f) ||
        (previous_vertical_speed < 0.f && speed_.y() > 0.f);

    if (crossed_zero) {
        speed_ = {speed_.x(), 0.f};
        acceleration_ = {acceleration_.x(), 0.f};
    }
}

std::vector<TextLayer> Bus::text_layers() const {
    const string main = "LE TEC - " + to_string(line_number_);
    const string sub  = "TELLEMENT EN COMMUN";
    
    constexpr gfx::Color TEXT_COLOR{242, 10, 10};

    constexpr int   MAIN_PIXEL_SIZE = 6;
    constexpr int   SUB_PIXEL_SIZE = 2;
    constexpr float PADDING = 40.f;

    return {
        TextLayer{
            main, 
            speed_.y() >= 0.f ? Direction::Down : Direction::Up, 
            MAIN_PIXEL_SIZE, 
            TEXT_COLOR
        }, 
        TextLayer{
            sub, 
            speed_.y() >= 0.f ? Direction::Down : Direction::Up, 
            SUB_PIXEL_SIZE, 
            TEXT_COLOR,
            speed_.y() >= 0.f 
                ? Vector2D(-PADDING, 0.f)
                : Vector2D( PADDING, 0.f), 
        }
    };
}