#pragma once
#include "Piece.h"
#include<iostream>

class Knight : public Piece
{
public:
	Knight() = delete;
	Knight(PieceColor color);

	Piece* clone() const override;

};

