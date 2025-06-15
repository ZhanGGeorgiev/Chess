#pragma once
#include "Piece.h"


enum class KletkaColor
{
	white,
	black
};

class Kletka
{
private:
	Piece* piece;
	KletkaColor color;

	void free();

	void copy(const Kletka& other);
	void copy(const Kletka&& other);

	Kletka& operator=(const Kletka& other);
	Kletka& operator=(const Kletka&& other);

public:
	Kletka(KletkaColor color);

	~Kletka();	

	bool isFree()const;
	const Piece* getPiece()const;
	Piece* getPiece();
	
	const KletkaColor& getColor()const;
	KletkaColor& getColor();

	void print()const;

	const wchar_t getSymbol()const;
	const wchar_t getSymbol();

	const PieceColor getPieceColor()const;
	const PieceColor getPieceColor();

	void setPiece(const Piece* piece);
	void removePiece();
};


void writeKletka(std::ofstream& ofs, const Kletka& kletka);
Kletka* readKletka(std::ifstream& ifs);