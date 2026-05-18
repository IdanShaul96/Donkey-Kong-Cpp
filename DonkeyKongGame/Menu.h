#pragma once
#include <vector>
class Menu
{

public:
	void displayMenu(); //display main game navigating menu
	void displayInstructions(); //game instructions and movement keys
	void startMenu(); // the main function to navigate between screens according to user's choice
	void getAllBoardFileNames(std::vector<std::string>& screenNames); //get screen files from the working directory
	void printFileNames(const std::vector<std::string>& fileNames); //for "run fron a specific screen" option

};
