#include "Knight.h"
#include "Logic.h"
Piece* Knight::clone() const
{
	return new Knight(*this);
}

Knight::Knight(PieceColor color) :Piece(PieceType::knight)
{
	setSymbol(L'\u2658');
	setColor(color);
	canMoveTo = &Logic::KnightCan;
}