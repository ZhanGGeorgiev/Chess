#include "Piece.h"

namespace Chess {
    Piece::Piece(Color c, PieceType t) : color(c), type(t), hasMoved(false) {}

    // Pawn
    Pawn::Pawn(Color c) : Piece(c, PieceType::Pawn) {}
    std::unique_ptr<Piece> Pawn::clone() const { return std::make_unique<Pawn>(*this); }
    wchar_t Pawn::getSymbol() const { return (color == Color::White) ? L'\u2659' : L'\u265F'; }

    // Rook
    Rook::Rook(Color c) : Piece(c, PieceType::Rook) {}
    std::unique_ptr<Piece> Rook::clone() const { return std::make_unique<Rook>(*this); }
    wchar_t Rook::getSymbol() const { return (color == Color::White) ? L'\u2656' : L'\u265C'; }

    // Knight
    Knight::Knight(Color c) : Piece(c, PieceType::Knight) {}
    std::unique_ptr<Piece> Knight::clone() const { return std::make_unique<Knight>(*this); }
    wchar_t Knight::getSymbol() const { return (color == Color::White) ? L'\u2658' : L'\u265E'; }

    // Bishop
    Bishop::Bishop(Color c) : Piece(c, PieceType::Bishop) {}
    std::unique_ptr<Piece> Bishop::clone() const { return std::make_unique<Bishop>(*this); }
    wchar_t Bishop::getSymbol() const { return (color == Color::White) ? L'\u2657' : L'\u265D'; }

    // Queen
    Queen::Queen(Color c) : Piece(c, PieceType::Queen) {}
    std::unique_ptr<Piece> Queen::clone() const { return std::make_unique<Queen>(*this); }
    wchar_t Queen::getSymbol() const { return (color == Color::White) ? L'\u2655' : L'\u265B'; }

    // King
    King::King(Color c) : Piece(c, PieceType::King) {}
    std::unique_ptr<Piece> King::clone() const { return std::make_unique<King>(*this); }
    wchar_t King::getSymbol() const { return (color == Color::White) ? L'\u2654' : L'\u265A'; }
}