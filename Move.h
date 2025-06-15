#pragma once
#include "String.h"
#include "Board.h"


class Piece;

enum class MoveType
{
    normal,
    queenCastle,
    kingCastle,
    enPassant,
    remi,
    undef
};


class Move 
{
private:
    String from;
    String to;
    const Piece* moved;
    const Piece* captured;

public:
    Move() = default;
    Move(const String& from, const String& to, const Piece* moved, const Piece* captured);
    virtual ~Move() = default;
    virtual Move* clone() const = 0;
    virtual void apply() = 0;
    virtual void redo() = 0;

    String& getFrom();
    const String& getFrom()const;

    String& getTo();
    const String& getTo()const;

    const Piece* getCaptured();
};

