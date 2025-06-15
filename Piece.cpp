#include "Piece.h"
#include "Factory.h"

void writePiece(std::ofstream& ofs, const Piece& piece)
{
	if (!ofs.is_open())
	{
		return;
	}
	PieceData::PieceColor color = piece.getColor();
	PieceData::PieceType type = piece.getType();

	ofs.write((const char*)&color, sizeof(PieceData::PieceColor));
	ofs.write((const char*)&type, sizeof(PieceData::PieceType));
}

Piece* readPiece(std::ifstream& ifs)
{
	if (!ifs.is_open())
	{
		return nullptr;
	}

	PieceData::PieceColor color;
	PieceData::PieceType type;

	ifs.read((char*)&color, sizeof(PieceData::PieceColor));
	ifs.read((char*)&type, sizeof(PieceData::PieceType));

	return Factory::getPiece(type, color);
}

const PieceColor Piece::getColor()const
{
	return color;
}

const PieceType Piece::getType()const
{
	return type;
}

Piece::Piece(PieceType type)
{
	this->type = type;
}

void Piece::print()const
{
	std::wcout << symbol;
}

void Piece::setSymbol(const wchar_t s)
{
	symbol = s;
}

void Piece::setColor(PieceColor color)
{
	this->color = color;
}

const wchar_t& Piece::getSymbol()const
{
	return symbol;
}

const wchar_t& Piece::getSymbol()
{
	return symbol;
}

