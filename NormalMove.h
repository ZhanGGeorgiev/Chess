#pragma once
#include "Move.h"

class NormalMove: public Move
 {
 private:

    public:
        NormalMove() = default;
        void apply() override;
        void redo();

        Move* clone() const override;

        NormalMove(const String& from, const String& to, const Piece* moved, const Piece* captured)
            : Move(from, to, moved, captured) {};
};

