#pragma once
#include <cmath>

namespace Chess {
    enum class Color { White, Black, None };
    enum class PieceType { Pawn, Rook, Knight, Bishop, Queen, King, None };
    enum class GameState { Ongoing, Checkmate, Stalemate, Draw };

    struct Position {
        int row;
        int col;
        bool operator==(const Position& other) const { return row == other.row && col == other.col; }
        bool operator!=(const Position& other) const { return !(*this == other); }
        [[nodiscard]] int toIndex() const { return row * 8 + col; }
        [[nodiscard]] bool isValid() const { return row >= 0 && row < 8 && col >= 0 && col < 8; }
    };

    inline Color oppositeColor(Color c) {
        return (c == Color::White) ? Color::Black : Color::White;
    }
}