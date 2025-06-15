#include "EnPassant.h"


void EnPassant::apply()
{
	Board::getInstance().movePieces(getFrom(), getTo());
	Board::getInstance().removePiece(taken);

}

Move* EnPassant::clone() const
{
	return new EnPassant(*this);
}

void EnPassant::redo()
{
	Board::getInstance().movePieces(getTo(), getFrom());
	Board::getInstance().placePiece(getCaptured(), taken);
}