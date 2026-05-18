#include "Mario.h"
#include "Board.h"
#include <iostream>
#include "Barrel.h"
#include "Game.h"
#include "Menu.h"
#include <conio.h>
#include "Hammer.h"


void Mario::keyPressed(char key, Hammer& hammer) //handle pressed key to assign to mario's movement
{
    char currentChar = marioPoint.getNextChar(marioPoint.getX(), marioPoint.getY());

    // block mario from keep going up if up is already pressed
    if (std::tolower(key) == Point::UP)
    {
        if (key == Point::UP && isJumping)
        {
            return;
        }
        if (isFreeFalling || stillInTheAir)
        {
            return;
        }

        marioPoint.setPrevDir(marioPoint.getDir());
    }



    int marioX;
    int marioY;
	//handle hammer action for ghosts:
    for (auto it = board->ghosts.begin(); it != board->ghosts.end(); ) 
    {
        auto ghost = *it;
        marioX = marioPoint.getX();
        marioY = marioPoint.getY();
        int ghostX = ghost->getLocation().getX();
        int ghostY = ghost->getLocation().getY();

      

        if (std::tolower(key) == Point::HAMMER && hammer.getIsHammerActive() &&
            ((abs(marioX - ghostX) <= HAMMER_HIT_RADIUS) && marioY == ghostY)) {


            //animation in STAY not showing on screen

            // Activate hammer
            hammer.setHasHammerBeenActivated(true);

            // Erase the ghost from the screen and free its memory
            ghost->getLocation().erase();
            ghost->setIsAlive(false);

            // Remove ghost from vector
            it = board->ghosts.erase(it);
        }
        else 
        {
            ++it;
            marioPoint.setDir(marioPoint.getPrevDir()); //dont activate hammer if next to no one to kiil
        }
        
    }

	//handle hammer action for barrels:
    for (auto it = board->barrels.begin(); it != board->barrels.end(); ) 
    {
        auto barrel = *it;
        marioX = marioPoint.getX();
        marioY = marioPoint.getY();
        int barrelX = barrel->getLocation().getX();
        int barrelY = barrel->getLocation().getY();

        if (std::tolower(key) == Point::HAMMER && hammer.getIsHammerActive() &&
            ((abs(marioX - barrelX) <= HAMMER_HIT_RADIUS) && marioY == barrelY)) {

            //animation in STAY not showing on screen

            // Activate hammer
            hammer.setHasHammerBeenActivated(true);

            // Erase the barrel from the screen and free its memory
            barrel->getLocation().erase();

            // Remove barrel from vector
            it = board->barrels.erase(it);
        }
        else {
            marioPoint.setDir(marioPoint.getPrevDir()); // don't activate hammer if next to no one to kill
            ++it;
        }
    }



    //block movement to the sides if on a ladder
	if ((currentChar == Board::LADDER) && (key == Point::RIGHT || key == Point::LEFT) && !isOnFloor())
    {
        return;
    }
    else if (std::tolower(key) == Point::UP)
    {
        if (key == Point::UP && isJumping)
        {
            return;
        }
        if (isFreeFalling || stillInTheAir)
        {
            return;
        }

        marioPoint.setPrevDir(marioPoint.getDir());

        if (currentChar == Board::LADDER) {
            marioPoint.setDir(marioPoint.getClimb());
        }
        else
        {
            marioPoint.setDir(marioPoint.getDirectionFromArr(Point::UP));
            isJumping = true;
            jumpPhase = JUMP_PHASE_1;
        }
    }
    // else: forward the key pressed to the move function:
    else {
        marioPoint.keyPressed(key);
    }
}


void Mario::move() {

    // settting mario postion in a local variables:

    int newX = marioPoint.getX() + marioPoint.getDir().x;
    int newY = marioPoint.getY() + marioPoint.getDir().y;

    char currentChar = marioPoint.getNextChar(marioPoint.getX(), marioPoint.getY());
    char nextChar = marioPoint.getNextChar(newX, newY);
  
    char charBelow = marioPoint.getNextChar(marioPoint.getX(), marioPoint.getY() + MOVE_DOWN);
    char nextYchar = marioPoint.getNextChar(marioPoint.getX(), marioPoint.getY() + MOVE_DOWN);

    int prevX = marioPoint.getX();
    int prevY = marioPoint.getY();

    // setting a jump in case of 'UP' after the run
    handleRunAndJump(newX, newY);


    // Mario freeFall function if he is in a state of "isFreeFalling":

    if (currentChar == Board::SPACE && charBelow == Board::SPACE) {

        if (!wasJumping)
        {
            isFreeFalling = true;
        }
    }
    if (isFreeFalling)
    {
        freeFall();
    }
    // getting the prevPos of mario based on the key pressed:

    MarioPrevPosBasedOnKey(prevX, prevY, newX, newY, nextYchar);

    char prevChar = marioPoint.getNextChar(prevX, prevY);

    // enebling mario to go down a ladder:

    if (marioPoint.getDir() == marioPoint.getDirectionFromArr(Point::DOWN)) {
        if (nextYchar == Board::LADDER && prevChar == Board::SPACE) {
            newY += MOVE_DOWN;
        }
    }

    // checking mario wallBounds:

    if (marioPoint.getBoard().Board::hasReachedWallBounds(newX, newY))
    {
        marioPoint.setDir(marioPoint.getDirectionFromArr(Point::STAY)); // Stop moving if out of wall bounds
        return;
    }
    // Checking if mario has reached PUALINE:
    // If he has changing mario state to: "hasWon":
    if (nextChar == PAULINE) {
        setHasWon(true);
        return;
    }

    // Climing and going down on  a Ladder:
    LadderClimbingAndDescending(newX, newY, currentChar, nextChar, prevChar, charBelow, nextYchar);

    // Checking mario state to jump:

    if (isJumping) {
        jump(newX, newY);
    }
    // checking if mario is on the floor, to know that mario state "stillInTheAir" has ended.

    if (isOnFloor())
    {
        stillInTheAir = false;
    }
}


// mario jump function:

void Mario::jump(int newX, int newY) {
    // Handle the jump phases

    switch (jumpPhase) {
    case JUMP_PHASE_1: // mario is moving left or right and then jumping.
        if (marioPoint.getPrevDir().x == MOVE_LEFT) {
            marioPoint.setDir(marioPoint.getUpLeft());
            wasJumping = true;

        }
        else if (marioPoint.getPrevDir().x == MOVE_RIGHT) {
            marioPoint.setDir(marioPoint.getUpRight());
            wasJumping = true;

        }

        // mario is jumping vertically:
        else if (marioPoint.getPrevDir().x == STOP && marioPoint.getNextChar(newX, newY) != Board::LADDER) {
            static int JumpCountVertical = 0;
            marioPoint.setY(newY + MOVE_DOWN);
            JumpCountVertical++;
            wasJumping = true;
            // the top of the vertical hieght, mario will now go down. 
            if (JumpCountVertical == TOP_OF_VERTICAL)
            {
                marioPoint.setDir(marioPoint.getDirectionFromArr(Point::DOWN));
                isJumping = false;
                stillInTheAir = true;
                JumpCountVertical = 0;
            }

            break;
        } // if mario is currnetly climbing a ledder he is not in a state of 'isJumping' 
        else if (marioPoint.getPrevDir().x == STOP) {
            marioPoint.setDir(marioPoint.getClimb());
            isJumping = false;
            break;
        }
        jumpPhase = JUMP_PHASE_2;
        break;
    case JUMP_PHASE_2: // making mario moving in the air to the right location(right or left)
        if (marioPoint.getPrevDir().x == MOVE_RIGHT)
        {
            marioPoint.setDir(marioPoint.getDownRight());
        }
        else
        {
            marioPoint.setDir(marioPoint.getDownLeft());
        }

        jumpPhase = JUMP_PHASE_3;
        break;
    case JUMP_PHASE_3:// making mario moving in the air to the right location(right or left)

        if (marioPoint.getPrevDir().x == MOVE_RIGHT)
        {
            marioPoint.setDir(marioPoint.getDownRight());
        }
        else
        {
            marioPoint.setDir(marioPoint.getDownLeft());
        }
        jumpPhase = JUMP_PHASE_5;
        break;
    case JUMP_PHASE_4:
        // make mario stop at the floor above or below the ladder:
        if (marioPoint.getDir().y == MOVE_UP) {
            isJumping = false;
            break;
        }
        if (marioPoint.getDir().y == MOVE_DOWN && marioPoint.getPrevDir().y != MOVE_DOWN) {

            marioPoint.setDir(marioPoint.getDirectionFromArr(Point::DOWN));
            isJumping = false;
            break;
        }
        break; // Add this break statement to prevent fallthrough
    case JUMP_PHASE_5: // the jump has ended:
        marioPoint.setDir(marioPoint.getPrevDir()); // keep going in the dir before the jump.
        isJumping = false;
        wasJumping = false;
        break;
    }

}

// mario freeFall function:

void Mario::freeFall() {
    if (!isJumping)
    {

        if (!isOnFloor())
        {
            // Now handle Mario falling (move him down in the Y direction)
            marioPoint.setY(marioPoint.getY() + MOVE_DOWN);  // Move Mario down (fall)

            // Increment fall count
            marioPoint.setFallCount(marioPoint.getFallCount() + 1);

        }
        else
        {
            // if mario has fallen over 5 chars, and reached the floor the board reset and he loses a life.

            isFreeFalling = false;
            if (marioPoint.getFallCount() >= MARIO_MAX_FALL_COUNT) 
            {
				board->ghosts.clear();
                marioPoint.setDir(marioPoint.getDirectionFromArr(Point::STAY));
                isReset = true;
                hasCollided = true;
                return;
            }
            else
            {
                marioPoint.setFallCount(0);
                marioPoint.setY(marioPoint.getY());
                marioPoint.setDir(marioPoint.getPrevDir());
                return;
            }
        }
    }
}
// mario handleRunAndJump function:

bool Mario::handleRunAndJump(int& newX, int& newY) {
    // Check if Mario is running left or right (horizontal movement)
    if (marioPoint.getPrevDir().x == MOVE_RIGHT || marioPoint.getPrevDir().x == MOVE_LEFT) {

        if (marioPoint.getDir() == marioPoint.getDirectionFromArr(Point::UP)) { // Jumping up
            if (marioPoint.getPrevDir().x == MOVE_RIGHT) {
                newX = marioPoint.getX() + MOVE_RIGHT; // Move right
            }
            else {
                newX = marioPoint.getX() + MOVE_LEFT; // Move left
            }
            newY = marioPoint.getY() + MOVE_UP; // Jump upwards
            isJumping = true;
            return true;
        }
    }
    return false; // Not a run-and-jump scenario
}

//hadle movement:
void Mario::MarioPrevPosBasedOnKey(int& prevX, int& prevY, int newX, int newY, char& nextYchar) {

    // Update nextYchar based on Mario's position and direction

    if (marioPoint.getDir() == marioPoint.getDirectionFromArr(Point::UP)) {
        prevX = marioPoint.getX();
        prevY = marioPoint.getY() + MOVE_DOWN;
        nextYchar = marioPoint.getNextChar(newX, newY + MOVE_UP);
    }
    else if (marioPoint.getDir() == marioPoint.getDirectionFromArr(Point::DOWN)) {
        prevX = marioPoint.getX();
        prevY = marioPoint.getY() + MOVE_UP;
        nextYchar = marioPoint.getNextChar(newX, newY + MOVE_DOWN);
    }
    else if (marioPoint.getDir() == marioPoint.getDirectionFromArr(Point::LEFT)) {
        prevX = marioPoint.getX() + MOVE_RIGHT;
        prevY = marioPoint.getY();
        nextYchar = marioPoint.getNextChar(newX + MOVE_LEFT, newY);
    }
    else if (marioPoint.getDir() == marioPoint.getDirectionFromArr(Point::RIGHT)) {
        prevX = marioPoint.getX() + MOVE_LEFT;
        prevY = marioPoint.getY();
        nextYchar = marioPoint.getNextChar(newX + MOVE_RIGHT, newY);
    }
}
// mario ladder function:

void Mario::LadderClimbingAndDescending(int newX, int newY, char currentChar, char nextChar, char prevChar, char charBelow, char nextYchar) {
    // If Mario is near the floor and the next character is a floor tile
    if (nextChar == Board::LEFT_FLOOR || nextChar == Board::STRAIGHT_FLOOR || nextChar == Board::RIGHT_FLOOR) {
        if (prevChar == Board::LADDER) {
           // isJumping = true;
            jumpPhase = JUMP_PHASE_4;
            jump(newX, newY);  // Call jump when Mario leaves the ladder
        }
        else {
            if (nextYchar != Board::LADDER) {  // Ensure the next position is not a ladder
                marioPoint.setDir(marioPoint.getDirectionFromArr(Point::STAY));  // Stop moving
                return;
            }
        }
    }

    // If Mario is already on the floor
    if ((currentChar == Board::STRAIGHT_FLOOR) ||
        (currentChar == Board::LEFT_FLOOR) ||
        (currentChar == Board::RIGHT_FLOOR))
  
    {
        marioPoint.setDir(marioPoint.getDirectionFromArr(Point::STAY));  // Stop movement when on the floor
        return;
    }
    else {

        // If Mario is on the ladder and the next position is a floor tile
        if (currentChar == Board::LADDER && (nextChar == Board::LEFT_FLOOR || nextChar == Board::STRAIGHT_FLOOR || nextChar == Board::RIGHT_FLOOR)) {
            marioPoint.setX(newX);  // Move horizontally to the new position
            marioPoint.setY(newY + MOVE_UP);  // Move one step up to land on the floor
        }
		
        else {
            // If Mario is at a standstill and is above a floor tile (can be adjusted based on your logic)
            if (marioPoint.getPrevDir() == marioPoint.getDirectionFromArr(Point::STAY) &&
                (charBelow == Board::STRAIGHT_FLOOR || charBelow == Board::RIGHT_FLOOR || charBelow == Board::LEFT_FLOOR) && prevChar == Board::SPACE) {
                marioPoint.setDir(marioPoint.getDirectionFromArr(Point::STAY));
                return;
            }
            if (!isFreeFalling && !isReset)
            {
                marioPoint.setX(newX);  // Update Mario's position
                marioPoint.setY(newY);
            }
            else
            {
                isReset = false;
            }
        }
    }

    // Restoring the ladder when Mario leaves it
    if (currentChar == Board::LADDER) {
        if (nextChar == Board::LEFT_FLOOR || nextChar == Board::STRAIGHT_FLOOR || nextChar == Board::RIGHT_FLOOR) {
            // Draw the ladder back at the previous position
            gotoxy(marioPoint.getX() - marioPoint.getDir().x, marioPoint.getY() + MOVE_DOWN - marioPoint.getDir().y);
            std::cout << Board::LADDER;
            marioPoint.setDir(marioPoint.getDirectionFromArr(Point::STAY));  // Stop movement
        }
        else {
            // Draw the ladder back at the previous position
            gotoxy(marioPoint.getX() - marioPoint.getDir().x, marioPoint.getY() - marioPoint.getDir().y);
            std::cout << Board::LADDER;

            // If Mario is climbing UP the ladder
            if (marioPoint.getDir() == marioPoint.getDirectionFromArr(Point::UP)) {
                if (nextChar == Board::LEFT_FLOOR || nextChar == Board::STRAIGHT_FLOOR || nextChar == Board::RIGHT_FLOOR) {
                    // If Mario reaches the floor above the ladder, stop moving and land on the floor
                    marioPoint.setDir({ marioPoint.getDir().x, marioPoint.getDir().y + (2 * MOVE_UP) });
                    marioPoint.setY(newY);  // Make sure Mario lands on the floor
                }
            }
        }
    }

}
// mario resert function in case of barrel collistion:


void Mario::resetIfCollisionWithEnemy()
{
    if (board == nullptr) {
        return; // Ensure board is set
    }
    int currentX = marioPoint.getX();
    int currentY = marioPoint.getY();

    // Define the adjacent positions to check
    std::vector<std::pair<int, int>> adjacentPositions = {
        {currentX, currentY},
        {currentX + MOVE_RIGHT, currentY},
        {currentX + MOVE_LEFT, currentY},
        {currentX, currentY + MOVE_DOWN},
        {currentX, currentY + MOVE_UP}
    };


    // Check for collision with each barrel
    for (const auto& barrel : board->barrels) {
        for (const auto& pos : adjacentPositions) {
            if (barrel->getLocation().getX() == pos.first && barrel->getLocation().getY() == pos.second) {
                erase(); // Erase Mario from the screen

                // Erase all barrels from the screen
                for (auto& barrel : board->barrels) {
                    barrel->getLocation().erase();
                }

                // Free the memory allocated for barrels
                for (auto& barrel : board->barrels) {
                    delete barrel;
                }

                board->barrels.clear(); // Clear the barrels vector
				board->ghosts.clear(); // Clear the ghosts vector

                board->reset();

                // mainGame.setRemainingLives(); // Uncomment if you have this functionality
                hasCollided = true;
                marioPoint.setDir(marioPoint.getDirectionFromArr(Point::STAY));

                // Clear Mario's previous position on the screen
                gotoxy(currentX, currentY);
                std::cout << Board::SPACE;

                // Clear the adjacent positions where Mario might be
                for (const auto& pos : adjacentPositions) {
                    gotoxy(pos.first, pos.second);
                    std::cout << Board::SPACE;
                }
                clear_screen();

                board->print();

                return; // Exit after reset to avoid further actions
            }
        }
    }


    for (const auto& ghost : board->ghosts) {
        for (const auto& pos : adjacentPositions) {
            if (ghost->getLocation().getX() == pos.first && ghost->getLocation().getY() == pos.second && ghost->getIsAlive()) {
                erase(); // Erase Mario from the screen

                // Erase all ghosts from the screen and free their memory
                for (auto& ghost : board->ghosts) {
                    ghost->getLocation().erase();
                    delete ghost;
                }
                board->ghosts.clear(); // Clear the vector after deletion

                board->reset();

                hasCollided = true;
                // marioPoint.setDir(Point::STAY);

                 // Clear Mario's and adjacent positions
                gotoxy(currentX, currentY);
                std::cout << Board::SPACE;
                for (const auto& pos : adjacentPositions) {
                    gotoxy(pos.first, pos.second);
                    std::cout << Board::SPACE;
                }

                clear_screen();
                board->print();
                return; // Exit after reset
            }
        }
    }
}

void Mario::resetIfCollisionWithBarrel() 
{
    if (board == nullptr) {
        return; // Ensure board is set
    }

    int currentX = marioPoint.getX();
    int currentY = marioPoint.getY();

    // Define the adjacent positions to check
    std::vector<std::pair<int, int>> adjacentPositions = {
        {currentX, currentY},
        {currentX + MOVE_RIGHT, currentY},
        {currentX + MOVE_LEFT, currentY},
        {currentX, currentY + MOVE_DOWN},
        {currentX, currentY + MOVE_UP}
    };

    // Check for collision with each barrel
    for (const auto& barrel : board->barrels) {
        for (const auto& pos : adjacentPositions) {
            if (barrel->getLocation().getX() == pos.first && barrel->getLocation().getY() == pos.second) {
                erase(); // Erase Mario from the screen

                // Erase all barrels from the screen
                for (auto& barrel : board->barrels) {
                    barrel->getLocation().erase();
                }

                // Free the memory allocated for barrels
                for (auto& barrel : board->barrels) {
                    delete barrel;
                }

                board->barrels.clear(); // Clear the barrels vector

                board->reset();

                // mainGame.setRemainingLives(); // Uncomment if you have this functionality
                hasCollided = true;
                marioPoint.setDir(marioPoint.getDirectionFromArr(Point::STAY));

                // Clear Mario's previous position on the screen
                gotoxy(currentX, currentY);
                std::cout << Board::SPACE;

                // Clear the adjacent positions where Mario might be
                for (const auto& pos : adjacentPositions) {
                    gotoxy(pos.first, pos.second);
                    std::cout << Board::SPACE;
                }
                clear_screen();

                board->print();

                return; // Exit after reset to avoid further actions
            }
        }
    }

}

// function to reset mario position to the start with a 'STAY' state:
void Mario::resetPositionOriginal(Point pos)
{
    marioPoint.setX(pos.getX());
    marioPoint.setY(pos.getY());
	marioPoint.setDir(marioPoint.getDirectionFromArr(Point::STAY));
}

bool Mario::isOnFloor()
{
    if (marioPoint.getNextChar(marioPoint.getX(), marioPoint.getY() + 1) == Board::LEFT_FLOOR ||
        marioPoint.getNextChar(marioPoint.getX(), marioPoint.getY() + 1) == Board::RIGHT_FLOOR ||
        marioPoint.getNextChar(marioPoint.getX(), marioPoint.getY() + 1) == Board::STRAIGHT_FLOOR)
        return true;
    return false;
}

void Mario::resetIfCollisionWithGhost() {
    if (board == nullptr) {
        return; // Ensure board is set
    }

    int currentX = marioPoint.getX();
    int currentY = marioPoint.getY();

    std::vector<std::pair<int, int>> adjacentPositions = {
        {currentX, currentY},
        {currentX + MOVE_RIGHT, currentY},
        {currentX + MOVE_LEFT, currentY},
        {currentX, currentY + MOVE_DOWN},
        {currentX, currentY + MOVE_UP}
    };

    for (const auto& ghost : board->ghosts) {
        for (const auto& pos : adjacentPositions) {
            if (ghost->getLocation().getX() == pos.first && ghost->getLocation().getY() == pos.second && ghost->getIsAlive()) {
                erase(); // Erase Mario from the screen

                // Erase all ghosts from the screen and free their memory
                for (auto& ghost : board->ghosts) {
                    ghost->getLocation().erase();
                    delete ghost;
                }
                board->ghosts.clear(); // Clear the vector after deletion
                board->reset();

                // Reinitialize ghosts
                for (int i = 0; i < Board::getMaxY(); i++) {
                    for (int j = 0; j < Board::getMaxX(); j++) {
                        if (board->getOriginalBoard()[i][j] == Board::GHOST) {
                           // std::cout << "Reinitializing ghost at (" << j << ", " << i << ")" << std::endl;
                            Point ghostPoint(j, i);
                            Ghost* newGhost = new Ghost(ghostPoint);
                            board->ghosts.push_back(newGhost);
                            newGhost->setIsAlive(true);
                        }
                    }
                }


                hasCollided = true;

                // Clear Mario's and adjacent positions
                gotoxy(currentX, currentY);
                std::cout << Board::SPACE;
                for (const auto& pos : adjacentPositions) {
                    gotoxy(pos.first, pos.second);
                    std::cout << Board::SPACE;
                }

                clear_screen();
                board->print();
                return; // Exit after reset
            }
        }
    }
}
