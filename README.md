# Rektangles

Implementation of a Japanese puzzle game known as shikaku. A game for PinePhone (and for desktop).

## Build instructions

Dependencies: C++ compiler, CMake, Kirigami

To build and run the program:
```sh
mkdir build
cd build
cmake ..
make
bin/rektangles
```

With fewer commands:
```sh
cmake -B build/ . && cmake --build build/
build/bin/rektangles

```

## How to play

### Rules
1. Divide the grid into rectangles (and squares).
2. Each rectangle should contain exactly one number.
3. The number must be equal to the territory of the rectangle.

### Controls
- Navigation: arrow keys
- Enter/exit selection mode: Space/Enter
- New game: N
- Resize board: R
- Quit: Q

## Screenshots

![unsolved](screenshots/rektangles_unsolved.png "Unsolved puzzle")

![solved](screenshots/rektangles_solved.png "Solved puzzle")

![unsolved-mobile](screenshots/rektangles_mobile_unsolved.png "Unsolved puzzle on mobile")

![solved-mobile](screenshots/rektangles_mobile_solved.png "Solved puzzle on mobile")
