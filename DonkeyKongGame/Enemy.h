#pragma once
#include <iostream>
#include <string>
#include "Utils.h"
#include "Point.h"

// Forward declaration
class Board;

//BASE CLASS: ENEMY . DERIVED CLASSES: BARREL & GHOST

class Enemy {
    Point location;  // Holds the position and direction of the enemy
    char symbol;     // The symbol representing the enemy
    int direction;   // 1 for right, -1 for left

public:
    // Constructors
    Enemy();
    Enemy(Point p);

    // Virtual method to set symbol
     void setSymbol(char newSymbol) { symbol = newSymbol; }

     char getSymbol() const { return symbol; }

    // Getter for location
    Point getLocation() const { return location; }

    // Virtual method to draw the enemy
    void draw() const {
        gotoxy(location.getX(), location.getY());
        std::cout << symbol;
    }

    // Erase the enemy using the Point's erase method
    void erase();

    // Sets the board for the enemy
    void setBoard(Board& board);

    // Sets and gets the fall count
    void setFallCount(int newFallCount) { location.setFallCount(newFallCount); }
    int getFallCount() const { return location.getFallCount(); }

    // Getters and setters for x and y coordinates
    int getX() const { return location.getX(); }
    void setX(int newX) { location.setX(newX); }

    int getY() const { return location.getY(); }
    void setY(int newY) { location.setY(newY); }

    int getDirection() const { return direction; }
    void setDirection(int dir) { direction = dir; }

    // Determines the new direction for the enemy
    Point getNewDir();
    void changeDirection();

     ~Enemy() {}
};

