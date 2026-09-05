#include "game.hpp"

#include "collision.hpp"
#include "font.hpp"
#include "helpers.hpp"
#include "text.hpp"

#include <algorithm>
#include <iostream>

using namespace std;

Game::Game(
    int width, 
    int height, 
    float fps
) :
    W_(width), 
    H_(height), 
    fps_(fps), 
    dt_(1.f / fps),
    win_("Carré Surfer Deluxe Edition", width, height),
    player_(
        Vector2D(width / 2, height / 2), 
        float(width), 
        float(height)
    ),
    summoner_(float(width), float(height))
{}

void Game::run() {
    while (win_.isOpen()) {
        win_.pollEvents();

        if (win_.wasKeyPressed(gfx::Key::Escape))
            win_.close();

        if (state_ == State::Playing) {
            process_input();
            update(dt_);
        } else if (state_ == State::GameOver) {
            if (win_.wasKeyPressed(gfx::Key::Space))
                reset();
        }

        Vector2D shake_offset = compute_shake_offset(dt_);
        render(shake_offset);

        win_.present();
        win_.waitForNextFrame(int(fps_));
    }
}

void Game::process_input() {
    using namespace gfx;
    if (win_.isKeyDown(Key::Left))  player_.move_left();
    if (win_.isKeyDown(Key::Right)) player_.move_right();
    if (win_.isKeyDown(Key::Up))    player_.move_up();
    if (win_.isKeyDown(Key::Down))  player_.move_down();
}

void Game::update(float dt) {
    player_.update(dt);

    if (auto spawned = summoner_.update(dt))
        obstacles_.push_back(std::move(spawned));

    for (auto& o : obstacles_)
        o->move(dt);

    handle_player_obstacle_collisions();
    handle_obstacle_obstacle_collisions();

    if (player_.consume_hit_event())
        shake_.start(SHAKE_DURATION);

    remove_dead_obstacles();

    score_ += dt;

    if (player_.health_percentage() <= 0.f) {
        state_ = State::GameOver;
        std::cout << "Thomas walked for " << int(score_) << " seconds!" << endl;
    }
}

void Game::handle_player_obstacle_collisions() {
    for (auto& o : obstacles_) {
        auto info = player_.collide_with(*o);
        if (info.collided) {
            FirstEntityVisitor resolver(*o, info);
            player_.accept(resolver);
        }
    }
}

void Game::handle_obstacle_obstacle_collisions() {
    for (auto first = obstacles_.begin(); first != obstacles_.end(); ++first) {
        for (auto second = first + 1; second != obstacles_.end(); ++second) {

            auto info = (*first)->collide_with(**second);
            
            if (info.collided) {
                FirstEntityVisitor resolver(**second, info);
                (*first)->accept(resolver);
            }
        }
    }
}

void Game::remove_dead_obstacles() {
    obstacles_.erase(
        std::remove_if(
            obstacles_.begin(), 
            obstacles_.end(),
            [](const std::unique_ptr<Entity>& e) { 
                return e->is_dead(); 
            }
        ),
        obstacles_.end()
    );
}

Vector2D Game::compute_shake_offset(float dt) {
    if (!shake_.is_active())
        return {};

    shake_.tick(dt);
    return Vector2D(
        random_float(-SHAKE_MAG, SHAKE_MAG),
        random_float(-SHAKE_MAG, SHAKE_MAG)
    );
}

void Game::render(Vector2D shake_offset) {
    RenderContext ctx{
        player_.health_percentage(), 
        shake_offset
    };

    win_.clear(gfx::scale_brightness(BACKGROUND_COLOR, ctx.brightness));

    player_.render(win_, ctx);
    for (auto& o : obstacles_)
        o->render(win_, ctx);

    write(
        win_, 
        {20.f, 20.f}, 
        Direction::Right,
        "Score: " + to_string(int(score_)), 
        2,
        TEXT_COLOR
    );

    if (state_ == State::GameOver) {
        write_centered(
            win_, 
            {W_ / 2.f, H_ / 2.f}, 
            Direction::Right,
            "GAME OVER", 
            6, 
            TEXT_COLOR
        );

        write_centered(
            win_, 
            {W_ / 2.f, H_ / 2.f + 50.f}, 
            Direction::Right,
            "Press [SPACE] to restart! ", 
            3, 
            TEXT_COLOR
        );
    }
}

void Game::reset() {
    obstacles_.clear();
    player_.reset();
    score_ = 0.f;
    state_ = State::Playing;

    run();
}

int main() {
    Game game(800, 600, 30.f);
    game.run();
    return 0;
}

