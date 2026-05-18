#pragma once

#include "Point.h"


class Hammer
{
    Board* board = nullptr;
    Point originalLocation;
    Point activationLocation;
    bool isHammerActive = false;
	char hammerSymbol = 'p';
    bool hammerBeenActivated = false;
	static constexpr int HAMMER_ANIMATION = 4;

public:
    Hammer() {}

    // Hammer animation function
    void hammerAnimation(Point& hammerPos, Point& marioPos);

    // Getter and setter for originalLocation
    Point getOriginalLocation() const { return originalLocation; }
    void setOriginalLocation(const Point& location) { originalLocation = location; }

    // Getter and setter for activationLocation
    Point getActivationLocation() const { return activationLocation; }
    void setActivationLocation(const Point& location) { activationLocation = location; }

    // Getter and setter for isHammerActive
    bool getIsHammerActive() const { return isHammerActive; }
    void setIsHammerActive(bool hammerActive) { isHammerActive = hammerActive; }

    // Getter and setter for hammerSymbol
    char getHammerSymbol() const { return hammerSymbol; }
    void setHammerSymbol(char symbol) { hammerSymbol = symbol; }

    // Getter and setter for hammerBeenActivated
    bool getHasHammerBeenActivated() const { return hammerBeenActivated; }
    void setHasHammerBeenActivated(bool hammerActivated) { hammerBeenActivated = hammerActivated; }

    void setBoard(Board& newBoard) {
        board = &newBoard;  // Assign the new board reference
        originalLocation.setBoard(newBoard);
    }
};
