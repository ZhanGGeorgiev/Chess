#pragma once
#include "Move.h"

class KingRook : public Move
{

public:
    KingRook() = default;
    void apply() override;
    KingRook(const String& from, const String& to, const Piece* moved, const Piece* captured)
        : Move(from, to, moved, captured) {};
    Move* clone() const override;
    void redo()override;
};

class QueenRook : public Move
{


public:
    QueenRook() = default;
    void apply() override;
    QueenRook(const String& from, const String& to, const Piece* moved, const Piece* captured)
        : Move(from, to, moved, captured) {};

    Move* clone() const override;
    void redo()override;
};
