#include "MoveValidator.h"

namespace Chess {

    GameState MoveValidator::getGameState(const Board& board, Color activeColor) {
        if (board.getHalfMoveClock() >= 100) return GameState::Draw;

        if (isInsufficientMaterial(board)) return GameState::Draw;

        bool inCheck = isKingInCheck(board, activeColor);
        bool canMove = hasAnyLegalMoves(board, activeColor);

        if (inCheck && !canMove) return GameState::Checkmate;
        if (!inCheck && !canMove) return GameState::Stalemate;

        return GameState::Ongoing;
    }

    bool MoveValidator::isInsufficientMaterial(const Board& board) {
        int whitePieces = 0;
        int blackPieces = 0;
        int whiteMinors = 0; 
        int blackMinors = 0;

        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Piece* p = board.getPiece(r, c);
                if (!p) continue;

                if (p->getType() == PieceType::Pawn || p->getType() == PieceType::Queen || p->getType() == PieceType::Rook) {
                    return false; 
                }

                if (p->getColor() == Color::White) {
                    whitePieces++;
                    if (p->getType() == PieceType::Bishop || p->getType() == PieceType::Knight) whiteMinors++;
                }
                else {
                    blackPieces++;
                    if (p->getType() == PieceType::Bishop || p->getType() == PieceType::Knight) blackMinors++;
                }
            }
        }

        if (whiteMinors == 0 && blackMinors == 0) return true;

        if ((whiteMinors == 1 && blackMinors == 0) || (whiteMinors == 0 && blackMinors == 1)) return true;

        return false;
    }

    std::vector<Move> MoveValidator::getAllLegalMoves(const Board& board, Color color) {
        std::vector<Move> legalMoves;
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Piece* p = board.getPiece(r, c);
                if (p && p->getColor() == color) {
                    for (int tr = 0; tr < 8; ++tr) {
                        for (int tc = 0; tc < 8; ++tc) {
                            if (isMoveValid(board, { r, c }, { tr, tc })) {
                                legalMoves.push_back({ {r, c}, {tr, tc} });
                            }
                        }
                    }
                }
            }
        }
        return legalMoves;
    }

    bool MoveValidator::isMoveValid(const Board& board, Position from, Position to) {
        if (!isPseudoLegal(board, from, to)) return false;

        std::unique_ptr<Board> tempBoard = board.clone();
        tempBoard->movePiece(from, to);

        Piece* movedPiece = tempBoard->getPiece(to);
        return !isKingInCheck(*tempBoard, movedPiece->getColor());
    }

    bool MoveValidator::isKingInCheck(const Board& board, Color color) {
        Position kingPos = board.findKing(color);
        if (!kingPos.isValid()) return true;

        Color enemyColor = oppositeColor(color);
        return isSquareAttacked(board, kingPos, enemyColor);
    }

    bool MoveValidator::isSquareAttacked(const Board& board, Position pos, Color attackerColor) {
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Piece* p = board.getPiece(r, c);
                if (p && p->getColor() == attackerColor) {
                    if (isPseudoLegal(board, { r, c }, pos, true)) return true;
                }
            }
        }
        return false;
    }

    bool MoveValidator::hasAnyLegalMoves(const Board& board, Color color) {
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Piece* p = board.getPiece(r, c);
                if (p && p->getColor() == color) {
                    for (int tr = 0; tr < 8; ++tr) {
                        for (int tc = 0; tc < 8; ++tc) {
                            if (isMoveValid(board, { r, c }, { tr, tc })) return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    bool MoveValidator::isPseudoLegal(const Board& board, Position from, Position to, bool ignoreKingSpecial) {
        if (!from.isValid() || !to.isValid()) return false;
        if (from == to) return false;

        Piece* p = board.getPiece(from);
        if (!p) return false;

        Piece* target = board.getPiece(to);
        bool isEnPassant = (p->getType() == PieceType::Pawn && to == board.getEnPassantTarget());

        if (!isEnPassant && target && target->getColor() == p->getColor()) return false;

        switch (p->getType()) {
        case PieceType::Pawn:   return validatePawn(board, p, from, to);
        case PieceType::Rook:   return validateRook(board, from, to);
        case PieceType::Knight: return validateKnight(from, to);
        case PieceType::Bishop: return validateBishop(board, from, to);
        case PieceType::Queen:  return validateQueen(board, from, to);
        case PieceType::King:   return validateKing(board, p, from, to, ignoreKingSpecial);
        default: return false;
        }
    }

    bool MoveValidator::validatePawn(const Board& board, Piece* p, Position from, Position to) {
        int dir = (p->getColor() == Color::White) ? -1 : 1;
        int startRow = (p->getColor() == Color::White) ? 6 : 1;
        int dRow = to.row - from.row;
        int dCol = std::abs(to.col - from.col);

        if (dCol == 0 && dRow == dir) return board.isFree(to);

        if (dCol == 0 && dRow == 2 * dir && from.row == startRow) {
            return board.isFree(to) && board.isFree(from.row + dir, from.col);
        }

        if (dCol == 1 && dRow == dir) {
            Piece* target = board.getPiece(to);
            if (target && target->getColor() != p->getColor()) return true;
            if (to == board.getEnPassantTarget()) return true;
        }
        return false;
    }

    bool MoveValidator::validateRook(const Board& board, Position from, Position to) {
        if (from.row != to.row && from.col != to.col) return false;
        return isPathClear(board, from, to);
    }

    bool MoveValidator::validateBishop(const Board& board, Position from, Position to) {
        if (std::abs(from.row - to.row) != std::abs(from.col - to.col)) return false;
        return isPathClear(board, from, to);
    }

    bool MoveValidator::validateQueen(const Board& board, Position from, Position to) {
        bool straight = (from.row == to.row || from.col == to.col);
        bool diagonal = (std::abs(from.row - to.row) == std::abs(from.col - to.col));
        if (!straight && !diagonal) return false;
        return isPathClear(board, from, to);
    }

    bool MoveValidator::validateKnight(Position from, Position to) {
        int dRow = std::abs(from.row - to.row);
        int dCol = std::abs(from.col - to.col);
        return (dRow == 2 && dCol == 1) || (dRow == 1 && dCol == 2);
    }

    bool MoveValidator::validateKing(const Board& board, Piece* king, Position from, Position to, bool ignoreSpecial) {
        int dRow = std::abs(from.row - to.row);
        int dCol = std::abs(from.col - to.col);

        if (dRow <= 1 && dCol <= 1) return true;

        if (!ignoreSpecial && dRow == 0 && dCol == 2) {
            if (king->getHasMoved()) return false;
            if (to.col > from.col)
                return canCastle(board, from, { from.row, 7 }, { from.row, 5 }, { from.row, 6 });
            else
                return canCastle(board, from, { from.row, 0 }, { from.row, 3 }, { from.row, 2 });
        }
        return false;
    }

    bool MoveValidator::canCastle(const Board& board, Position kingPos, Position rookPos, Position empty1, Position empty2) {
        Piece* rook = board.getPiece(rookPos);
        if (!rook || rook->getType() != PieceType::Rook || rook->getHasMoved()) return false;

        if (!board.isFree(empty1) || !board.isFree(empty2)) return false;

        if (std::abs(kingPos.col - rookPos.col) > 3) {
            if (!board.isFree(kingPos.row, 1)) return false;
        }

        Color c = board.getPiece(kingPos)->getColor();
        Color enemy = oppositeColor(c);

        if (isSquareAttacked(board, kingPos, enemy)) return false;
        if (isSquareAttacked(board, empty1, enemy)) return false;

        return true;
    }

    bool MoveValidator::isPathClear(const Board& board, Position from, Position to) {
        int dRow = (to.row - from.row);
        int dCol = (to.col - from.col);

        int stepRow = (dRow == 0) ? 0 : (dRow > 0 ? 1 : -1);
        int stepCol = (dCol == 0) ? 0 : (dCol > 0 ? 1 : -1);

        int curR = from.row + stepRow;
        int curC = from.col + stepCol;

        while (curR != to.row || curC != to.col) {
            if (!board.isFree(curR, curC)) return false;
            curR += stepRow;
            curC += stepCol;
        }
        return true;
    }
}