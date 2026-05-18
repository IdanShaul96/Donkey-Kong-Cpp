#include <iostream>
#include <string>
#include "Utils.h"
#include <conio.h>
#include "Menu.h"
#include "Game.h"

using namespace std;

// display the main menu
void Menu::displayMenu() {
    cout << "===========================\n";
    cout << "    Donkey Kong Menu\n";
    cout << "===========================\n";
    cout << "1. Start a New Game\n";
    cout << "2. Start a New Game from a specific screen\n";
    cout << "8. Instructions and Movement Keys\n";
    cout << "9. Exit\n";
    cout << "Please select one of these options: ";
}

// display game instructions
void Menu::displayInstructions() {
    clear_screen();
    cout << "\nInstructions:\n";
    cout << "1. Use the movement keys to move your character (@).\n";
    cout << "2. Avoid barrels (O) thrown by Donkey Kong (&).\n";
    cout << "3. Jump to avoid dangers and save Pauline ($) at the top!\n";
    cout << "\nMovement Keys:\n";
    cout << "LEFT: A or a\n";
    cout << "RIGHT: D or d\n";
    cout << "UP: W or w\n";
    cout << "DOWN: X or x\n";
    cout << "STAY IN PLACE: S or s\n";
    cout << "Press ENTER to return to the menu...\n";
    cin.get();  // Wait for a key press to return
    clear_screen();
}


void Menu::startMenu() {
    Game mainGame;
    char choice;  // get choice as char to handle invalid input
    std::vector<std::string> fileNames;
    getAllBoardFileNames(fileNames);
    size_t numOfScreens = fileNames.size();

    while (true) {
        displayMenu();

        choice = _getch();  // Read a single character input without pressing Enter
        clear_screen();
        switch (choice) {
        case '1':
            if (numOfScreens == 0) {
                MessageBox(NULL, L"There are no files found!", L"Error 404!", MB_OK);
                clear_screen();
                return; // Exit the loop and return to the menu
            }
            std::cout << "\nStarting a new game...\n";
            mainGame.run(fileNames);
            break;

        case '2':
            if (numOfScreens == 0) 
            {
                MessageBox(NULL, L"There are no files found!", L"Error 404!", MB_OK);
                clear_screen();
                return; // Exit the loop and return to the menu
            }
            while (true)
            {
                size_t startIndex;
                std::cout << "Enter the index of the screen you want to begin with\n";
                printFileNames(fileNames);
                std::cin >> startIndex;
                startIndex--;
                if (startIndex < numOfScreens) 
                {
                    mainGame.runFromScreen(fileNames, startIndex);
                    clear_screen();
                    break;
                }
                else {
                    clear_screen();
                    std::cout << "Invalid screen index. Please try again.\n";
                }
            }
            break;

        case '8':
            displayInstructions();
            break;

        case '9':
            clear_screen();
            std::cout << "\nExiting the game. Goodbye!\n";
            return;  // Exit the program

        default:
            clear_screen(); // Handle invalid input
            std::cout << "Invalid option. Please select a valid option.\n";
        }
    }
}


void Menu::getAllBoardFileNames(std::vector<std::string>& screenNames) {
    namespace fs = std::filesystem;
    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        auto filename = entry.path().filename();
        auto filenameStr = filename.string();
        if (filenameStr.substr(0, 5) == "dkong" && entry.path().extension() == ".screen")
        {
            screenNames.push_back(filenameStr);
        }
    }
}


void Menu::printFileNames(const std::vector<std::string>& fileNames) {
    size_t index = 1;
    for (const auto& fileName : fileNames) {
        std::cout << index << ". " << fileName << std::endl;
        ++index;
    }
}

