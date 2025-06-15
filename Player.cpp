#include "Player.h"
#pragma warning(disable : 4996)

Player::Player(const char* name, PieceColor color)
{
	size_t len = strlen(name);
	this->name = new char [len + 1];
	strcpy(this->name, name);

	this->color = color;
	playerSet = new SetOfPieces(this->color);
}

void Player::copy(const Player& other)
{
	delete[] name;

	size_t len = strlen(other.name);
	this->name = new char[len + 1];
	strcpy(this->name, other.name);

	color = other.color;
	playerSet = other.playerSet;
}

void Player::free()
{
	delete [] name;
}

Player::~Player()
{
	free();
}

Player::Player(const Player& other)
{
	copy(other);
}

Player& Player::operator=(const Player& other)
{
	if (this != &other)
	{
		free();
		copy(other);
	}
	return *this;
}

const char* Player::getName() const 
{
	return name;
}

char* Player::getName()
{
	return name;
}

Player::Player(Player&& other) noexcept
{
	name = other.name;
	other.name = nullptr;
}

Player& Player::operator=(Player&& other) noexcept 
{
	if (this != &other) {
		delete[] name;
		name = other.name;
		other.name = nullptr;
	}
	return *this;
}

PieceColor Player::getColor()const
{
	return color;
}


const SetOfPieces* Player::getSet()const
{
	return playerSet;
}

SetOfPieces* Player::getSet()
{
	return playerSet;
}