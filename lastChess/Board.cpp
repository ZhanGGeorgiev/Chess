#include "Board.h"
#include <iostream>
#include <cmath>
#include <sstream>

namespace Chess {
    Board::Board() : enPassantTarget({ -1, -1 }), halfMoveClock(0) {
        resetBoard();
    }

    std::unique_ptr<Board> Board::clone() const {
        auto newBoard = std::make_unique<Board>();
        for (int i = 0; i < 64; ++i) {
            newBoard->grid[i].reset();
            if (grid[i]) {
                newBoard->grid[i] = grid[i]->clone();
            }
        }
        newBoard->enPassantTarget = this->enPassantTarget;
        newBoard->halfMoveClock = this->halfMoveClock;
        return newBoard;
    }

    void Board::resetBoard() {
        for (auto& p : grid) p.reset();
        enPassantTarget = { -1, -1 };
        halfMoveClock = 0;

        auto place = [&](int r, int c, Piece* p) { grid[r * 8 + c].reset(p); };

        // Black Pieces
        place(0, 0, new Rook(Color::Black));   place(0, 7, new Rook(Color::Black));
        place(0, 1, new Knight(Color::Black)); place(0, 6, new Knight(Color::Black));
        place(0, 2, new Bishop(Color::Black)); place(0, 5, new Bishop(Color::Black));
        place(0, 3, new Queen(Color::Black));  place(0, 4, new King(Color::Black));
        for (int i = 0; i < 8; ++i) place(1, i, new Pawn(Color::Black));

        // White Pieces
        place(7, 0, new Rook(Color::White));   place(7, 7, new Rook(Color::White));
        place(7, 1, new Knight(Color::White)); place(7, 6, new Knight(Color::White));
        place(7, 2, new Bishop(Color::White)); place(7, 5, new Bishop(Color::White));
        place(7, 3, new Queen(Color::White));  place(7, 4, new King(Color::White));
        for (int i = 0; i < 8; ++i) place(6, i, new Pawn(Color::White));
    }

    void Board::save(std::ofstream& out) const {
        out.write(reinterpret_cast<const char*>(&enPassantTarget), sizeof(Position));
        out.write(reinterpret_cast<const char*>(&halfMoveClock), sizeof(int)); // Save Clock

        for (int i = 0; i < 64; ++i) {
            bool hasPiece = (grid[i] != nullptr);
            out.write(reinterpret_cast<const char*>(&hasPiece), sizeof(bool));
            if (hasPiece) {
                PieceType t = grid[i]->getType();
                Color c = grid[i]->getColor();
                bool m = grid[i]->getHasMoved();
                out.write(reinterpret_cast<const char*>(&t), sizeof(PieceType));
                out.write(reinterpret_cast<const char*>(&c), sizeof(Color));
                out.write(reinterpret_cast<const char*>(&m), sizeof(bool));
            }
        }
    }

    void Board::load(std::ifstream& in) {
        for (auto& p : grid) p.reset();
        in.read(reinterpret_cast<char*>(&enPassantTarget), sizeof(Position));
        in.read(reinterpret_cast<char*>(&halfMoveClock), sizeof(int)); // Load Clock

        for (int i = 0; i < 64; ++i) {
            bool hasPiece = false;
            in.read(reinterpret_cast<char*>(&hasPiece), sizeof(bool));
            if (hasPiece) {
                PieceType t; Color c; bool m;
                in.read(reinterpret_cast<char*>(&t), sizeof(PieceType));
                in.read(reinterpret_cast<char*>(&c), sizeof(Color));
                in.read(reinterpret_cast<char*>(&m), sizeof(bool));

                switch (t) {
                case PieceType::Pawn:   grid[i] = std::make_unique<Pawn>(c); break;
                case PieceType::Rook:   grid[i] = std::make_unique<Rook>(c); break;
                case PieceType::Knight: grid[i] = std::make_unique<Knight>(c); break;
                case PieceType::Bishop: grid[i] = std::make_unique<Bishop>(c); break;
                case PieceType::Queen:  grid[i] = std::make_unique<Queen>(c); break;
                case PieceType::King:   grid[i] = std::make_unique<King>(c); break;
                default: break;
                }
                if (grid[i]) grid[i]->setHasMoved(m);
            }
        }
    }

    Piece* Board::getPiece(Position pos) const {
        if (!pos.isValid()) return nullptr;
        return grid[pos.toIndex()].get();
    }
    Piece* Board::getPiece(int r, int c) const { return getPiece({ r, c }); }

    bool Board::isFree(Position pos) const { return getPiece(pos) == nullptr; }
    bool Board::isFree(int r, int c) const { return isFree({ r, c }); }

    Position Board::getEnPassantTarget() const { return enPassantTarget; }

    Position Board::findKing(Color c) const {
        for (int i = 0; i < 64; ++i) {
            if (grid[i] && grid[i]->getType() == PieceType::King && grid[i]->getColor() == c) {
                return { i / 8, i % 8 };
            }
        }
        return { -1, -1 };
    }

    std::string Board::getSignature() const {
        std::stringstream ss;
        for (int i = 0; i < 64; ++i) {
            if (!grid[i]) ss << ".";
            else {
                ss << (char)grid[i]->getType() << (char)grid[i]->getColor();
            }
        }
 
        auto wk = getPiece(7, 4); auto bk = getPiece(0, 4);
        bool wkMoved = (wk && wk->getHasMoved());
        bool bkMoved = (bk && bk->getHasMoved());

        auto wr1 = getPiece(7, 0); auto wr2 = getPiece(7, 7);
        auto br1 = getPiece(0, 0); auto br2 = getPiece(0, 7);

        ss << "|";
        if (!wkMoved) {
            if (wr1 && !wr1->getHasMoved()) ss << "K";
            if (wr2 && !wr2->getHasMoved()) ss << "Q";
        }
        if (!bkMoved) {
            if (br1 && !br1->getHasMoved()) ss << "k";
            if (br2 && !br2->getHasMoved()) ss << "q";
        }

        ss << "|" << enPassantTarget.row << "," << enPassantTarget.col;

        return ss.str();
    }

    void Board::moveInternal(Position from, Position to) {
        grid[to.toIndex()] = std::move(grid[from.toIndex()]);
        if (grid[to.toIndex()]) {
            grid[to.toIndex()]->setHasMoved(true);
        }
    }

    void Board::handleCastling(Position from, Position to) {
        int row = from.row;
        if (to.col > from.col) moveInternal({ row, 7 }, { row, 5 });
        else moveInternal({ row, 0 }, { row, 3 });
    }

    void Board::handleEnPassant(Position from, Position to, const Piece* p) {
        if (p->getType() == PieceType::Pawn && to == enPassantTarget) {
            if (isFree(to)) {
                int captureRow = (p->getColor() == Color::White) ? to.row + 1 : to.row - 1;
                grid[captureRow * 8 + to.col].reset();
            }
        }
    }

    void Board::updateEnPassantStatus(Position from, Position to, const Piece* p) {
        enPassantTarget = { -1, -1 };
        if (p->getType() == PieceType::Pawn && std::abs(to.row - from.row) == 2) {
            enPassantTarget = { (from.row + to.row) / 2, from.col };
        }
    }

    void Board::movePiece(Position from, Position to) {
        Piece* p = getPiece(from);
        if (!p) return;

        bool isPawn = (p->getType() == PieceType::Pawn);
        bool isCapture = (getPiece(to) != nullptr);

        if (isPawn && to == enPassantTarget && isFree(to)) {
            isCapture = true;
        }

        if (isPawn || isCapture) {
            halfMoveClock = 0;
        }
        else {
            halfMoveClock++;
        }

        if (p->getType() == PieceType::King && std::abs(from.col - to.col) == 2) {
            handleCastling(from, to);
        }

        handleEnPassant(from, to, p);
        updateEnPassantStatus(from, to, p);
        moveInternal(from, to);
    }

    void Board::promotePiece(Position pos, PieceType type) {
        Piece* p = getPiece(pos);
        if (!p) return;
        Color c = p->getColor();
        int idx = pos.toIndex();
        switch (type) {
        case PieceType::Queen:  grid[idx] = std::make_unique<Queen>(c); break;
        case PieceType::Rook:   grid[idx] = std::make_unique<Rook>(c); break;
        case PieceType::Bishop: grid[idx] = std::make_unique<Bishop>(c); break;
        case PieceType::Knight: grid[idx] = std::make_unique<Knight>(c); break;
        default: break;
        }
    }
 
    void Board::print() const {
        std::wcout << L"\n  a b c d e f g h\n";
        std::wcout << L" +----------------+\n";
        for (int r = 0; r < 8; ++r) {
            std::wcout << (8 - r) << L"|";
            for (int c = 0; c < 8; ++c) {
                Piece* p = getPiece(r, c);
                if (p) std::wcout << p->getSymbol() << L" ";
                else std::wcout << ((r + c) % 2 == 0 ? L". " : L"  ");
            }
            std::wcout << L"|" << (8 - r) << L"\n";
        }
        std::wcout << L" +----------------+\n";
        std::wcout << L"  a b c d e f g h\n\n";
    }
}