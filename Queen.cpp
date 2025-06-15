#include "Queen.h"
#include "Logic.h"

Piece* Queen::clone() const
{
	return new Queen(*this);
}

Queen::Queen(PieceColor color) :Piece(PieceType::queen)
{
	setSymbol(L'\u2655');
	setColor(color);
	canMoveTo = &Logic::QueenCan;
}