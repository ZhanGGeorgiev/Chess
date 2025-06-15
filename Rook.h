#pragma once
#include "Piece.h"
#include<iostream>

class Rook : public Piece
{
public:
	Rook() = delete;
	Rook(PieceColor color);
	bool hasMoved = false;
	Piece* clone() const override;

};