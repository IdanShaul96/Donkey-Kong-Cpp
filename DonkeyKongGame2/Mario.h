#pragma once
#include <iostream>
#include "Point.h"
#include "Board.h" 

class Mario {

    Point marioPoint;

    Board* board=nullptr;  // Add a pointer to the Board object
    static constexpr int MARIO_MAX_FALL_COUNT = 5;
    static constexpr char MARIO = '@';
    static constexpr char PAULINE = '$';
    static constexpr int TOP_OF_VERTICAL = 2;
    static constexpr int HAMMER_HIT_RADIUS = 4;
    
    enum Movements{ MOVE_UP = -1, MOVE_DOWN = 1, MOVE_LEFT = -1, MOVE_RIGHT = 1, STOP = 0 };
    enum JumpPhases{ JUMP_PHASE_1=1, JUMP_PHASE_2, JUMP_PHASE_3, JUMP_PHASE_4, JUMP_PHASE_5};
    int jumpPhase = 0;

    // mario state:
    bool hasCollided = false; // mario's collision with barrel indicator
    bool isJumping = false;
    bool isFreeFalling = false;
    bool wasJumping = false;
    bool isReset = false; //back to original location on reset
    bool stillInTheAir = false;
    bool hasWon = false;

public:

    Mario() {
        marioPoint.setSymbol(MARIO);  // Set the symbol for Mario
    }

    Mario(Board& board) : board(&board) {  // Initialize the board reference
        Mario(); //constructor delegation
    }

    // Set the board for Mario's point
    void setBoard(Board& newBoard) {
        board = &newBoard;  // Assign the new board reference
        marioPoint.setBoard(newBoard);
    }

    const Board& getBoard() const {
        return *board;  // Dereference the pointer to return the referenced Board object
    }

    // Key pressed handling for Mario
    void keyPressed(char key, Hammer& hammer);

    // Draw Mario using the Point's draw method
    void draw() const {
        marioPoint.draw();
    }

    // Erase Mario using the Point's erase method
    void erase() { marioPoint.erase(); }

    Point getMarioPoint() { return marioPoint; }

    int getX() const { return marioPoint.getX(); }
    void setX(int newX) { marioPoint.setX(newX); }

    int getY() const { return marioPoint.getY(); }
    void setY(int newY) { marioPoint.setY(newY); }

    char getSymbol() const { return marioPoint.getSymbol(); }
    void setSymbol(char newSymbol) { marioPoint.setSymbol(newSymbol); }

    bool getHasWon() const { return hasWon; }
    void setHasWon(bool won) { hasWon = won; }
    bool getHasCollided() const { return hasCollided; }
    void setHasCollided(bool collided) { hasCollided = collided; }

    // Getter function 
    static char getPaulineSymbol() { return  PAULINE; }
    static char getMarioSymbol() { return  MARIO; }

    // Getter for stillInTheAir
    bool isStillInTheAir() const {  return stillInTheAir; }
    void setStillInTheAir(bool inAir) { stillInTheAir = inAir;}


    // mario momvement functions:
    void freeFall();
    bool handleRunAndJump(int& newX, int& newY);
    void MarioPrevPosBasedOnKey(int& prevX, int& prevY, int newX, int newY, char& nextYchar);
    void LadderClimbingAndDescending(int newX, int newY, char currentChar, char nextChar, char prevChar, char charBelow, char nextYchar);
    void move();
    void jump(int newX, int newY);


    void resetPositionOriginal(Point pos);
    void resetIfCollisionWithBarrel(); //manage mario's reset on board
    bool isOnFloor();
    void resetIfCollisionWithGhost();
    void resetIfCollisionWithEnemy();
};
