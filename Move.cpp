#include "Move.h"

Move::Move(const String& from, const String& to, const Piece* moved, const Piece* captured)
{
    this->from = from;
    this->to = to;
    this->moved = moved;
    this->captured = captured;
}

String& Move::getFrom()
{
    return from;
}

const String& Move::getFrom()const
{
    return from;
}

String& Move::getTo()
{
    return to;
}

const String& Move::getTo()const
{
    return to;
}

const Piece* Move::getCaptured()
{
    return captured;
}