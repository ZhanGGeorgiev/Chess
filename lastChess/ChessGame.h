#pragma once
#include "Board.h"
#include <string>
#include <map> 

namespace Chess {
    class ChessGame {
    private:
        Board board;
        Color currentTurn;
        bool gameOver;

        std::map<std::string, int> positionHistory;

        Position parseInput(const std::wstring& s) const;
        void handlePromotion(Position to);
        void switchTurn();
        void updateHistory();

        void saveGame(const std::wstring& filename);
        void loadGame(const std::wstring& filename);

    public:
        ChessGame();
        void run();
    };
}