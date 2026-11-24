# Carré Surfer

## About

It's 2am in the Carré, the legendary student district of Liège, Belgium. Thomas has had a few drinks... now he has to stumble his way back home along a dirty sidewalk. Think *Subway Surfers*, but with a lot more Chouffe.

Your job is to help him get there. Thomas scrolls through an endless street, and everything in his way comes at him from the right:

- 🍢 **Leftover kebabs**: a delicious snack that actually heals him a little.
- 🗑️ **Garbage**: hurts a little.
- 🛴 **Abandoned Bolt scooters**: hurts a lot, he trips and falls.
- 🍺 **Other drunk students**: hurts a bit, and he's so embarrassed that his controls get reversed for a few seconds.
- 🚌 **Buses**: in the center of Liège, buses often come and go, hurting and blocking Thomas in his quest.

Thomas is quite drunk, so he can't help but wobble from side to side and pitifully stumble on random obstacles. His health is shown by the brightness of the screen: the more he gets hurt, the darker it gets, and at zero he passes out. Survive as long as you can: your score is the number of seconds you last before the game is over. Press `SPACE` to try again.

Under the hood, the game is written in C++20 with an object-oriented design: every on-screen object derives from a common `Entity` class, shapes resolve collisions through double dispatch, and entity-specific collision behaviour is handled with a visitor pattern. A report describing the design is available in [doc/report/report.tex](doc/report/report.tex).

## Dependencies

The game draws through a Wayland window wrapper, so it requires a **Wayland** session (X11 is not supported).

- A C++20 compiler (e.g. `g++`) and `make`
- `libwayland-dev`
- `libxkbcommon-dev`
- `wayland-protocols`
- `wayland-scanner` (usually shipped with `libwayland-dev`)

On Debian/Ubuntu:

```bash
sudo apt install build-essential libwayland-dev libxkbcommon-dev wayland-protocols
```

See [doc/wrapper/wrapper.md](doc/wrapper/wrapper.md) for troubleshooting.

## Build and run

```bash
cd src
make
./game
```

`make clean` removes the build artifacts, including the generated Wayland protocol files.

## Screenshots

![Gameplay](doc/img/gameplay.png)

![Obstacles](doc/img/obstacles.png)

![Game over](doc/img/game_over.png)

## Copyright

This was originally project 2 of the *Object-Oriented Programming Projects* course taught at Uliège by Laurent Mathy and his TAs, written by Valérian Wislez. There is not yet a LICENSE associated to this code, the author is actively trying to find a license suiting his needs. For the time being, you may consider that this code cannot be reused by anyone else unless explicitly stated by the author.
