#include "geometry.hpp"

#include <algorithm>

using namespace std;

Vector2D Vector2D::operator+(const Vector2D& other) const { 
    return {x_ + other.x(), y_ + other.y()};
}

Vector2D& Vector2D::operator+=(const Vector2D& other) { 
    x_ += other.x(); y_ += other.y();
    return *this; 
}

Vector2D Vector2D::operator-(const Vector2D& other) const { 
    return {x_ - other.x(), y_ - other.y()};
}

Vector2D& Vector2D::operator-=(const Vector2D& other) { 
    x_ -= other.x(); y_ -= other.y();
    return *this; 
}

Vector2D Vector2D::operator*(float scalar) const {
    return {x_ * scalar, y_ * scalar};
}

Vector2D& Vector2D::operator*=(float scalar) {
    x_ *= scalar; y_ *= scalar;
    return *this;
}

float Vector2D::length() const {
    return sqrt(x_ * x_ + y_ * y_); 
}

namespace geometry {

    CollisionInfo circle_vs_AABB(
        Vector2D circle_center, 
        float radius,
        Vector2D rectangle_top_left, 
        float width, 
        float height
    ) {
        const Vector2D rectangle_min = rectangle_top_left;
        const Vector2D rectangle_max(rectangle_top_left.x() + width, rectangle_top_left.y() + height);
    
        // Closest point on the box to the circle's center.
        const float closest_x = clamp(circle_center.x(), rectangle_min.x(), rectangle_max.x());
        const float closest_y = clamp(circle_center.y(), rectangle_min.y(), rectangle_max.y());
        const Vector2D closest(closest_x, closest_y);
    
        const Vector2D delta = circle_center - closest;
        const float distance = delta.length();
    
        CollisionInfo info;
        if (distance < radius) {
            info.collided = true;
            // If the center is exactly on the boundary (dist == 0, i.e. the
            // circle's center is inside the box), fall back to an arbitrary
            // push direction rather than dividing by zero.
            const Vector2D direction = distance > 0.001f 
                ? delta * (1.f / distance) 
                : Vector2D(0.f, -1.f);
            info.minimum_translation = direction * (radius - distance);
        }
        return info;
    }

    CollisionInfo AABB_vs_AABB(
        Vector2D posA, float wA, float hA, 
        Vector2D posB, float wB, float hB
    ) {
        CollisionInfo info;

        const float aMinX = posA.x(), aMaxX = posA.x() + wA;
        const float aMinY = posA.y(), aMaxY = posA.y() + hA;
        const float bMinX = posB.x(), bMaxX = posB.x() + wB;
        const float bMinY = posB.y(), bMaxY = posB.y() + hB;

        const float overlapX = std::min(aMaxX, bMaxX) - std::max(aMinX, bMinX);
        const float overlapY = std::min(aMaxY, bMaxY) - std::max(aMinY, bMinY);

        if (overlapX > 0.f && overlapY > 0.f) {
            info.collided = true;
            // Separate along the shallowest axis
            if (overlapX < overlapY) {
                const float sign = (posA.x() + wA / 2.f < posB.x() + wB / 2.f) ? -1.f : 1.f;
                info.minimum_translation = Vector2D(sign * overlapX, 0.f);
            } else {
                const float sign = (posA.y() + hA / 2.f < posB.y() + hB / 2.f) ? -1.f : 1.f;
                info.minimum_translation = Vector2D(0.f, sign * overlapY);
            }
        }
        return info;
    }

    CollisionInfo circle_vs_circle(
        Vector2D circle_A_center, float circle_A_radius, 
        Vector2D circle_B_center, float circle_B_radius
    ) {
        const Vector2D delta = circle_A_center - circle_B_center;
        const float distance = delta.length();
        const float radius_sum = circle_A_radius + circle_B_radius;
        CollisionInfo info;

        if (distance < radius_sum) {
            info.collided = true;
            // Same fallback as circle_vs_AABB: avoid dividing by zero when
            // the two centers coincide exactly.
            const Vector2D direction = distance > 0.001f
                ? delta * (1.f / distance)
                : Vector2D(0.f, -1.f);
            info.minimum_translation = direction * (radius_sum - distance);
        }
        return info;
    }   
}

void Circle::draw(gfx::Window& win, Vector2D pos, gfx::Color color) const {
    win.fillCircle(
        int(pos.x()), int(pos.y()), 
        int(radius_), 
        color
    );
    win.drawCircle(
        int(pos.x()), int(pos.y()), 
        int(radius_), 
        scale_brightness(color, 0.8)
    );
}

Vector2D Circle::center_offset() const {
    return Vector2D(0.f, 0.f);
}

CollisionInfo Circle::collide_with(
    const Shape& other, 
    Vector2D selfPos, 
    Vector2D otherPos
) const {
    return other.collide_with_circle(*this, selfPos, otherPos);
}

CollisionInfo Circle::collide_with_circle(
    const Circle& other, 
    Vector2D other_position, 
    Vector2D self_position
) const {
    return geometry::circle_vs_circle(
        self_position, radius_,
        other_position, other.radius()
    );
}

CollisionInfo Circle::collide_with_rectangle(
    const Rectangle& other, 
    Vector2D other_position, 
    Vector2D self_position
) const {
    return geometry::circle_vs_AABB(
        self_position, radius_, 
        other_position, other.width(), other.height()
    );
}

float Circle::radius() const { 
    return radius_; 
}

void Rectangle::draw(gfx::Window& win, Vector2D pos, gfx::Color color) const {
    win.fillRect(
        int(pos.x()), int(pos.y()), 
        int(width_), int(height_), 
        color
    );
    win.drawRect(
        int(pos.x()), int(pos.y()), 
        int(width_), int(height_), 
        gfx::scale_brightness(color, 0.8)
    );
}

Vector2D Rectangle::center_offset() const {
    return Vector2D(width_ / 2, height_ / 2);
}

CollisionInfo Rectangle::collide_with(
    const Shape& other, 
    Vector2D self_position, 
    Vector2D other_position
) const {
    return other.collide_with_rectangle(
        *this, 
        self_position, 
        other_position
    );
}

CollisionInfo Rectangle::collide_with_circle(
    const Circle& other, 
    Vector2D other_position, 
    Vector2D self_position
) const {
    return geometry::circle_vs_AABB(
        other_position, other.radius(), 
        self_position, width_, height_
    );
}

CollisionInfo Rectangle::collide_with_rectangle(
    const Rectangle& other, 
    Vector2D other_position, 
    Vector2D self_position
) const {
    return geometry::AABB_vs_AABB(
        self_position, width_, height_, 
        other_position, other.width(), other.height()
    );
}

float Rectangle::width() const { 
    return width_; 
}

float Rectangle::height() const { 
    return height_; 
}


