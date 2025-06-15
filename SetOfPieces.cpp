#include "SetOfPieces.h"
#include "Logic.h"
#include "Board.h"

void SetOfPieces::free()
{
	for (size_t i = 0; i < count; i++)
	{
		delete pieces[i];
	}

	delete[] pieces;
}

SetOfPieces::SetOfPieces(PieceColor color)
{
	this->color = color;
	cap = 16;
	count = 0;
	pieces = new Piece * [cap];
	generateSet();
}

SetOfPieces::~SetOfPieces()
{
	free();
}

void SetOfPieces::copy(const SetOfPieces& other)
{
	free();

	pieces = new Piece * [other.cap];

	for (int i = 0; i < other.count; i++)
	{
		pieces[i] = other.pieces[i]->clone();
	}
		
	count = other.count;
	cap = other.cap;
}

void SetOfPieces::addPiece(PieceType type)
{
	pieces[count++] = Factory::getPiece(type, color);
}

Piece* SetOfPieces::operator[](size_t i)
{
	return pieces[i];
}

const Piece* SetOfPieces::operator[](size_t i) const
{
	return pieces[i];
}

void SetOfPieces::remove(size_t i)
{
	if (count == 0 || i >= count)
		return; 

	delete pieces[i];              
	pieces[i] = pieces[count - 1]; 
	pieces[count - 1] = nullptr;
	count--;
}

SetOfPieces::SetOfPieces(const SetOfPieces& other)
{
	copy(other);
}

SetOfPieces& SetOfPieces::operator=(const SetOfPieces& other)
{
	if (this != &other)
	{
		free();
		copy(other);
	}

	return *this;
}

void SetOfPieces::generateSet()
{	
	addPiece(PieceType::king);
	addPiece(PieceType::queen);

	for (size_t i = 0; i < 8; i++)
	{
		addPiece(PieceType::pawn);
	}

	for (size_t i = 0; i < 2; i++)
	{
		addPiece(PieceType::rook);
		addPiece(PieceType::knight);
		addPiece(PieceType::bishop);
	}
}

Piece* SetOfPieces::getKing()const
{
	if (count == 0 || pieces[0] == nullptr)
		return nullptr;

	return pieces[0]; //king is always at this pos;
}

void SetOfPieces::placeOn(Board& board)
{
	Logic::placeSetOnBoard(*this, board);
}

int SetOfPieces::getCount() const
{
	return count;
}

PieceColor SetOfPieces::getColor()const
{
	return color;
}

void SetOfPieces::addPiece(Piece* piece)
{
	pieces[count++] = piece;
}

void SetOfPieces::readSet(Board& board)
{
	for (size_t i = 0; i < MAX_KLETKI; i++)
	{
		if (count == cap)
		{
			break;
		}

		Piece* p = board.getKletka(i)->getPiece();
		
		if (p != nullptr && p->getColor() == color)
		{
			addPiece(p);

			if (p->getType() == PieceType::king && count > 1)
			{
				Piece* temp = pieces[0];
				pieces[0] = pieces[count - 1];
				pieces[count - 1] = temp;
			}

		}
	}

}