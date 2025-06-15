#pragma once
#include "Move.h"

class EnPassant : public Move {

public:
    int taken;


    EnPassant(const String& from, const String& to, const Piece* moved, const Piece* captured)
        : Move(from, to, moved, captured) {};
    EnPassant() = default;
    void apply() override;
    Move* clone() const override;
    void redo()override;
};
