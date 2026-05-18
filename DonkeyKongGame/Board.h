#pragma once
#include <vector>
#include <string>
#include "Point.h"
#include "Ghost.h"
#include "Hammer.h"

// Forward declaration:
class Barrel;  
class Mario;

class Board {
    static constexpr int MAX_X = 80;
    static constexpr int MAX_Y = 25;
    static constexpr int WALL_MIN_BOUND = 0;
	static constexpr int ENTITY_VALUES_SIZE = 6;
    char originalBoard[MAX_Y][MAX_X + 1] = {};
    char currentBoard[MAX_Y][MAX_X + 1] = {}; // +1 for null terminator
    enum entities { DONKEY_KONG_COUNT = 0, MARIO_COUNT = 1, PAULINE_COUNT = 2 , LEGEND_COUNT = 3,HAMMER_COUNT=4, LEGEND_POS = 5};
    int entityValues[ENTITY_VALUES_SIZE] = {};
    bool marioInvalid = false, paulineInvalid = false, donkeyKongInvalid = false, LegendInvalid = false,LegendInvalidPos = false, hammerInvalid =false;
	enum legendValidRange { MIN_X_BOUND = 20, MAX_X_BOUND = 60, MIN_Y_BOUND = 1, MAX_Y_BOUND = 22 };

public:
    static constexpr char GHOST = 'x';
    static constexpr char HAMMER = 'p';
    static constexpr char LEGEND = 'L';
    static constexpr char WALL = 'Q';
    static constexpr char BARREL = 'O';
    static constexpr char STRAIGHT_FLOOR = '=';
    static constexpr char RIGHT_FLOOR = '>';
    static constexpr char LEFT_FLOOR = '<';
    static constexpr char LADDER = 'H';
    static constexpr char SPACE = ' ';
    static constexpr char DONKEY_KONG = '&';
    std::vector<Barrel*> barrels;  // Vector to store all barrels
    std::vector<Ghost*> ghosts; // Vector to store all the ghosts

    Mario* mario = nullptr;
	Hammer* hammer = nullptr;

    //getters:

    char getChar(int x, int y) const { return currentBoard[y][x]; }

    bool hasReachedWallBounds(int newX, int newY) const {
        return (newX < WALL_MIN_BOUND || newX >= MAX_X || newY < WALL_MIN_BOUND || newY >= MAX_Y || getChar(newX, newY) == WALL) ? true : false;
    }

    char (*getOriginalBoard())[MAX_X + 1] { return originalBoard; }

    static int getMaxX() { return MAX_X; }
    static int getMaxY() { return MAX_Y; }
    static int getWallMinX() { return WALL_MIN_BOUND; }

    bool isMarioInvalid() const { return marioInvalid; }
    bool isPaulineInvalid() const { return paulineInvalid; }
    bool isDonkeyKongInvalid() const { return donkeyKongInvalid; }
    bool isHammerInvalid() const { return hammerInvalid; }
    bool isLegendInvaild() const { return LegendInvalid; }
    bool isLegendInvaildPos() const { return LegendInvalidPos; }


     // Setters:
    void setMarioInvalid(bool value){ marioInvalid = value; }

    void setPaulineInvalid(bool value)  { paulineInvalid = value;  }

    void setDonkeyKongInvalid(bool value)  {  donkeyKongInvalid = value;  }

    void setLegendInvalid(bool value) {  LegendInvalid = value;  }

    void setLegendInvalidPos(bool value) {  LegendInvalidPos = value; }

    void setHammerInvalid(bool value)
    {
        if (entityValues[HAMMER_COUNT] != 1)
        {
            hammerInvalid = value;
        }
    }


    
    void reset(); // reset board to its original state
    void print() const;

    bool inVaildScreen(size_t screenIndex, Point& legendPos);
    void manageBarrels(); // manage barrels movement
    void uploadBoardFromFile(const std::string& fileName, Mario& mario, Point& donkeyKong, Point& legendPos, Point& hammer);
    bool fileExists(const std::string& fileName); // Declaration

	void recreateGhosts();
    void resetGhosts(Board& board);

    void resetEntityValues();

    void printErrors();

};