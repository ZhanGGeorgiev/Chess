#pragma once
#include <fstream>
#include<iostream>
#include <windows.h>
#include <io.h>
#include <fcntl.h>

#include "String.h"



class Factory;


namespace PieceData
{
	enum class PieceColor
	{
		white,
		black,
		undef
	};

	enum class PieceType
	{
		pawn,
		rook,
		queen,
		bishop,
		knight,
		king
	};
}

using namespace PieceData;

class Piece
{

private:
	PieceColor color;
	PieceType type;
	
	wchar_t symbol;

public:
	int* (*canMoveTo)(const String&, const String&, int& count) = nullptr;

	const PieceColor getColor()const;
	const PieceType getType()const;

	Piece() = delete;
	Piece(PieceType type);

	virtual Piece* clone()const = 0;
	virtual ~Piece() = default;
	virtual void print()const;

	void setSymbol(const wchar_t s);
	void setColor(PieceColor color);

	const wchar_t& getSymbol()const;
	const wchar_t& getSymbol();
};

void writePiece(std::ofstream& ofs, const Piece& piece);
Piece* readPiece(std::ifstream& ifs);

