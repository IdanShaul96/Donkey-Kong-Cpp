#include "Barrel.h"
#include "Board.h"
#include <iostream>  
#include <algorithm>  
#include "Mario.h"
#include "Game.h"

using namespace std;


Barrel::Barrel(Point p) : Enemy(p) {
    setSymbol(Board::BARREL); // Set the symbol for the barrel
    prevFloor = Board::SPACE;
}

void Barrel::move(Board& board) {
    // Calculate new position based on current direction
    Point newP = getNewDir();

    // Get the current character at the barrel's current position
    char currentChar = getLocation().getNextChar(getLocation().getX(), getLocation().getY());

    // Handle ladders 
    if (currentChar == Board::LADDER) {
        gotoxy(getLocation().getX() - getLocation().getDir().x, getLocation().getY() - getLocation().getDir().y);
        std::cout << Board::LADDER;
    }

    if (currentChar == Mario::getPaulineSymbol()) {
        gotoxy(getLocation().getX() - getLocation().getDir().x, getLocation().getY() - getLocation().getDir().y);
        std::cout << Mario::getPaulineSymbol();
    }
   
    if (currentChar == Board::HAMMER) 
    {
		if (board.hammer != nullptr && !board.hammer->getIsHammerActive())
		{
			gotoxy(getLocation().getX() - getLocation().getDir().x, getLocation().getY() - getLocation().getDir().y);
			std::cout << Board::HAMMER;
		}
    }

    // Check the character below the barrel
    char charBelow = getLocation().getNextChar(newP.getX(), newP.getY() + 1);

    // Handle movement based on the floor type below
    if (charBelow == Board::LEFT_FLOOR) {
        setX(newP.getX() - 1);  // Move left
        prevFloor = Board::LEFT_FLOOR;   // Save the current floor type
        if (getFallCount() != BARREL_MAX_FALL_COUNT)
            setFallCount(0);
        return;
    }
    else if (charBelow == Board::RIGHT_FLOOR) {
        setX(newP.getX() + 1);  // Move right
        prevFloor = Board::RIGHT_FLOOR;  // Save the current floor type
        if (getFallCount() != BARREL_MAX_FALL_COUNT)
            setFallCount(0);
        return;
    }
    else if (charBelow == Board::SPACE) {
        //Point tmp = getLocation();
        setFallCount(getFallCount() + 1);  // Increment fall count
        if (getFallCount() == BARREL_MAX_FALL_COUNT) {
            getLocation().erase();  // Erase the barrel if it falls for 8 steps
            return;
        }
        setY(newP.getY() + 1);  // Move down (fall)
        return;
    }
    else if (charBelow == Board::STRAIGHT_FLOOR) {
        if (getFallCount() != BARREL_MAX_FALL_COUNT)
            setFallCount(0);

        // Determine movement direction based on the last floor type
        if (prevFloor == Board::LEFT_FLOOR) {
            setX(newP.getX() - 1);  // Move left
        }
        else if (prevFloor == Board::RIGHT_FLOOR) {
            setX(newP.getX() + 1);  // Move right
        }
    }
}

// BARRELS VECTOR FUNCTIONS:

void Barrel::drawBarrelsVector(Board& board) {
    // Draw all the barrels
    for (auto& barrel : board.barrels) {
        barrel->draw();  // Draw each barrel from the vector
    }
}

void Barrel::TimeTodrawANewBarrel(Board& board, Point& pos) 
{
    ULONGLONG currentTime = GetTickCount64();
    if (currentTime - lastBarrelDrawTime >= barrelInterval) {
        // Create and set a new barrel
        Barrel* newBarrel = new Barrel(pos);

        newBarrel->setBoard(board);  // Set the board for the new barrel
        board.barrels.push_back(newBarrel);  // Add the new barrel to the vector
        lastBarrelDrawTime = currentTime;  // Update the time for the last barrel draw
    }
}

void Barrel::eraseAndMoveBarrels(Board& board) {
    for (auto& barrel : board.barrels) {
        barrel->erase();  // Erase each barrel before updating position
        barrel->move(board);   // Move each barrel
    }
}

void Barrel::removeBombingBarrels(Board& board) {
    board.barrels.erase(std::remove_if(board.barrels.begin(), board.barrels.end(),
        [&](Barrel* b) { // Use a lambda with board context
            bool shouldRemove = false;

            if (b->getFallCount() >= BARREL_MAX_FALL_COUNT) {
                int newX = b->getLocation().getX();
                int newY = b->getLocation().getY();
                char floorBelow = b->getLocation().getNextChar(newX, newY + 1);

                if (floorBelow == Board::LEFT_FLOOR || floorBelow == Board::RIGHT_FLOOR || floorBelow == Board::STRAIGHT_FLOOR) {
                    b->getLocation().erase(); // Erase the barrel
                    b->animateExplosion(newX, newY); // Animate the explosion

                    if (board.mario) { // Check if Mario exists on the board
                        int marioX = board.mario->getX();
                        int marioY = board.mario->getY();

                        if (abs(newX - marioX) <= SPARK_RADIUS && abs(newY - marioY) <= SPARK_RADIUS)
                        {
                            board.mario->setHasCollided(true);
                            board.ghosts.clear();
                        }
                    }
                    shouldRemove = true;
                }
            }

            // Check if the barrel has reached the wall
            if (!shouldRemove && board.hasReachedWallBounds(b->getLocation().getX(), b->getLocation().getY())) {
                b->getLocation().erase(); // Erase the barrel
                shouldRemove = true;
            }

            if (shouldRemove) {
                delete b; // Free the memory of the barrel
                return true; // Mark the barrel for removal
            }

            return false;
        }),
        board.barrels.end());
}




void Barrel::animateExplosion(int centerX, int centerY) {
    vector<vector<string>> frames = {
        {
            " *** ",
            " * * "
        },
        {
            "*****",
            "*   *",
            "*   *"
        }
    };

    for (const auto& frame : frames) {
        int frameHeight = (int)frame.size(); // Number of rows in the current frame

        // Adjust starting Y-coordinate so the bottom row aligns with centerY
        int startY = centerY - (frameHeight - 1);

        // Print the frame centered around (centerX, centerY)
        for (int row = 0; row < frame.size(); ++row) {
            gotoxy(centerX - SPARK_RADIUS, startY + row); // Adjust X and aligned Y for each row
            cout << frame[row];
        }

        Sleep(30); // explosion duration
    }

    // Clear the explosion area after animation
    int clearHeight = (int)frames.back().size(); // Use the height of the largest frame
    for (int row = 0; row < clearHeight; ++row) {
        gotoxy(centerX - SPARK_RADIUS, centerY - (clearHeight - 1) + row);
        cout << "     "; // Clear 5 spaces to erase the *s
    }
}



