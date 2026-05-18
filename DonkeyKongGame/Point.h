#pragma once
#include <iostream>
#include "Utils.h"

// Forward declaration of Board
class Board;

class Point {
    static constexpr char keys[] = { 'w', 'a', 'x', 'd', 's' ,'p'};
    static constexpr size_t numKeys = sizeof(keys) / sizeof(keys[0]);

    struct Direction {
        int x, y;

        bool operator==(const Direction& other) const {
            return x == other.x && y == other.y;
        }
    };
	static constexpr Direction directions[] = { {0, -2}, {-1, 0}, {0, 1}, {1, 0}, {0, 0}, {0, 0} }; // Added {0, 0} for hammer (+STAY)
    static constexpr Direction UP_LEFT = { -1, -1 };
    static constexpr Direction DOWN_LEFT = { -1, 1 };
    static constexpr Direction UP_RIGHT = { 1, -1 };
    static constexpr Direction DOWN_RIGHT = { 1, 1 };
    static constexpr Direction CLIMB = { 0, -1 };

    Board* pBoard = nullptr; // Connect point to the board

    // Private member variables
    char symbol; // Default space character
    int x = 0, y = 0; // Initialize location coordinates
    int fallCount = 0;
    Direction dir{ 0, 0 };
    Direction prevDir{ 0, 0 };

    // Private method to draw character
    void draw(char c) const {
        gotoxy(getX(), getY());  // Use getter functions
        std::cout << c;
    }

public:
    static constexpr char DOWN = 'x';
    static constexpr char UP = 'w';
    static constexpr char LEFT = 'a';
    static constexpr char RIGHT = 'd';
    static constexpr char STAY = 's';
    static constexpr char HAMMER = 'p';


    // Constructor
    Point() : symbol(' ') {}
    Point(int newX, int newY) : x(newX), y(newY), symbol(' ') {}

    // Getter and setter for x coordinate
    int getX() const { return x; }
    void setX(int newX) { x = newX; }

    // Getter and setter for y coordinate
    int getY() const { return y; }
    void setY(int newY) { y = newY; }

    // Getter and setter for the point's specific char symbol
    char getSymbol() const { return symbol; }
    void setSymbol(char newSymbol) { symbol = newSymbol; }

    // Getter and setter for fallCount used separately by Mario and Barrel
    int getFallCount() const { return fallCount; }
    void setFallCount(int newFallCount) { fallCount = newFallCount; }

    // Getter and setter for current direction
    Direction getDir() const { return dir; }
    void setDir(const Direction& newDir) { dir = newDir; }

    // Getter and setter for previous Direction
    Direction getPrevDir() const { return prevDir; }
    void setPrevDir(const Direction& newPrevDir) { prevDir = newPrevDir; }

    // Draw the point on the board:
    void draw() const {
        draw(getSymbol());
    }

    // Erase the point on the board:
    void erase();

    // Handle keyboard input
    void keyPressed(char key);

    // Set the board for this point
    void setBoard(Board& board) {
        pBoard = &board;
    }

    const Board& getBoard() const {
        return *pBoard;  // Dereference the pointer to return the referenced Board object
    }

    // Get the character at the specified (x, y) position from the board
    char getNextChar(int x, int y);

    // Get direction from key press
    Direction getDirectionFromArr(char key);

    // Public method to access Direction structs
    Direction getUpLeft() const { return UP_LEFT; }
    Direction getDownLeft() const { return DOWN_LEFT; }
    Direction getUpRight() const { return UP_RIGHT; }
    Direction getDownRight() const { return DOWN_RIGHT; }
    Direction getClimb() const { return CLIMB; }
};
