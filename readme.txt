# Donkey Kong Arcade Game - C++ Engine

A classic arcade-style **Donkey Kong** game implemented from scratch in **C++** using object-oriented programming (OOP) principles, custom console graphics, and advanced game physics/animations.

[cite_start]*Developed by Idan Shaul & Shirin Younessian.* [cite: 8]

## Advanced Features & Gameplay Mechanics
* **Dynamic Enemy AI:** Features multiple types of enemies (`Ghosts` and `Barrels`) leveraging class inheritance and polymorphism from a base `Enemy` class.
* [cite_start]**Hammer Weapon System:** Active combat mechanics allowing Mario to collect and use a hammer with custom directional slicing animations (Left, Right, and STAY) [cite: 9] [cite_start]and a calculated 4-character effective hit radius[cite: 11]. [cite_start]Includes custom hammer graphics in the status Legend[cite: 9].
* [cite_start]**Sprite & Explosion Animations:** Custom text-based render animations for rolling barrels, ghost patrol paths, and dynamic barrel explosions[cite: 9].
* [cite_start]**Score & Progression System:** Integrated real-time score tracking (+500 points for defeating ghosts/barrels or successfully reaching Pauline) [cite: 9] with live state layouts.
* [cite_start]**Robust Error Correction & Validation:** Custom pre-loading map parser that automatically fixes corrupted layout screens (e.g., cleans up multiple `@` spawn points safely by keeping only the bottom-left one) [cite: 10] [cite_start]and normalizes rows to a standard 80x25 grid size[cite: 12].
* [cite_start]**Interactive UI:** Smooth in-game pause, win, and loss menus handled elegantly via Windows API message boxes[cite: 8].

## Architecture & Class Design
The engine enforces a clean separation of concerns using dedicated controllers:
* `Game.cpp / Game.h`: Coordinates the central game loop, window states, remaining lives, and physics frames.
* `Board.cpp / Board.h`: Handles map file parsing, visual screen refreshing, and coordinate bound safety checks.
* `Enemy.h / Ghost.cpp / Barrel.cpp`: Utilizes inheritance to manage distinct AI behaviors, gravity impacts, and rendering protocols.
* `Mario.cpp / Mario.h`: Manages player input buffer detection, dynamic free-fall gravity, and collision logic.

## Setup & Execution
1. Ensure you are on a Windows environment (the game utilizes `windows.h` and console manipulation methods).
2. Open `DonkeyKongGame2.sln` inside Visual Studio.
3. Build the solution under `Debug/Release` (x64 or x86).
4. Run the executable to launch the main game menu.
