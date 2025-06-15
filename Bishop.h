#pragma once
#include "Piece.h"
#include<iostream>

class Bishop : public Piece
{
public:
	Bishop() = delete;
	Bishop(PieceColor color);

	Piece* clone() const override;

};

