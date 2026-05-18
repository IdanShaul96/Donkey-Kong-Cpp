#include "Board.h"
#include "Barrel.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring> 
#include "Mario.h"
#include <set>


// Set of allowed characters on the board for uploadBoardFromFile function:
std::set<char> allowedChars = { Board::LEFT_FLOOR, Board::RIGHT_FLOOR, Board::STRAIGHT_FLOOR, Board::LADDER, 
Mario::getMarioSymbol(), Mario::getPaulineSymbol(),
Board::DONKEY_KONG, Board::GHOST, Board::HAMMER, Board::LEGEND,Board::WALL };

void Board::reset() 
{
    for (int i = 0; i < MAX_Y; i++) {
        memcpy(currentBoard[i], originalBoard[i], MAX_X + 1);
    }
    barrels.clear();  // Clear the barrels vector
}

void Board::recreateGhosts() // Recreate the ghosts on the board
{
    for (int i = 0; i < MAX_Y; i++)
    {
        for (int j = 0; j < MAX_X; j++)
        {
            if (originalBoard[i][j] == GHOST)
            {
                Point ghostPoint(j, i);
                Ghost* newGhost = new Ghost(ghostPoint);
                ghosts.push_back(newGhost);
                newGhost->setIsAlive(true);
            }
        }
    }
}


void Board::print() const { // Print the current board
    for (int i = 0; i < MAX_Y - 1; i++) {
        std::cout << currentBoard[i] << '\n';
    }
    std::cout << currentBoard[MAX_Y - 1];
}

void Board::manageBarrels() { // Manage the barrels on the board
    Barrel barrel;
    Point point;
    barrel.TimeTodrawANewBarrel(*this, point);
    barrel.eraseAndMoveBarrels(*this);
    barrel.drawBarrelsVector(*this);
    barrel.removeBombingBarrels(*this);
}

// Function to upload the board from a txt file:
void Board::uploadBoardFromFile(const std::string& fileName, Mario& mario, Point& donkeyKong, Point& legendPos, Point& hammerPos) 
{


    if (!fileExists(fileName)) {
        std::cerr << "Error: File " << fileName << " does not exist.\n";
        return;
    }

    std::ifstream inputFile(fileName);
    if (!inputFile) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return;
    }

    int lineIndex = 0;
    std::string line;
    int bottomLeftMarioX = -1, bottomLeftMarioY = -1;
    bool legendPosSet = false; // Flag to track if legendPos has been set

    while (std::getline(inputFile, line) && lineIndex < MAX_Y) 
    {
        // Ignore characters beyond MAX_X :
        if (line.size() > MAX_X) {
            line = line.substr(0, MAX_X);
        }

        // Copy the contents of the line into originalBoard
        for (int i = 0; i < line.size(); ++i)
        {

            if (allowedChars.find(line[i]) == allowedChars.end())
            {
                originalBoard[lineIndex][i] = Board::SPACE;
            }
            else
            {
                originalBoard[lineIndex][i] = line[i];
            }


            if (originalBoard[lineIndex][i] == mario.getSymbol())
            {
                bottomLeftMarioX = i;
                bottomLeftMarioY = lineIndex;
                entityValues[MARIO_COUNT]++;
            }

            else if (originalBoard[lineIndex][i] == Board::DONKEY_KONG)
            {
                donkeyKong.setX(i);
                donkeyKong.setY(lineIndex);
                entityValues[DONKEY_KONG_COUNT]++;
            }

            else if (originalBoard[lineIndex][i] == Mario::getPaulineSymbol())
            {
                entityValues[PAULINE_COUNT]++;
            }
            else if (originalBoard[lineIndex][i] == Board::GHOST)
            {
                Point ghostPoint(i, lineIndex);
                Ghost* newGhost = new Ghost(ghostPoint);
                this->ghosts.push_back(newGhost);  // Add the new Ghost to the vector
                newGhost->setIsAlive(true);
            }
            
            if (originalBoard[lineIndex][i] == Board::LEGEND)
            {
                legendPos.setX(i);
                legendPos.setY(lineIndex);
                entityValues[LEGEND_COUNT]++;
                legendPosSet = true;

            }

            if (originalBoard[lineIndex][i] == Board::HAMMER)
            {
                hammerPos.setX(i);
                hammerPos.setY(lineIndex);
                entityValues[HAMMER_COUNT]++;
                legendPosSet = true;

            }
        }

        // Fill the remaining spaces with SPACE if the line is shorter than MAX_X
        for (size_t i = line.size(); i < MAX_X; ++i) {
            originalBoard[lineIndex][i] = SPACE;
        }

        lineIndex++;
    }

    // Remove all Mario symbols except the one at bottom-left position
    for (int y = 0; y < MAX_Y; ++y) {
        for (int x = 0; x < MAX_X; ++x) {
            if (originalBoard[y][x] == mario.getSymbol()) {
                if (x != bottomLeftMarioX || y != bottomLeftMarioY) {
                    originalBoard[y][x] = SPACE;
                }
            }
        }
    }


    // Replace 'L' with the string "Remaining Lives:\nScore:\nHammer:"
     if (legendPosSet)
     {
         int startX = legendPos.getX();
         int startY = legendPos.getY();
         std::string legendString = "Remaining Lives: \nScore: \nHammer: ";
         int index = 0;
         for (int dy = 0; dy < 3; ++dy)
         {
             // 3 lines of text
              for (int dx = 0; dx < MAX_X && index < legendString.length(); ++dx)
              {
                  if (legendString[index] == '\n')
                  {
                      ++index; break;
                      // Move to the next line in the legend string
                  }
                  if (startY + dy < MAX_Y && startX + dx < MAX_X)
                  {
                      originalBoard[startY + dy][startX + dx] = legendString[index];
                  } ++index;
              }
         } 
     }


    // Set Mario's position to the bottom-left position found
    if (bottomLeftMarioX != -1 && bottomLeftMarioY != -1) {
        mario.setX(bottomLeftMarioX);
        mario.setY(bottomLeftMarioY);
     

      int marioPosX = mario.getX();
      int marioPosY = mario.getY() + 1;

      if(originalBoard[marioPosY][marioPosX] == Board::SPACE)
      {
            mario.setStillInTheAir(true);
      }

      if (mario.isStillInTheAir())
      {

          originalBoard[bottomLeftMarioY][bottomLeftMarioX] = Board::SPACE;
          mario.freeFall();
          mario.setStillInTheAir(false);
      }
    }

    inputFile.close();
}

bool Board::inVaildScreen(size_t screenIndex, Point& legendPos)
{
    clear_screen();


    // printing the error of each problem on the screen.
    if (entityValues[HAMMER_COUNT] > 1)
    {
        setHammerInvalid(true);
    }

    if (entityValues[DONKEY_KONG_COUNT] != 1)
    {
        setDonkeyKongInvalid(true);
    }
    if (entityValues[PAULINE_COUNT] != 1)
    {
        setPaulineInvalid(true);
    } 
    if (entityValues[MARIO_COUNT] == 0)
    {
        setMarioInvalid(true);
    }
    
    if (entityValues[LEGEND_COUNT] != 1 )
    {
        setLegendInvalid(true);
    }

    if (legendPos.getX() > MAX_X_BOUND || legendPos.getX() < MIN_X_BOUND || legendPos.getY() > MAX_Y_BOUND || legendPos.getY() < MIN_Y_BOUND)
    {
        setLegendInvalidPos(true);
    }
    
    // determinating the validation of the screen, and sending the result to game.cpp:

    return (marioInvalid || paulineInvalid || donkeyKongInvalid || LegendInvalid || LegendInvalidPos || hammerInvalid);

}

bool Board::fileExists(const std::string& fileName) {  // Definition of fileExists
    std::ifstream file(fileName);
    return file.is_open();  // Returns true if the file can be opened, false otherwise
}

void Board::resetEntityValues()
{ 
    for (int i = 0; i < ENTITY_VALUES_SIZE-1; ++i)
    { 
        this->entityValues[i] = 0; 
    }
}

void Board::printErrors()
{
    if (this->isMarioInvalid())
    { 
        std::cout << "Incompatible quantity of  Mario '@' " << std::endl;
        this->setMarioInvalid(false);
    }
    if (this->isPaulineInvalid())
    {
        std::cout << "Incompatible quantity of Pauline '$' " << std::endl;
        this->setPaulineInvalid(false);
    }
    if (this->isDonkeyKongInvalid())
    {
        std::cout << "Incompatible quantity of Donkey Kong '&' " << std::endl;
        this->setDonkeyKongInvalid(false);
    }
    if (this->isHammerInvalid())
    {
        std::cout << "Incompatible quantity of  hammers 'p' " << std::endl;
        this->setHammerInvalid(false);
    }
    if (this->isLegendInvaild())
    {
        std::cout << "Incompatible quantity of  Legend 'L' " << std::endl;
        this->setLegendInvalid(false);
    }
    if (this->isLegendInvaildPos())
    {
        std::cout << "There's not enough space for the Legend (gets out of screen range)" << std::endl;
        this->setLegendInvalidPos(false);
    }
}

void Board::resetGhosts(Board& board)
{
    board.ghosts.clear();
}