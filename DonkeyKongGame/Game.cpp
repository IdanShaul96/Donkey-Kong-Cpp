#include <iostream>
#include <windows.h>
#include <conio.h>
#include <filesystem>
#include "Game.h"
#include "Board.h"
#include "Utils.h"
#include "Mario.h"
#include "Barrel.h"
#include "Menu.h"
#include "Ghost.h"
#include "Hammer.h"

// Controls the main game loop

void Game::run(std::vector<std::string>& fileNames)
{
    runFromScreen(fileNames, 0); // Start from the first screen by default 
}

void Game::runFromScreen(std::vector<std::string>& fileNames, size_t startIndex) 
{
    ShowConsoleCursor(false); // Hide the cursor for a better game experience
    numOfScreens = fileNames.size(); // Set the number of screens

    Point donkeyKongPos, legendPos, hammerPos;
    Mario player;
    Board board;
    Hammer hammer;

    for (size_t screenIndex = startIndex; screenIndex < fileNames.size(); ++screenIndex) 
    {
        const auto& filename = fileNames[screenIndex];
        hammerPos = hammer.getOriginalLocation();
        board.uploadBoardFromFile(filename, player, donkeyKongPos, legendPos, hammerPos); 

        if (board.inVaildScreen(screenIndex, legendPos)) 
        {
            std::cout << "The next screen is invaild!" << std::endl;
            std::cout << "Problems list of screen number: " << screenIndex + 1 << std::endl;
            board.printErrors();

            MessageBox(NULL, L"This screen is invalid! Press OK to move to the next screen", L"Invalid screen", MB_OK);
            board.resetEntityValues();
            if (screenIndex == fileNames.size() - 1) { // if last screen is invalid
                winGame(board);
            }
            continue;
        }

        Point originalPos(player.getX(), player.getY());
        board.reset();  // Reset the board state
        clear_screen(); 
        board.print(); // Print the initial state of the board

        player.setBoard(board); // Assign Mario and board
		hammer.setBoard(board); // Assign Hammer and board
        player.freeFall(); //ensure @ isn't stuck in the air
		Barrel barrels(donkeyKongPos); //barrels initial position depends on donkeyKong
        Ghost ghosts;

		board.mario = &player; // Assign Mario to the board
        board.hammer = &hammer;
		hammer.setOriginalLocation(hammerPos);

        while (!player.getHasWon()) 
        {
            updateBoard(legendPos, hammer.getIsHammerActive());// Update and draw lives

            // Draw Mario and his enemies:
            player.draw();
            ghosts.drawGhostVector(board);
            barrels.drawBarrelsVector(board);
            barrels.TimeTodrawANewBarrel(board, donkeyKongPos); // Measure time differences to draw barrels at a pre-set time interval
            donkeyKongPos.setSymbol(Board::DONKEY_KONG);
            donkeyKongPos.draw();



            for (int i = 0; i < INPUT_BUFFER_SIZE; i++) {
                if (player.getHasWon()) {
                    break;
                }
                if (_kbhit()) {
                    char pressedKey = _getch();  // Get the pressed key if it was pressed
                    if (pressedKey == ESC) {
                        pauseGame(); // Pause the game if ESC is pressed
                    }
                   

                    player.keyPressed(pressedKey, hammer); // Handle the key press + check if hammer has been activated (used)
                  
                }
            }


            Sleep(100);

            player.erase(); // Erase Mario (@)
            ghosts.eraseAndMoveGhosts(board);
            barrels.eraseAndMoveBarrels(board);  // Move the barrels on the board
            barrels.removeBombingBarrels(board); // If barrel exploded - remove it from the board

            player.move();  // Move Mario and check for collisions


			if (player.getX() == hammerPos.getX() && player.getY() == hammerPos.getY()) { // If Mario is on the hammer
                hammer.setIsHammerActive(true); // Activate the hammer
            }

			if (hammer.getHasHammerBeenActivated()) //if hammer is allowed to be used by all conditions
			{

                Point marioPos(player.getMarioPoint());
				Point activationLocation(player.getX() + player.getMarioPoint().getDir().x, player.getY()+ player.getMarioPoint().getDir().y);
                hammer.setActivationLocation(activationLocation);
                Point hammerActivationPos =hammer.getActivationLocation();
				hammer.hammerAnimation(hammerActivationPos,marioPos);
				hammer.setHasHammerBeenActivated(false);
                increaseScore();

			}

			player.resetIfCollisionWithEnemy();

            if (player.getHasCollided()) 
            {
                std::cout.flush();
                setRemainingLives(); // Reduce lives count
                board.reset();  // Reset the board to its initial state.
				board.recreateGhosts(); // Reset the ghosts
				hammer.setIsHammerActive(false); // Deactivate the hammer
				player.resetPositionOriginal(originalPos); // Reset Mario's position and make him STAY there
                if (getRemainingLives() == 0) // Lost the game
                { 
                    restoreRemainingLives();
                    int finalScore = getScore();
                    std::wstring message = L"Game is Over - returning to main menu\nThe final score is: " + std::to_wstring(finalScore);
                    MessageBox(NULL, message.c_str(), L"You Have LOST!", MB_OK);
                    resetScore();
                    clear_screen();
                    return; // Exit the runFromScreen method to stop the game completely
                }
                player.setHasCollided(false);
            }
        }

        if (player.getHasWon()) // won each level individually
        { 
            increaseScore();
            board.resetEntityValues();
            board.resetGhosts(board);
            hammer.setIsHammerActive(false);
            if (screenIndex != fileNames.size() - 1)
            {
                player.setHasWon(false);
            }
        }

		if (player.getHasWon() && screenIndex == fileNames.size() - 1) // Won the last screen
        {
            winGame(board);
            resetScore();
            break;
        }
    }
}

void Game::pauseGame() // If ESC is pressed
{ 
    MessageBox(NULL, L"The game has been paused! Press OK and then ESC to continue.", L"Game Paused", MB_OK);

    // Wait for the second ESC key press to resume
    while (true) {
        if (_kbhit()) {
            char pressedKey = _getch();  // Get the pressed key if it was pressed
            if (pressedKey == ESC) {
                break;  // Exit the loop and continue the game
            }
        }
    }
}

void Game::winGame(Board& board) const // If the game is won
{
    // Show a MessageBox with a custom message (use wide-character strings)
    int finalScore = getScore();
    std::wstring message = L"You have cleared all the screens! You Won!\nThe final score is: " + std::to_wstring(finalScore);
    MessageBox(NULL, message.c_str(), L"Game is Over", MB_OK);

    // Clear the screen and reset the board
    clear_screen();
    board.reset();
}

void Game::updateBoard(Point& legendPos, bool isActive) // Update the board with the current lives, score, and hammer state
{
    Point lives(legendPos.getX() + LIVES_X, legendPos.getY()+LIVES_Y); // Initialize Point for lives display       
    Point scorePos(legendPos.getX() + SCORE_X, legendPos.getY() + SCORE_Y); // Initialize Point for score display       
    Point hammer(legendPos.getX() + HAMMER_X, legendPos.getY() + HAMMER_Y); // Initialize Point for hammer display  

    // Print lives as char on board
    lives.setSymbol('0' + getRemainingLives());
    lives.draw();

    // Convert score to string and print each character
    std::string scoreStr = std::to_string(getScore());
	for (int i = 0; i < scoreStr.size(); ++i) // Print each character of the score
    {
        Point scoreCharPos(scorePos.getX() + i, scorePos.getY());
        scoreCharPos.setSymbol(scoreStr[i]);
        scoreCharPos.draw();
    }

    // Move to the hammer's position
    gotoxy(hammer.getX(), hammer.getY());

    // Print hammer state in color

    if (isActive) {
        setColor(10); // Green for ACTIVE
        std::cout << "ACTIVE  ";
    }
    else {
        setColor(12); // Red for DISABLED
        std::cout << "DISABLED";
    }

    setColor(7); // Reset color to default
}


