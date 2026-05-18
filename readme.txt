# Donkey Kong Arcade Game - C++ Engine

A classic arcade-style **Donkey Kong** game implemented from scratch in **C++** using object-oriented programming (OOP) principles, custom console graphics, and advanced game physics/animations.

*Developed by Idan Shaul & Shirin Younessian.*

## Advanced Features & Gameplay Mechanics
* **Dynamic Enemy AI:** Features multiple types of enemies (`Ghosts` and `Barrels`) leveraging class inheritance and polymorphism from a base `Enemy` class.
* **Hammer Weapon System:** Active combat mechanics allowing Mario to collect and use a hammer with custom directional slicing animations (Left, Right, and STAY) and a calculated 4-character effective hit radius. Includes custom hammer graphics in the status Legend.
* **Sprite & Explosion Animations:** Custom text-based render animations for rolling barrels, ghost patrol paths, and dynamic barrel explosions.
* **Score & Progression System:** Integrated real-time score tracking (+500 points for defeating ghosts/barrels or successfully reaching Pauline) with live state layouts.
* **Robust Error Correction & Validation:** Custom pre-loading map parser that automatically fixes corrupted layout screens (e.g., cleans up multiple `@` spawn points safely by keeping only the bottom-left one) and normalizes rows to a standard 80x25 grid size.
* **Interactive UI:** Smooth in-game pause, win, and loss menus handled elegantly via Windows API message boxes.

## Architecture & Class Design
The engine enforces a clean separation of concerns using dedicated controllers:
* `Game.cpp / Game.h`: Coordinates the central game loop, window states, remaining lives, and physics frames.
* `Board.cpp / Board.h`: Handles map file parsing, visual screen refreshing, and coordinate bound safety checks.
* `Enemy.h / Ghost.cpp / Barrel.cpp`: Utilizes inheritance to manage distinct AI behaviors, gravity impacts, and rendering protocols.
* `Mario.cpp / Mario.h`: Manages player input buffer detection, dynamic free-fall gravity, and collision logic.

## Setup & Execution
1. Ensure you are on a Windows environment (the game utilizes `windows.h` and console manipulation methods).
2. Open `DonkeyKongGame.sln` inside Visual Studio.
3. Build the solution under `Debug/Release` (x64 or x86).
4. Run the executable to launch the main game menu.
