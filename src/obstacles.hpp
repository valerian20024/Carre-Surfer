#ifndef OBSTACLES_HPP
#define OBSTACLES_HPP

#include "entity.hpp"
#include "geometry.hpp"

class Vector2D;

class Obstacle : public Entity {
public:
    static constexpr float DEFAULT_HORIZONTAL_SPEED = -80.f;
    virtual ~Obstacle() = default;
    
    // Full control constructor.
    Obstacle(
        Vector2D position, 
        Vector2D speed, 
        Vector2D acceleration,
        std::unique_ptr<Shape> shape, 
        gfx::Color color, 
        int damage
    );

    // Constructor assuming default speed and acceleration.
    Obstacle(
        Vector2D position, 
        std::unique_ptr<Shape> shape, 
        gfx::Color color, 
        int damage
    );    

    void accept(CollisionVisitor& visitor) override;

    int damage() const;

private:
    int damage_ = 0;
};

class Kebab : public Obstacle {
public:
    static constexpr float RADIUS = 7.5f;
    static constexpr gfx::Color COLOR{251, 179, 55};  // Sunflower Yellow.
    static constexpr int DAMAGE = 0;
    static constexpr int HEAL_AMOUNT = 10;

    Kebab(Vector2D position);
    Kebab();

    void accept(CollisionVisitor& visitor) override;

private:
    std::vector<TextLayer> text_layers() const override;
};

class Garbage : public Obstacle {
public:
    static constexpr float WIDTH = 30.f;
    static constexpr float HEIGHT = 45.f;
    static constexpr gfx::Color COLOR{95, 94, 110};  // Dim Grey.
    static constexpr int DAMAGE = 10;

    Garbage(Vector2D position);
    Garbage();

    void accept(CollisionVisitor& visitor) override;

private:
    std::vector<TextLayer> text_layers() const override;
};

class Scooter : public Obstacle {
public:
    static constexpr float WIDTH = 15.f;
    static constexpr float HEIGHT = 45.f;
    static constexpr gfx::Color COLOR{127, 197, 89};  // Moss Green.
    static constexpr int DAMAGE = 30;

    Scooter(Vector2D position);
    Scooter();

    void accept(CollisionVisitor& visitor) override;

private:
    std::vector<TextLayer> text_layers() const override;
};

class DrunkStudent : public Obstacle {
public:
    static constexpr int MIN_DIMENSION = 20.f;
    static constexpr int MAX_DIMENSION = 40.f;
    static constexpr gfx::Color COLOR{124, 112, 215};  // Medium Slate Blue.
    static constexpr int DAMAGE = 30;

    DrunkStudent(Vector2D position);
    DrunkStudent();

    void accept(CollisionVisitor& visitor) override;

private:
    float dimension_ = 0.f;
    
    /// @brief Delegate constructor: dimension is generated once by the public
    /// constructor and reused both for the shape and for dimension_.
    DrunkStudent(Vector2D position, float dimension);


    /// @brief Creates a new Rectangle with random, bounded size.
    static std::unique_ptr<Rectangle> random_size_shape();

    std::vector<TextLayer> text_layers() const override;

    int calculate_pixel_size(const std::string& text, int max_width, int max_height) const;
};

class Bus : public Obstacle {
public:
    static constexpr float WIDTH = 150.f, HEIGHT = 500.f;
    static constexpr float MIN_VERTICAL_SPEED = -50.f;
    static constexpr float MAX_VERTICAL_SPEED = 50.f;
    static constexpr float MIN_VERTICAL_ACCELERATION = -50.f;
    static constexpr float MAX_VERTICAL_ACCELERATION = 50.f;
    static constexpr gfx::Color COLOR{242, 203, 64};  // Tuscan Sun.
    static constexpr int DAMAGE = 10;
    

    Bus(Vector2D position, float vertical_speed, float vertical_acceleration);
    Bus(Vector2D position);
    Bus();

    void accept(CollisionVisitor& visitor) override;

    /// @brief Updates the Bus position and speed according to physics, while letting it stop when speed reaches zero.
    /// @param dt A discrete timestep.
    void move(float dt) override;

private:
    int line_number_ = 0;

    std::vector<TextLayer> text_layers() const override;
};


#endif
