#include "Ghost.h"
#include "Board.h"
#include "Mario.h"
#include <windows.h>


Ghost::Ghost(Point ghost) : Enemy(ghost) {
    setSymbol(Board::GHOST); // Set the symbol
}


void Ghost::eraseAndMoveGhosts(Board& board) {
    for (auto& ghost : board.ghosts) {
        ghost->erase();  // Erase each barrel before updating position
        ghost->move(board);   // Move each barrel
    }
}

void Ghost::move(Board board) {
    // Get the current position and direction of the ghost
    Point newP = getNewDir();  // Get the new direction based on the current direction
    newP.setBoard(board);
    char floorBelow = newP.getNextChar(newP.getX(), newP.getY() + 1);

    // Get the character at the current position of the ghost
    char nextCharFromRight = newP.getNextChar(getLocation().getX() + 1, getLocation().getY());
    char nextCharFromLeft = newP.getNextChar(getLocation().getX() - 1, getLocation().getY());

    char prevCharFromRight = newP.getNextChar(getLocation().getX() - 1, getLocation().getY());
    char prevCharFromLeft = newP.getNextChar(getLocation().getX() + 1, getLocation().getY());

    // Check the character below the ghost (to simulate if it is standing on a floor or if it will fall)
    char charBelowNext = Board::SPACE;
    char charBelowNextRight = newP.getNextChar(newP.getX() + 1, newP.getY() + 1);
    char charBelowNextLeft = newP.getNextChar(newP.getX() - 1, newP.getY() + 1);

    if (floorBelow == Board::SPACE)
    {
		this->setY(newP.getY() + 1); // Move the ghost down
    }

    // Handle movement based on the floor type below
    if (floorBelow == Board::LEFT_FLOOR || floorBelow == Board::RIGHT_FLOOR || floorBelow == Board::STRAIGHT_FLOOR) {

        if (rand() % 100 < 5) {
            changeDirection(); // Reverse the direction with 5% probability
        }

        if (getDirection() > 0) // moving right:
        {
            if (prevCharFromRight == Mario::getPaulineSymbol()) {
                gotoxy(getX() , getY());
                std::cout << Mario::getPaulineSymbol();
            }

            // Handle ladder and space checks
            if (prevCharFromRight == Board::LADDER) {
                gotoxy(getX() - 1, getY());
                std::cout << Board::LADDER;
            }
            if (prevCharFromRight == Board::HAMMER && board.hammer != nullptr && !board.hammer->getIsHammerActive()) {
                gotoxy(getX() - 1, getY());
                std::cout << Board::HAMMER;
            }

            if (charBelowNextRight == Board::SPACE) {
                changeDirection();
            }
            else if (nextCharFromRight == Board::GHOST) {
                changeDirection();
            }

            if (newP.getX() + 1 == Board::getMaxX()) {
                changeDirection();
            }
        }
        else  // moving left:
        {
            if (prevCharFromRight == Mario::getPaulineSymbol()) {
                gotoxy(getX() , getY());
                std::cout << Mario::getPaulineSymbol();
            }
            // Handle ladder and space checks
            if (prevCharFromLeft == Board::LADDER) {
                gotoxy(getX() + 1, getY());
                std::cout << Board::LADDER;
            }
            if (prevCharFromLeft == Board::HAMMER && board.hammer != nullptr && !board.hammer->getIsHammerActive()) {
                gotoxy(getX() + 1, getY());
                std::cout << Board::HAMMER;
            }

            if (charBelowNextLeft == Board::SPACE) {
                changeDirection();
            }
            else if (nextCharFromLeft == Board::GHOST) {
                changeDirection();
            }

            if (newP.getX() - 1 == Board::getWallMinX()) {
                changeDirection();
            }
        }

        // Move the ghost
        if (getDirection() == 1) {
            setX(newP.getX() + 1); // move right
        }
        else if (getDirection() == -1) {
            setX(newP.getX() - 1); // move left
        }
    }
}


void Ghost::drawGhostVector(Board& board) {
    // Draw all the barrels
    for (auto& ghost : board.ghosts) {
        if (ghost->getIsAlive())
        {
            ghost->draw();  // Draw each barrel from the vector
        }
    }
}