#pragma once
#include "Piece.h"
#include<iostream>

class Queen : public Piece
{
public:
	Queen() = delete;
	Queen(PieceColor color);

	Piece* clone() const override;
};

