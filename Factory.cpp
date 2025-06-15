#include "Factory.h"
#include "Pawn.h"
#include "Rook.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Knight.h"


#include "Move.h"


Piece* Factory::getPiece(PieceData::PieceType type, PieceData::PieceColor color)
{
	switch (type)
	{
	case PieceType::pawn:
		return new Pawn(color);
		
	case PieceType::rook:
		return new Rook(color);

	case PieceType::bishop:
		return new Bishop(color);

	case PieceType::queen:
		return new Queen(color);

	case PieceType::king:
		return new King(color);

	case PieceType::knight:
		return new Knight(color);

	default:
		return nullptr;
	}

}

Move* Factory::getMove(MoveType type, const String& from, const String& to, const Piece* moved, const Piece* taken)
{
	switch (type)
	{
	case MoveType::normal:
		return new NormalMove(from, to ,moved, taken);

	case MoveType::queenCastle:
		return new QueenRook(from, to, moved, taken);

	case MoveType::kingCastle:
		return new KingRook(from, to, moved, taken);

	case MoveType::remi:
		return new Remi(from, to, moved, taken);

	case MoveType::enPassant:
		return new EnPassant(from, to, moved, taken);

	case MoveType::undef:
		return nullptr;

	default:
		return nullptr;
	}
}