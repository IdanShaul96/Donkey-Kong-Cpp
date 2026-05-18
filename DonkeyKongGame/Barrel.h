#pragma once
#include "Point.h"
#include <vector>
#include "Board.h"
#include "Enemy.h"

class Board;  // Forward declaration

class Barrel : public Enemy {

    char prevFloor = Board::SPACE;

    static constexpr int SPARK_RADIUS = 2;
    static constexpr char BARREL_SPARK = '*';
    static constexpr int BARREL_MAX_FALL_COUNT = 8;
    static constexpr int BARREL_INTERVAL = 2500; //milliseconds


    DWORD explosionTime=0;
    DWORD timeWhenStarDrawn=0;
    std::vector<std::pair<int, int>> explosionPositions;  // Store explosion positions

    ULONGLONG lastBarrelDrawTime = GetTickCount64();  // Track the last time a barrel was drawn
    ULONGLONG barrelInterval = BARREL_INTERVAL;  // Interval to draw a new barrel every 3000ms

public:
	
    //constructors:
    Barrel() {}
    Barrel(Point p);

    void move(Board& board);


    // BARRELS FUNCTIONS:
    void drawBarrelsVector(Board& board);
    void TimeTodrawANewBarrel(Board& board, Point& pos);
    void eraseAndMoveBarrels(Board& board);
    void removeBombingBarrels(Board& board);

    // Explosion management functions:
    void addExplosionPosition(int x, int y) {
        explosionPositions.push_back(std::make_pair(x, y));
    }

    const std::vector<std::pair<int, int>>& getExplosionPositions() const {
        return explosionPositions;
    }

    void animateExplosion(int centerX, int centerY);

    void clearExplosionPositions() {
        explosionPositions.clear();
    }

    void setExplosionTime(DWORD time) {
        this->explosionTime = time;
    }

    void setTimeWhenStarDrawn(DWORD time) {
        this->timeWhenStarDrawn = time;
    }

    DWORD getTimeWhenStarDrawn() const {
        return timeWhenStarDrawn;
    }
};
