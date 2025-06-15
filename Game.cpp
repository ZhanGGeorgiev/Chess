#include "Game.h"
#include "Logic.h"

bool Game::ChekStatus()
{
	if (status == gameStatus::remiOffer)
	{

		std::wcout << "ACCEPT REMI: YES/NO: ";
		String acc;
		std::cin >> acc;

		while(acc != "YES" && acc != "NO")
			std::cin >> acc;

		if (acc == "YES")
		{
			status = gameStatus::draw;
			return true;
		}

		if (acc == "NO")
		{
			status = gameStatus::normal;
			return false;
		}
	}


	return false;
}


void Game::running()
{
	while (true)
	{
		Board::getInstance().print();
		mv->makeMove(*player1, *player2);
		
		if (ChekStatus())
		{
			break;
		}

		if (!Logic::hasLeagalMovess(*player1, *player2))
		{
			status = gameStatus::whiteWin;
			break;
		}

		Board::getInstance().print();
		mv->makeMove(*player2, *player1);
		
		if (ChekStatus())
		{
			break;
		}
		if (!Logic::hasLeagalMovess(*player2, *player1))
		{
			status = gameStatus::blackWin;
			break;
		}
	}
	End();

}

void Game::End()
{
	if (status == gameStatus::whiteWin)
	{
		std::wcout << "WHITE WIN";
	}

	if (status == gameStatus::blackWin)
	{
		std::wcout << "Black WIN";
	}

	if (status == gameStatus::draw)
	{
		std::wcout << "NO 1 WIN";
	}
}


	
Game::Game()
{

	player1 = new Player("player1", PieceColor::white);
	player2 = new Player("player2", PieceColor::black);

	player1->getSet()->placeOn(Board::getInstance());
	player2->getSet()->placeOn(Board::getInstance());

	mv = new MoveContainer();

	
}

void Game::free()
{
	delete player1;
	delete player2;
	delete mv;
}

Game::~Game()
{
	free();
}

Game& Game::getInstance()
{
	static Game game;
	return game;
}

void Game::setStatus(gameStatus status)
{
	this->status = status;
}