#include "Pawn.h"
#include "Logic.h"

Piece* Pawn::clone() const
{
	return new Pawn(*this);
}

Pawn::Pawn(PieceColor color) :Piece(PieceType::pawn)
{
	setSymbol(L'\u265F');
	setColor(color);

	if (color == PieceColor::white)
	{
		canMoveTo = &Logic::WPawnCan;
		canTake = &Logic::WPawnTake;
	}

	if (color == PieceColor::black)
	{
		canMoveTo = &Logic::BPawnCan;
		canTake = &Logic::BPawnTake;
	}
}