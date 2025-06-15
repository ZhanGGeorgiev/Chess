#include "Bishop.h"
#include "Logic.h"

Piece* Bishop::clone() const
{
	return new Bishop(*this);
}

Bishop::Bishop(PieceColor color) :Piece(PieceType::bishop)
{
	setSymbol(L'\u2657');
	setColor(color);
	canMoveTo = &Logic::BishopCan;
}
