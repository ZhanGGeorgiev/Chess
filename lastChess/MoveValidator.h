#pragma once
#include "Board.h"
#include <vector>

namespace Chess {
    struct Move { Position from; Position to; };

    class MoveValidator {
    public:
        static GameState getGameState(const Board& board, Color activeColor);
        static bool isMoveValid(const Board& board, Position from, Position to);
        static bool isKingInCheck(const Board& board, Color color);
        static std::vector<Move> getAllLegalMoves(const Board& board, Color color);

        static bool isInsufficientMaterial(const Board& board);

    private:
        static bool isSquareAttacked(const Board& board, Position pos, Color attackerColor);
        static bool hasAnyLegalMoves(const Board& board, Color color);
        static bool isPseudoLegal(const Board& board, Position from, Position to, bool ignoreKingSpecial = false);

        static bool validatePawn(const Board& board, Piece* p, Position from, Position to);
        static bool validateRook(const Board& board, Position from, Position to);
        static bool validateBishop(const Board& board, Position from, Position to);
        static bool validateQueen(const Board& board, Position from, Position to);
        static bool validateKnight(Position from, Position to);
        static bool validateKing(const Board& board, Piece* king, Position from, Position to, bool ignoreSpecial);

        static bool isPathClear(const Board& board, Position from, Position to);
        static bool canCastle(const Board& board, Position kingPos, Position rookPos, Position empty1, Position empty2);
    };
}