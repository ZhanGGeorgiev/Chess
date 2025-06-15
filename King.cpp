#include "King.h"
#include "Logic.h"

Piece* King::clone() const
{
	return new King(*this);
}

King::King(PieceColor color) :Piece(PieceType::king)
{
	setSymbol(L'\u2654');
	setColor(color);
	canMoveTo = &Logic::KingCan;
}