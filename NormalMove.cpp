#include "NormalMove.h"


Move* NormalMove::clone() const
{
    return new NormalMove(*this);
}

void NormalMove::apply()
{
    Board::getInstance().movePieces(getFrom(), getTo());
}

void NormalMove::redo()
{
    Board::getInstance().movePieces(getTo(), getFrom());
    Board::getInstance().placePiece(getCaptured(), getTo());
}