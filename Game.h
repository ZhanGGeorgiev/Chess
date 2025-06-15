#pragma once
#include "Player.h"
#include "Board.h"
#include "MoveContainer.h"

enum class gameStatus
{
	normal,
	whiteWin,
	blackWin,
	remiOffer,
	draw
};


class Game
{
private:
	gameStatus status;
	Player* player1 ;
	Player* player2;

	MoveContainer* mv;
	void free();

public:
	static Game& getInstance();
	void setStatus(gameStatus status);
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;

	Game();
	~Game();

	void running();
	bool ChekStatus();
	void End();

	gameStatus GetStatus()const 
	{ 
		return status;
	}

};

