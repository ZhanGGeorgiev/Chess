#include "ChessGame.h"
#include "MoveValidator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cwctype> 

namespace Chess {

    ChessGame::ChessGame() : currentTurn(Color::White), gameOver(false) {
        updateHistory(); 
    }

    Position ChessGame::parseInput(const std::wstring& s) const {
        if (s.length() < 2) return { -1, -1 };
        int c = towlower(s[0]) - L'a';
        int r = 8 - (s[1] - L'0');
        return { r, c };
    }

    void ChessGame::switchTurn() {
        currentTurn = oppositeColor(currentTurn);
    }

    void ChessGame::updateHistory() {
        std::string sig = board.getSignature();
        sig += (currentTurn == Color::White ? "W" : "B");
        positionHistory[sig]++;
    }

    void ChessGame::saveGame(const std::wstring& filename) {
        std::string fn(filename.begin(), filename.end());
        std::ofstream out(fn, std::ios::binary);
        if (out.is_open()) {
            out.write(reinterpret_cast<const char*>(&currentTurn), sizeof(Color));
            board.save(out);
            std::wcout << L"Game saved (binary) to " << filename << L"\n";
        }
        else {
            std::wcout << L"Error: Could not save file.\n";
        }
    }

    void ChessGame::loadGame(const std::wstring& filename) {
        std::string fn(filename.begin(), filename.end());
        std::ifstream in(fn, std::ios::binary);
        if (in.is_open()) {
            in.read(reinterpret_cast<char*>(&currentTurn), sizeof(Color));
            board.load(in);

            positionHistory.clear();
            updateHistory();

            std::wcout << L"Game loaded (binary) from " << filename << L"\n";
        }
        else {
            std::wcout << L"Error: Could not load file.\n";
        }
    }

    void ChessGame::handlePromotion(Position to) {
        Piece* p = board.getPiece(to);
        if (!p || p->getType() != PieceType::Pawn) return;

        bool whitePromote = (p->getColor() == Color::White && to.row == 0);
        bool blackPromote = (p->getColor() == Color::Black && to.row == 7);

        if (whitePromote || blackPromote) {
            std::wcout << L"Promotion! (Q/R/B/N): ";
            wchar_t promo;
            while (true) {
                std::wcin >> promo;
                promo = towlower(promo);
                if (promo == L'q') { board.promotePiece(to, PieceType::Queen); break; }
                if (promo == L'r') { board.promotePiece(to, PieceType::Rook); break; }
                if (promo == L'b') { board.promotePiece(to, PieceType::Bishop); break; }
                if (promo == L'n') { board.promotePiece(to, PieceType::Knight); break; }
            }
        }
    }

    void ChessGame::run() {
        std::wcout << L"Welcome to Console Chess.\n"
            << L"Commands: 'e2 e4', 'save <file>', 'load <file>', 'exit'\n\n";

        while (!gameOver) {
            board.print();

            GameState state = MoveValidator::getGameState(board, currentTurn);

            std::string sig = board.getSignature() + (currentTurn == Color::White ? "W" : "B");
            if (positionHistory[sig] >= 3) {
                state = GameState::Draw;
                std::wcout << L"Draw by Threefold Repetition!\n";
            }

            if (state == GameState::Checkmate) {
                std::wcout << (currentTurn == Color::White ? L"Black" : L"White") << L" WINS by Checkmate!\n";
                break;
            }
            if (state == GameState::Stalemate) {
                std::wcout << L"Draw by Stalemate!\n";
                break;
            }
            if (state == GameState::Draw) {
                if (positionHistory[sig] < 3) {
                    if (board.getHalfMoveClock() >= 100) std::wcout << L"Draw by 50-Move Rule!\n";
                    else std::wcout << L"Draw by Insufficient Material!\n";
                }
                break;
            }

            if (MoveValidator::isKingInCheck(board, currentTurn)) {
                std::wcout << L"!! CHECK !!\n";
            }

            std::wcout << (currentTurn == Color::White ? L"[White]" : L"[Black]") << L" Move: ";

            std::wstring line;
            if (std::wcin.peek() == L'\n') std::wcin.ignore();
            std::getline(std::wcin, line);

            if (line == L"exit" || line == L"quit") break;

            std::wstringstream ss(line);
            std::wstring command;
            ss >> command;

            if (command == L"save") {
                std::wstring filename;
                ss >> filename;
                if (!filename.empty()) saveGame(filename);
                continue;
            }
            if (command == L"load") {
                std::wstring filename;
                ss >> filename;
                if (!filename.empty()) loadGame(filename);
                continue;
            }

            std::wstring sFrom, sTo;
            if (command.length() == 4) {
                sFrom = command.substr(0, 2);
                sTo = command.substr(2, 2);
            }
            else {
                sFrom = command;
                ss >> sTo;
            }

            Position from = parseInput(sFrom);
            Position to = parseInput(sTo);

            if (!from.isValid() || !to.isValid()) {
                std::wcout << L"Invalid input.\n";
                continue;
            }

            Piece* p = board.getPiece(from);
            if (!p || p->getColor() != currentTurn) {
                std::wcout << L"Not your piece.\n";
                continue;
            }

            if (MoveValidator::isMoveValid(board, from, to)) {

                Piece* target = board.getPiece(to);
                bool isIrreversible = (p->getType() == PieceType::Pawn || target != nullptr);

                board.movePiece(from, to);
                handlePromotion(to);
                switchTurn();

                if (isIrreversible) {
                    positionHistory.clear();
                }

                updateHistory();
            }
            else {
                std::wcout << L"Illegal move.\n";
            }
        }
    }
}