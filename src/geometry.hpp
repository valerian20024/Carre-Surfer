#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "Window.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

class Circle;
class Rectangle;

class Vector2D {
public:
    Vector2D() = default;
    Vector2D(float x, float y): x_(x), y_(y) {}

    Vector2D  operator+ (const Vector2D& other) const;
    Vector2D& operator+=(const Vector2D& other);
    Vector2D  operator- (const Vector2D& other) const;
    Vector2D& operator-=(const Vector2D& other);
    Vector2D  operator* (float scalar) const;
    Vector2D& operator*=(float scalar);

    /// @return The vector euclidean length.
    float length() const;

    /// @return The vector horizontal component. 
    float x() const { return x_; }

    /// @return The vector vertical component.
    float y() const { return y_; }

private:
    float x_ = 0.f, y_ = 0.f;
};

/// @brief Result of a purely geometric collision test between two shapes.
struct CollisionInfo {
    bool collided = false;
 
    Vector2D minimum_translation{};
};

namespace geometry {
    CollisionInfo circle_vs_AABB(
        Vector2D circleCenter, float radius, 
        Vector2D rectTopLeft, float rectW, float rectH
    );

    CollisionInfo AABB_vs_AABB(
        Vector2D position1, float width1, float height1, 
        Vector2D position2, float width2, float height2
    );

    CollisionInfo circle_vs_circle(
        Vector2D circle_A_center, float circle_A_radius, 
        Vector2D circle_B_center, float circle_B_radius
    );
};

class Shape {
public:
    virtual ~Shape() = default;
    
    /// @brief Draw itself on the window.
    /// @param win The window. 
    /// @param pos The shape-dependent position.
    /// @param color The shape color.
    virtual void draw(gfx::Window& win, Vector2D pos, gfx::Color color) const = 0;

    /// @brief Returns a position offset corresponding to the shape's center relatively to its local position.
    virtual Vector2D center_offset() const = 0;

    virtual CollisionInfo collide_with(
        const Shape& other, 
        Vector2D selfPos, 
        Vector2D otherPos
    ) const = 0;

    virtual CollisionInfo collide_with_circle(
        const Circle& other, 
        Vector2D otherPos, 
        Vector2D selfPos
    ) const = 0;
    
    virtual CollisionInfo collide_with_rectangle(
        const Rectangle& other, 
        Vector2D otherPos, 
        Vector2D selfPos
    ) const = 0;
};

class Circle : public Shape {
public:
    Circle() = default;
    Circle(float radius) : radius_(radius) {}

    void draw(gfx::Window& win, Vector2D pos, gfx::Color color) const override;

    Vector2D center_offset() const;

    CollisionInfo collide_with(const Shape& other, Vector2D selfPos, Vector2D otherPos) const override;

    CollisionInfo collide_with_circle(const Circle& other, Vector2D other_position, Vector2D self_position) const override;

    CollisionInfo collide_with_rectangle(const Rectangle& other, Vector2D other_position, Vector2D self_position) const override;

    /// @return The Circle radius.
    float radius() const;

private:
    float radius_ = 0.f;
};

class Rectangle : public Shape {
public:
    Rectangle() = default;
    Rectangle(float width, float height) : width_(width), height_(height) {}

    void draw(gfx::Window& win, Vector2D pos, gfx::Color color) const override;

    Vector2D center_offset() const;

    CollisionInfo collide_with(const Shape& other, Vector2D self_position, Vector2D other_position) const override;

    CollisionInfo collide_with_circle(const Circle& other, Vector2D other_position, Vector2D self_position) const override;

    CollisionInfo collide_with_rectangle(const Rectangle& other, Vector2D other_position, Vector2D self_position) const override;

    /// @return The rectangle width (horizontal).
    float width() const;

    /// @return The rectangle height (vertical).
    float height() const;

private:
    float width_ = 0.f, height_ = 0.f;
};

#endif
