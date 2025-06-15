#include "Castle.h"

Move* KingRook::clone() const
{
    return new KingRook(*this);
}

Move* QueenRook::clone() const
{
    return new QueenRook(*this);
}

void KingRook::redo()
{

}

void QueenRook::redo()
{

}

void KingRook::apply()
{
    
}

void QueenRook::apply()
{

}
