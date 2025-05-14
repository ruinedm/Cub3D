# cub3D

## 📝 Overview

cub3D is a 3D maze game inspired by the famous Wolfenstein 3D game, rendered using ray-casting principles. This project is part of the 42 school curriculum, focusing on graphics programming and efficient rendering techniques.

## 🔍 Features

- First-person perspective maze navigation
- Textured walls with different textures for each direction (North, South, East, West)
- Ray-casting rendering engine
- Smooth window management
- Customizable floor and ceiling colors
- Map parsing from configuration file

### 🎮 Controls

- **W, A, S, D**: Move through the maze
- **←, →**: Rotate camera left and right
- **ESC**: Exit game
- **Red cross**: Close window and exit game

## 🛠️ Installation

### Prerequisites

- GCC compiler
- Make
- MiniLibX library
- X11 include files (on Linux)

### Steps to Compile and Run

```bash
# Clone the repository
git clone https://github.com/yourusername/cub3D.git
cd cub3D

# Compile the project
make

# Run with a map file
./cub3D maps/map.cub
```

## 📁 Map Format

Maps must be provided in `.cub` files with the following specifications:

### Textures and Colors

```
NO ./path_to_the_north_texture
SO ./path_to_the_south_texture
WE ./path_to_the_west_texture
EA ./path_to_the_east_texture
F 220,100,0    # Floor RGB color
C 225,30,0     # Ceiling RGB color
```

### Map Layout

- **0**: Empty space
- **1**: Wall
- **N/S/E/W**: Player starting position and orientation

Example map:
```
111111
100101
101001
1100N1
111111
```

Rules:
- Map must be enclosed by walls
- Only characters 0, 1, N, S, E, and W are allowed
- Only one player position is allowed

## 🛡️ Error Handling

The program handles various errors including:
- Invalid map format
- Missing textures or colors
- Map not surrounded by walls
- Duplicate player positions
- Invalid characters in map

## 🎯 Bonus Features

- Wall collision detection
- Minimap display
- Functional doors (open/close)
- Animated sprites
- Mouse rotation control
