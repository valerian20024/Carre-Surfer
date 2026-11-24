#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "geometry.hpp"

class Bus;
class DrunkStudent;
class Entity;
class Kebab;
class Obstacle;
class Player;

/// @brief Double dispatch visitor used to resolve entity-entity collisions.
class CollisionVisitor {
public:
    virtual ~CollisionVisitor() = default;

    virtual void visit(Bus& bus) = 0;
    virtual void visit(DrunkStudent& bus) = 0;
    virtual void visit(Kebab& kebab) = 0;
    virtual void visit(Obstacle& obstacle) = 0;
    virtual void visit(Player& player) = 0;
};

/// @brief The second entity (colliding with the first one) visitor.
template <class T>
class SecondEntityVisitor : public CollisionVisitor {
public:
    SecondEntityVisitor(T& first, CollisionInfo info) : first_(first), info_(info) {}

    void visit(Bus& second)             override { resolve(first_, second, info_); }
    void visit(DrunkStudent& second)    override { resolve(first_, second, info_); }
    void visit(Kebab& second)           override { resolve(first_, second, info_); }
    void visit(Obstacle& second)        override { resolve(first_, second, info_); }
    void visit(Player& second)          override { resolve(first_, second, info_); }

private:
    T& first_;
    CollisionInfo info_;
};

/// @brief The first entity (colliding with the second one) visitor.
class FirstEntityVisitor : public CollisionVisitor {
public:
    FirstEntityVisitor(Entity& other, CollisionInfo info) : other_(other), info_(info) {}

    void visit(Bus& first) override;
    void visit(DrunkStudent& first) override;
    void visit(Kebab& first) override;
    void visit(Obstacle& first) override;
    void visit(Player& first) override;

private:
    Entity& other_;
    CollisionInfo info_;
};

CollisionInfo flip(CollisionInfo info);

void resolve(Bus& first,            Bus& second,             CollisionInfo info);
void resolve(Bus& bus,              Obstacle& obstacle,      CollisionInfo info);

void resolve(DrunkStudent& student, Player& player,          CollisionInfo info);

void resolve(Entity& first,         Entity& second,          CollisionInfo info);

void resolve(Kebab& kebab,          Player& player,          CollisionInfo info);

void resolve(Obstacle& obstacle,    Bus& bus,                CollisionInfo info);
void resolve(Obstacle& first,       Obstacle& second,        CollisionInfo info);
void resolve(Obstacle& obstacle,    Player& player,          CollisionInfo info);

void resolve(Player& player,        Bus& bus,                CollisionInfo info);
void resolve(Player& player,        DrunkStudent& student,   CollisionInfo info);
void resolve(Player& player,        Kebab& kebab,            CollisionInfo info);
void resolve(Player& player,        Obstacle& obstacle,      CollisionInfo info);
void resolve(Player& first,         Player& second,          CollisionInfo info);

#endif
