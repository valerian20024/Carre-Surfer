#include "collision.hpp"

#include "entity.hpp"
#include "player.hpp"
#include "obstacles.hpp"

#include <iostream>

using namespace std;

void FirstEntityVisitor::visit(Bus& first) { 
    SecondEntityVisitor<Bus> second(first, info_);
    other_.accept(second); 
}

void FirstEntityVisitor::visit(DrunkStudent& first) { 
    SecondEntityVisitor<DrunkStudent> second(first, info_);
    other_.accept(second); 
}

void FirstEntityVisitor::visit(Kebab& first) { 
    SecondEntityVisitor<Kebab> second(first, info_);
    other_.accept(second); 
}

void FirstEntityVisitor::visit(Obstacle& first) { 
    SecondEntityVisitor<Obstacle> second(first, info_);
    other_.accept(second); 
}

void FirstEntityVisitor::visit(Player& first) { 
    SecondEntityVisitor<Player> second(first, info_);
    other_.accept(second); 
}

CollisionInfo flip(CollisionInfo info) {
    info.minimum_translation *= -1.f;
    return info;
}

void resolve(Bus& first, Bus& second, CollisionInfo info) {
    resolve(static_cast<Obstacle&>(first), static_cast<Obstacle&>(second), info);
}

void resolve(Bus&, Obstacle& obstacle, CollisionInfo) {
    obstacle.mark_dead();
}

void resolve(DrunkStudent& student, Player& player, CollisionInfo info) {
    player.reverse_movement();
    resolve(static_cast<Obstacle&>(student), player, info);
}

void resolve(Entity& first, Entity& second, CollisionInfo info) {
    first.translate(info.minimum_translation * -0.5f);
    second.translate(info.minimum_translation * 0.5f);
}

void resolve(Kebab& kebab, Player& player, CollisionInfo) {
    kebab.mark_dead();
    player.heal(Kebab::HEAL_AMOUNT);
}

void resolve(Obstacle& obstacle, Bus& bus, CollisionInfo info) {
    resolve(bus, obstacle, flip(info));
}

void resolve(Obstacle& first, Obstacle& second, CollisionInfo info) {
    resolve(static_cast<Entity&>(first), static_cast<Entity&>(second), info);
}

void resolve(Obstacle& obstacle, Player& player, CollisionInfo info) {
    player.take_damage(obstacle.damage());
    player.translate(info.minimum_translation);
}

void resolve(Player& player, Bus& bus, CollisionInfo info) {
    resolve(bus, player, flip(info));
}

void resolve(Player& player, DrunkStudent& student, CollisionInfo info) {
    resolve(student, player, flip(info));
}

void resolve(Player& player, Kebab& kebab, CollisionInfo info) {
    resolve(kebab, player, flip(info));
}

void resolve(Player& player, Obstacle& obstacle, CollisionInfo info) {
    resolve(obstacle, player, flip(info));
}

void resolve(Player& first, Player& second, CollisionInfo info) {
    resolve(static_cast<Entity&>(first), static_cast<Entity&>(second), flip(info));
}
