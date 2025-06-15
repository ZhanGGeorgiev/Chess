#include<iostream>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include "Pawn.h"


#include "Board.h"
#include "Player.h"
#include "Logic.h"
#include "Game.h"

#include "MoveContainer.h"

int main()
{
	_setmode(_fileno(stdout), _O_U16TEXT);

	
	Board::getInstance();




	Game::getInstance().running();


	return 0;
}