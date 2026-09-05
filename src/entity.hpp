#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "collision.hpp"
#include "geometry.hpp"
#include "text.hpp"
#include "Window.hpp"

#include <vector>

/// @brief External parameters for rendering entities.
struct RenderContext {
    float brightness = 1.f;
    Vector2D offset{};
};

/// @brief The Entity's text layers to be displayed when rendering.
struct TextLayer {
    std::string     message;
    Direction       direction = Direction::Right;
    int             pixel_size = 1;
    gfx::Color      color{};
    Vector2D        offset{};
};

/// @brief Represents a 2D object subject to physics, collisions, and rendering.
class Entity {
public:
    Entity() = default;
    virtual ~Entity() = default;

    Entity(
        Vector2D pos, 
        Vector2D speed, 
        Vector2D acceleration, 
        std::unique_ptr<Shape> shape, 
        gfx::Color color
    ) : 
        position_(pos), 
        speed_(speed), 
        acceleration_(acceleration), 
        shape_(std::move(shape)), 
        color_(color) 
    {}

    /// @brief Updates the entity's position and speed according to physics.
    /// @param dt discrete timestep. 
    virtual void move(float dt);
    
    /// @brief Instantaneously translate Entity's position.
    /// @param delta a position delta.
    void translate(Vector2D delta);

    /// @brief Prevents the Entity to move, zeroing its speed and acceleration.
    void freeze();

    /// @brief Renders the entity to the window, given some external parameters.
    /// @param win The window to render to.
    /// @param ctx The external rendering context.
    void render(gfx::Window& win, const RenderContext& ctx) const;

    /// @brief This method returns some layers of text to be displayed when rendering the entity.
    /// @return The text layers related to this entity's rendering.
    virtual std::vector<TextLayer> text_layers() const;

    CollisionInfo collide_with(const Entity& other) const;

    virtual void accept(CollisionVisitor& visitor) = 0;

    bool is_dead() const;
    
    void mark_dead();

protected:
    Vector2D position_      = {};
    Vector2D speed_         = {};
    Vector2D acceleration_  = {};
    std::unique_ptr<Shape> shape_ = std::make_unique<Rectangle>();
    gfx::Color color_ = {};

    bool visible_ = true;    

    virtual void set_position(Vector2D absolute_position);

private:
    bool dead_ = false;

};

#endif
