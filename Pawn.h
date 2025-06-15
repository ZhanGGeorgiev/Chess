#pragma once
#include "Piece.h"
#include<iostream>



class Pawn: public Piece
{
public:
	Pawn() = delete;
	Pawn(PieceColor color);
	bool (*canTake)(const String&, const String&) = nullptr;
	Piece* clone() const override;
	bool dashed = false;
};

