#pragma once
#include<iostream>
#include "SetOfPieces.h"

class Player
{
private:
	char* name;
	PieceColor color;

	void copy(const Player& other);
	void free();

	SetOfPieces* playerSet;

public:
	Player() = delete;
	Player(const char* name, PieceColor color);

	const SetOfPieces* getSet()const;
	SetOfPieces* getSet();

	Player(const Player& other);      
	Player(Player&& other)noexcept;

	Player& operator=(const Player& other);  
	Player& operator=(Player&& other)noexcept;
	~Player();                                

	const char* getName() const;
	char* getName();

	PieceColor getColor()const;
};

