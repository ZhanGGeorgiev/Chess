#include "Board.h"
#include "ConsolSetter.h"
#include "Logic.h"

Board::Board()
{
	kletki = new Kletka * [MAX_KLETKI];

	for (size_t i = 0; i < MAX_KLETKI; i++)
	{
		kletki[i] = new Kletka(KletkaColor((i + !((i / 8) % 2)) % 2));
	}
}

void Board::free()
{
	for (size_t i = 0; i < MAX_KLETKI; i++)
	{
		delete kletki[i];
	}
		
	delete [] kletki;
}

Board& Board::getInstance()
{
	static Board board;
	return board;
}

Board::~Board()
{
	free();
}

void Board::print()const
{
	//system("cls");

	for (size_t i = 0; i < MAX_KLETKI; i++)
	{
		
		getKletka(i)->print();
		
		if ((i + 1) % 8 == 0)
		{
			std::wcout << L'\n';
		}
	}
	ConsolSetter::resetSettings();
}

const Kletka* Board::getKletka(size_t i)const
{
	return kletki[i];
}

Kletka* Board::getKletka(size_t i)
{
	return kletki[i];
}

void Board::setKletka(size_t i, Kletka* k)
{
	delete kletki[i];
	kletki[i] = k;
}

void writeBoard(std::ofstream& ofs, const Board& board)
{
	if (!ofs.is_open())
	{
		return;
	}

	for (size_t i = 0; i < MAX_KLETKI; i++)
	{
		writeKletka(ofs, *board.getKletka(i));
	}
}

void readBoard(std::ifstream& ifs, Board& board)
{
	if (!ifs.is_open())
	{
		return;
	}

	for (size_t i = 0; i < MAX_KLETKI; ++i)
	{
		board.setKletka(i, readKletka(ifs));
	}
}

bool Board::isFree(size_t i)const
{
	if (i < 0 || i > MAX_KLETKI)
		return false;

	return getKletka(i)->isFree();
}

void Board::placePiece(const Piece* piece, size_t i)
{
	getKletka(i)->setPiece(piece);
}

void Board::placePiece(const Piece* piece, const String& str)
{
	int index = Logic::StrToIndex(str);
	getKletka(index)->setPiece(piece);
}

const Piece* Board::getPiece(size_t i)const
{
	return getKletka(i)->getPiece();
}

Piece* Board::getPiece(size_t i)
{
	return getKletka(i)->getPiece();
}

void Board::movePieces(const String& from, const String& to)
{
	Piece* piece = getPiece(from);
	
	getKletka(from)->removePiece();
	getKletka(to)->setPiece(piece);

	Logic::removeDashStatus(piece);
	Logic::madeDash(piece, from, to);

	
}

const Piece* Board::getPiece(const String & str)const
{
	int index = Logic::StrToIndex(str);
	return getPiece(index);
}

Piece* Board::getPiece(const String& str)
{
	int index = Logic::StrToIndex(str);
	return getPiece(index);
}

const Kletka* Board::getKletka(const String& str)const
{
	int index = Logic::StrToIndex(str);
	return getKletka(index);
}

Kletka* Board::getKletka(const String& str)
{
	int index = Logic::StrToIndex(str);
	return getKletka(index);
}

void Board::removePiece(int index)
{
	getKletka(index)->setPiece(nullptr);
}