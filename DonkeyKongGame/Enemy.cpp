#include "Enemy.h"
#include "Board.h" // Include full definition of Board

// Default constructor
Enemy::Enemy() : direction(1), symbol('E') {}


// Constructor with location initialization
Enemy::Enemy(Point p) : location(p), symbol('E')
{
    direction = 1;
}

// Sets the board for the enemy
void Enemy::setBoard(Board& board) {
    location.setBoard(board);
}

// Erases the enemy from the board
void Enemy::erase() {
    gotoxy(location.getX(), location.getY());
    std::cout << Board::SPACE; // Use the static constexpr SPACE member from Board
}

// Determines the new direction for the enemy
Point Enemy::getNewDir() {
    Point newPoint;
    int newX = getX() + getLocation().getDir().x;
    int newY = getY() + getLocation().getDir().y;

    newPoint.setX(newX);
    newPoint.setY(newY);

    return newPoint;
}


void Enemy::changeDirection()
{
    setDirection(-getDirection());
}
