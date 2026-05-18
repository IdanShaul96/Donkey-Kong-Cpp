#include "Hammer.h"
#include "Mario.h"
#include "Point.h"

//HAMMER ANIMATION:

void Hammer::hammerAnimation(Point& hammerPos , Point& marioPos ) {
	
	
	//animation in STAY not showing on screen


	if (marioPos.getDir() == marioPos.getDirectionFromArr(Point::STAY))
	{
		if (marioPos.getPrevDir() == marioPos.getDirectionFromArr(Point::RIGHT))
		{
			hammerPos.setSymbol('p');
			
		}
		else if (marioPos.getPrevDir() == marioPos.getDirectionFromArr(Point::LEFT))
		{
			hammerPos.setSymbol('q');
			
		}
	    else //STAY
		{
			Point hammerPosRight = hammerPos;
			Point hammerPosLeft = hammerPos;

			hammerPosRight.setX(marioPos.getX() + 1);
			hammerPosRight.setSymbol('p');

			hammerPosLeft.setX(marioPos.getX() - 1);
			hammerPosLeft.setSymbol('q');

			// Draw the symbols
			gotoxy(marioPos.getX(), marioPos.getY());
			std::cout << marioPos.getSymbol();
			gotoxy(hammerPosRight.getX(), hammerPosRight.getY());
			std::cout << hammerPosRight.getSymbol();
			gotoxy(hammerPosLeft.getX(), hammerPosLeft.getY());
			std::cout << hammerPosLeft.getSymbol();

			Sleep(30);   // Delay

			// Erase the symbols
			gotoxy(hammerPosRight.getX(), hammerPosRight.getY());
			std::cout << ' ';
			gotoxy(hammerPosLeft.getX(), hammerPosLeft.getY());
			std::cout << ' ';
			Sleep(30);   // Delay
		}

	}

	else if (marioPos.getDir() == marioPos.getDirectionFromArr(Point::RIGHT))
	{
		hammerPos.setSymbol('p');
	}
	else
	{
		//hammerPos.setX(marioPos.getX() - 1);
		hammerPos.setSymbol('q');
	}

	//animation
	for (int i = 0; i < HAMMER_ANIMATION; ++i)
	{ 
		// Draw the symbol
		gotoxy(marioPos.getX(), marioPos.getY());
		std::cout << marioPos.getSymbol();
		gotoxy(hammerPos.getX(), hammerPos.getY());
		std::cout << hammerPos.getSymbol();

		Sleep(30);   // Delay

		// Erase the symbol
		gotoxy(hammerPos.getX(), hammerPos.getY());
		std::cout << ' ' ;
		Sleep(30);   // Delay 
	}
}

