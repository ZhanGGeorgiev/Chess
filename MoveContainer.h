#pragma once
#include "Piece.h"
#include <fstream>
#include "Move.h"
#include "Player.h"



class MoveContainer 
{
   Move** moves;
   void free();

   size_t count;
   size_t cap;

   void resize();

   void addMove(const Move& move);
   void addMove(MoveType type);
   void addMove(Move&& move) noexcept;

public:

    void makeMove( Player& player1, Player& player2);

    MoveContainer();

    ~MoveContainer();
};