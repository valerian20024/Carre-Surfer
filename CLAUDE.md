# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

Carré Surfer: a 2D survival/dodging game in C++20, written for Project 2 of the *Object-Oriented Programming Projects* course at ULiège (assignment: `doc/stmt/project2_cpp.pdf`). The player square dodges obstacles scrolling in from the right; score = seconds survived. A LaTeX report describing the design lives in `doc/report/report.tex`.

## Build & run

Headers are in `include/`, sources in `src/`, the course-provided Wayland wrapper in `third_party/wayland_wrapper/`, and the Makefile is at the repo root. Run from the root:

```bash
make          # builds ./game (objects go to build/)
./game        # or: make run
make clean    # removes ./game and build/
```

- Requires a **Wayland** session (not X11) and the packages `libwayland-dev libxkbcommon-dev wayland-protocols` (+ `wayland-scanner`). See `doc/wrapper/wrapper.md` for troubleshooting.
- The Makefile generates `build/xdg-shell-client-protocol.h` / `build/xdg-shell-protocol.c` from `/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml` via `wayland-scanner`.
- Every `src/*.cpp` is compiled automatically (wildcard); header dependencies are tracked via `-MMD -MP`.
- There are no tests and no linter. Compiler warnings come from `-Wall -Wextra`.

## Architecture

- **`Window.cpp/hpp` (`gfx::Window`) is a course-provided wrapper** — treat it as third-party; the game only uses its drawing primitives (`fillRect`, `fillCircle`, …), polled key input (`isKeyDown`, `wasKeyPressed`) and `waitForNextFrame`.
- **`Game`** (`game.cpp`, also contains `main`) owns the window, the `Player`, a `Summoner` and `std::vector<std::unique_ptr<Entity>> obstacles_`. Fixed timestep (`dt = 1/fps`, 30 fps). Per frame: input → `player_.update` → spawn → `move` all obstacles → player–obstacle and O(n²) obstacle–obstacle collisions → remove `is_dead()` entities. Two states: `Playing` / `GameOver`.
- **`Entity`** is the base for everything on screen: position/speed/acceleration, an owned `Shape`, color, `visible_`, `dead_` flag. Subclasses customize rendering by overriding `text_layers()` (labels drawn centered on the shape with the custom pixel font in `font.cpp`/`text.cpp`). Rendering takes a `RenderContext` (global brightness tied to player health + screen-shake offset).
- **Shapes** (`geometry.hpp`): `Circle` (position = center) and `Rectangle` (position = top-left) use shape-level double dispatch (`collide_with` → `collide_with_circle`/`collide_with_rectangle`) returning `CollisionInfo { collided, minimum_translation }`.
- **Entity collision resolution uses a two-stage visitor** (`collision.hpp/cpp`): `FirstEntityVisitor` discovers the first entity's concrete type, then `SecondEntityVisitor<T>` discovers the second's and calls a free `resolve(A&, B&, CollisionInfo)`; C++ overload resolution picks the most specific overload, falling back to `resolve(Obstacle&, Obstacle&)` / `resolve(Entity&, Entity&)` (push apart). To give a new entity type special collision behavior: add a `visit` overload to `CollisionVisitor` and both visitors, override `accept()` in the class, and add `resolve` overloads (both argument orders — the reversed one usually forwards with `flip(info)`). Types without their own `visit` (e.g. `Garbage`, `Scooter`) are handled as `Obstacle`.
- **`Cooldown`** is the universal timer: `start(seconds)`, `tick(dt)` returns true exactly once when it reaches zero. Player invincibility/blinking/reversed-controls/stagger and spawn intervals are all built on it.
- **`Summoner`** spawns one obstacle off-screen right at random intervals, choosing the type via a weighted `std::discrete_distribution` over a factory list in `make_obstacle` — add new obstacle types there.
- Tunables are `static constexpr` constants in each class header (e.g. damage, sizes, speeds in `obstacles.hpp`, player timings in `player.hpp`). Random helpers are in `helpers.hpp`.
