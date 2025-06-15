#pragma once
#include "Piece.h"
#include "Factory.h"
#include "Board.h"

class SetOfPieces
{
private:
	Piece** pieces;

	PieceColor color;
	int count;
	int cap;

	void free();
	void copy(const SetOfPieces& other);
	void addPiece(PieceType type);
	void addPiece(Piece* piece);


	void remove(size_t i);



	void generateSet();

public:
	SetOfPieces() = delete;
	SetOfPieces(PieceColor color);
	SetOfPieces(const SetOfPieces& other);
	SetOfPieces& operator=(const SetOfPieces& other);
	~SetOfPieces();

	SetOfPieces(SetOfPieces&& other) = delete;
	SetOfPieces& operator=(SetOfPieces&& other) = delete;

	Piece* getKing()const;
	int getCount() const;
	PieceColor getColor()const;
	void placeOn(Board& board);

	Piece* operator[](size_t i);
	const Piece* operator[](size_t i) const;

	void readSet(Board& board);
};

