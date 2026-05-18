#pragma once
#include "Enemy.h"

class Mario;
class Board;

class Ghost : public Enemy {

    bool isAlive = false;

public:

    // Constructors:

    Ghost() {}
    Ghost(Point ghost);

	// Methods:
    void move(Board board);
  
    void drawGhostVector(Board& board);

    void eraseAndMoveGhosts(Board& board);

    bool getIsAlive() const { return isAlive; }

    void setIsAlive(bool alive) { isAlive = alive; }

};


