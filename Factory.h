#pragma once
#include "Piece.h"

#include "Move.h"
#include "NormalMove.h"
#include "Castle.h"
#include "Remi.h"
#include "EnPassant.h"



class Factory
{
public:
	static Piece* getPiece(PieceData::PieceType type, PieceData::PieceColor color);
	static Move* getMove(MoveType type, const String& from, const String& to, const Piece* moved, const Piece* taken);
private:

};
