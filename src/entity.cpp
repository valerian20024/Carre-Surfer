#include "entity.hpp"

using namespace std;

void Entity::move(float dt) {
    speed_ += acceleration_ * dt;
    position_ += speed_ * dt;
}

void Entity::translate(Vector2D delta) { 
    position_ += delta; 
}

void Entity::render(gfx::Window& win, const RenderContext& ctx) const {
    if (!visible_) 
        return;

    Vector2D draw_position = position_ + ctx.offset;
    Vector2D center = draw_position + shape_->center_offset();

    shape_->draw(win, draw_position, gfx::scale_brightness(color_, ctx.brightness));

    for (auto& layer : text_layers())
        write_centered(
            win, 
            center + layer.offset, 
            layer.direction, 
            layer.message, 
            layer.pixel_size,
            gfx::scale_brightness(layer.color, ctx.brightness)
        );
}

vector<TextLayer> Entity::text_layers() const { return {}; }

CollisionInfo Entity::collide_with(const Entity& other) const {
    return shape_->collide_with(*other.shape_, position_, other.position_);
}

bool Entity::is_dead() const { 
    return dead_; 
}

void Entity::mark_dead() { 
    dead_ = true; 
}

void Entity::freeze() {
    speed_ = {0.f, 0.f};
    acceleration_ = {0.f, 0.f};
}

void Entity::set_position(Vector2D absolute_position) {
    position_ = absolute_position;
}