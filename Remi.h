#pragma once
#include "Move.h"

class Remi : public Move
{
private:

public:
    Remi() = default;
    void apply() override;
    Move* clone() const override;
    Remi(const String& from, const String& to, const Piece* moved, const Piece* captured)
        : Move(from, to, moved, captured) {};

    void redo()override;
};