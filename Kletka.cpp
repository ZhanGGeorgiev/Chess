#include "Kletka.h"
#include "ConsolSetter.h"

Kletka::Kletka(KletkaColor color)
{
	this->color = color;
	piece = nullptr;
}

void writeKletka(std::ofstream& ofs, const Kletka& kletka)
{
	if (!ofs.is_open())
	{
		return;
	}

	ofs.write((const char*)&kletka.getColor(), sizeof(KletkaColor));
	bool isFree = kletka.isFree();

	ofs.write((const char*)&isFree, sizeof(bool));

	
	if (!isFree)
	{
		writePiece(ofs, *kletka.getPiece());
	}
}

Kletka* readKletka(std::ifstream& ifs)
{

	if (!ifs.is_open())
	{
		return nullptr;
	}

	KletkaColor color;
	bool isFree;

	ifs.read((char*)&color, sizeof(KletkaColor));

	Kletka* kletka = new Kletka(color);

	ifs.read((char*)&isFree, sizeof(bool));

	if (!isFree)
	{
		kletka->setPiece(readPiece(ifs));
	}

	return kletka;
}

bool Kletka::isFree()const
{
	return piece == nullptr;
}

void Kletka::free()
{
	delete piece;
}

Kletka::~Kletka()
{
	free();
}

const Piece* Kletka::getPiece()const
{
	return piece;
}

Piece* Kletka::getPiece()
{
	return piece;
}

const KletkaColor& Kletka::getColor()const
{
	return color;
}

KletkaColor& Kletka::getColor()
{
	return color;
}

void Kletka::print()const
{
	ConsolSetter::SetConsole(*this);
	std::wcout << getSymbol() <<" ";
}

const wchar_t Kletka::getSymbol()const
{
	if (!isFree())
	{
		const Piece* p = getPiece();
		return p->getSymbol();
	}
	
	return L' ';
}

const wchar_t Kletka::getSymbol()
{
	if (!isFree())
	{
		Piece* p = getPiece();
		return p->getSymbol();
	}

	return L' ';
}

const PieceColor Kletka::getPieceColor()const
{
	const Piece* p = getPiece();

	if (p != nullptr)
	{
		return p->getColor();
	}

	return PieceColor::undef;
}

const PieceColor Kletka::getPieceColor()
{
	Piece* p = getPiece();

	if (p != nullptr)
	{
		return p->getColor();
	}

	return PieceColor::undef;
}

void Kletka::setPiece(const Piece* piece)
{
	free(); 
	this->piece = const_cast<Piece*>(piece);
}

void Kletka::removePiece()
{
	piece = nullptr;
}