#include <iostream>
#include "Point.h"
#include "Mario.h"
#include "Barrel.h"
#include "Board.h" 


// Get direction from key press:
Point::Direction Point::getDirectionFromArr(char key) {
    Direction res = directions[numKeys - 1];  // Default to 'stop' (0, 0)
    for (size_t i = 0; i < numKeys; i++) {
        if (std::tolower(key) == keys[i]) {  // Compare the key press with the keys array
            res = directions[i];  // Set the corresponding direction
            break;
        }
    }
    return res;
}

// Handle key press for movement
void Point::keyPressed(char key) {
    for (size_t i = 0; i < numKeys; i++) 
    {
        if (std::tolower(key) == keys[i]) 
        {
            dir = directions[i];  // Update the current direction
			if (key == HAMMER) //dont let the hammer change the direction of @
            {
                dir = prevDir;
            }
            else if (key != UP) {  // Only update previous direction if it's not 'w' (jump key)
                prevDir = dir;
            }

			
            return;
        }
    }
}

void Point::erase() {
    gotoxy(x, y);
    std::cout << Board::SPACE;
}

char Point::getNextChar(int x, int y) {
    if (pBoard == nullptr || pBoard->hasReachedWallBounds(x, y))
    {
        return '\0';
    }
    return pBoard->getChar(x, y);
}

