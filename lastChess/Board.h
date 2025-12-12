#pragma once
#include <array>
#include <memory>
#include <fstream>
#include <string> 
#include "Defs.h"
#include "Piece.h"

namespace Chess {
    class Board {
    private:
        std::array<std::unique_ptr<Piece>, 64> grid;
        Position enPassantTarget;
        int halfMoveClock;

        void moveInternal(Position from, Position to);
        void handleCastling(Position from, Position to);
        void handleEnPassant(Position from, Position to, const Piece* p);
        void updateEnPassantStatus(Position from, Position to, const Piece* p);

    public:
        Board();

        // Disable copy
        Board(const Board&) = delete;
        Board& operator=(const Board&) = delete;

        [[nodiscard]] std::unique_ptr<Board> clone() const;
        void resetBoard();
        void print() const;

        void save(std::ofstream& out) const;
        void load(std::ifstream& in);

        [[nodiscard]] Piece* getPiece(Position pos) const;
        [[nodiscard]] Piece* getPiece(int r, int c) const;

        [[nodiscard]] bool isFree(Position pos) const;
        [[nodiscard]] bool isFree(int r, int c) const;

        [[nodiscard]] Position findKing(Color c) const;
        [[nodiscard]] Position getEnPassantTarget() const;

        [[nodiscard]] int getHalfMoveClock() const { return halfMoveClock; }
       
        [[nodiscard]] std::string getSignature() const;

        void movePiece(Position from, Position to);
        void promotePiece(Position pos, PieceType type);
    };
}