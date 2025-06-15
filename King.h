#pragma once
#include "Piece.h"
#include<iostream>

class King : public Piece
{
public:
	King() = delete;
	King(PieceColor color);

	Piece* clone() const override;
};

