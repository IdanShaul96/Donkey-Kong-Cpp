#pragma once
#include "Board.h"
#include <vector>
#include <string>
#include <filesystem>

class Game
{
    size_t numOfScreens = 0; // Store the number of screens
    int remainingLives = 3;
    int score = 0;
    static constexpr int ESC = 27; // ASCII value of the ESC key
	static constexpr int MAX_LIVES = 3; // Maximum number of lives possible
	static constexpr int SCORE_INCREMENT = 500; // Score increment value
	static constexpr int INPUT_BUFFER_SIZE = 10; // Size of the input buffer for 1 frame
	enum legendRange { LIVES_X = 17, SCORE_X = 7, HAMMER_X = 8, LIVES_Y=0, SCORE_Y=1, HAMMER_Y=2 }; // Enum for the legend range

public:

    // Control the main game loop
    void run(std::vector<std::string>& vec_to_fill); //menu option 1: start game from screen 1
    void runFromScreen(std::vector<std::string>& vec_to_fill, size_t startIndex); //run from specific screen
    
    //getters:
    int getRemainingLives() const { return remainingLives; } // Returns the remaining lives count
	int getScore() const { return score; } // Returns the current score
	size_t getNumOfScreens() const { return numOfScreens; } // Returns the number of screens


    //setters:
    void setRemainingLives() { remainingLives--; } // Reduces lives count by one


    //score and more:
   
    void restoreRemainingLives() { remainingLives = MAX_LIVES; } // Restores lives count to initial value for game restart

    void resetScore() {  score =0; }

    void increaseScore() { score += SCORE_INCREMENT;  }

    void updateBoard(Point& legendPos, bool isActive);
    
    void pauseGame(); // Pauses the game when ESC is pressed
    
    void winGame(Board& board) const; // Handles actions when the game is won

    void setColor(int color) 
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }

};
