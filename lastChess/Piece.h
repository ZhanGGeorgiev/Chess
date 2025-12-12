#pragma once
#include "Defs.h"
#include <memory>
#include <iostream>

namespace Chess {
    class Piece {
    protected:
        Color color;
        PieceType type;
        bool hasMoved;

    public:
        Piece(Color c, PieceType t);
        virtual ~Piece() = default;

        [[nodiscard]] virtual std::unique_ptr<Piece> clone() const = 0;
        [[nodiscard]] virtual wchar_t getSymbol() const = 0;

        [[nodiscard]] Color getColor() const { return color; }
        [[nodiscard]] PieceType getType() const { return type; }
        [[nodiscard]] bool getHasMoved() const { return hasMoved; }
        void setHasMoved(bool moved) { hasMoved = moved; }
    };

    class Pawn : public Piece {
    public:
        explicit Pawn(Color c);
        std::unique_ptr<Piece> clone() const override;
        wchar_t getSymbol() const override;
    };

    class Rook : public Piece {
    public:
        explicit Rook(Color c);
        std::unique_ptr<Piece> clone() const override;
        wchar_t getSymbol() const override;
    };

    class Knight : public Piece {
    public:
        explicit Knight(Color c);
        std::unique_ptr<Piece> clone() const override;
        wchar_t getSymbol() const override;
    };

    class Bishop : public Piece {
    public:
        explicit Bishop(Color c);
        std::unique_ptr<Piece> clone() const override;
        wchar_t getSymbol() const override;
    };

    class Queen : public Piece {
    public:
        explicit Queen(Color c);
        std::unique_ptr<Piece> clone() const override;
        wchar_t getSymbol() const override;
    };

    class King : public Piece {
    public:
        explicit King(Color c);
        std::unique_ptr<Piece> clone() const override;
        wchar_t getSymbol() const override;
    };
}