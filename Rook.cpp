#include "Rook.h"
#include "Logic.h"

Piece* Rook::clone() const
{
	return new Rook(*this);
}

Rook::Rook(PieceColor color) :Piece(PieceType::rook)
{
	setSymbol(L'\u2656');
	setColor(color);
	canMoveTo = &Logic::RookCan;
}
