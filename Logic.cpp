#include "Logic.h"
#include "Board.h"
#include "Factory.h"
#include "Kletka.h"
#include "Move.h"
#include <iostream>

#include "Pawn.h"
#include "King.h"
#include "Rook.h"

class SetOfPieces;


void safeDeletePath(Move* path)
{
	if (path != nullptr) {
		delete[] path;
	}
}


void Logic::placeSetOnBoard(const SetOfPieces& set, Board& board)
{
	for (size_t i = 0; i < set.getCount(); i++)
	{
		const Piece* cur = set[i];

		if (cur->getType() == PieceType::pawn)
		{
			placeStartPawn(cur, board);
		}

		else
		{
			placeTheOther(cur, board);
		}
		
	}
}

String indexToText(int index)
{
	char file = 'a' + (index % 8);
	char rank = '8' - (index / 8);

	char text[3];
	text[0] = file;
	text[1] = rank;
	text[2] = '\0';

	return String(text);
}


void Logic::placeStartPawn(const Piece* cur, Board& board)
{
	size_t start = cur->getColor() == PieceColor::black ? 8 : 48;

	for (size_t i = start; i < start + 8; i++)
	{
		if (board.isFree(i))
		{
			board.placePiece(cur, i);
			break;
		}
	}
}

void Logic::placeTheOther(const Piece* cur, Board& board)
{
	size_t start;
	size_t d;

	switch (cur->getType())
	{

	case PieceType::rook:
		start = 0, d = 7;
		break;

	case PieceType::knight:
		start = 1, d = 5;
		break;

	case PieceType::bishop:
		start = 2, d = 3;
		break;

	case PieceType::queen:
		start = 3;
		break;

	case PieceType::king:
		start = 4;
		break;

	default:
		break;
	};

	if (cur->getColor() == PieceColor::white)
	{
		start += 56;
	}

	for (size_t i = 0; i <  2; i++)
	{
		if (board.isFree(start))
		{
			board.placePiece(cur, start);
			break;
		}

		start += d;
	}
}

bool validPosition(const String& pos)
{
	if (pos.getSize() != 2)
		return false;

	if (pos[0] < 'a' || pos[0] > 'h')
		return false;

	if (pos[1] < '1' || pos[1] > '8')
		return false;

	return true;
}

int	Logic::StrToIndex(const String& str)
{
	if (!validPosition(str))
		return -1;

	int width = str[0] - 'a';
	int height = str[1] - '0';

	return 64 - (8 - (width)) - 8 * (height - 1);
}

bool validInedx(int index)
{
	return index >= 0 && index <= 63;
}

void setCoords(int& fromRow, int& fromCol, int& toRow, int& toCol, const int fromIndex, const int toIndex)
{
	fromRow = fromIndex / 8;
	fromCol = fromIndex % 8;

	toRow = toIndex / 8;
	toCol = toIndex % 8;
}
bool isEnPassant(const Piece* moved, const String& from, const String& to)
{
	int fromIndex = Logic::StrToIndex(from);
	int toIndex = Logic::StrToIndex(to);

	int fromRow = 0, fromCol = 0, toRow = 0, toCol = 0;
	setCoords(fromRow, fromCol, toRow, toCol, fromIndex, toIndex);

	int dRow = toRow - fromRow;   
	int dCol = std::abs(toCol - fromCol);

	if (dCol != 1)
		return false;

	if (moved->getColor() == PieceColor::white)
	{
		if (dRow != 1)
			return false;
	}

	if (moved->getColor() == PieceColor::black)
	{
		if (dRow != -1)
			return false;
	}

	return true;
}

void Logic::removeDashStatus(Piece* piece)
{
	Pawn* pawn = dynamic_cast<Pawn*>(piece);

	if (pawn)
	{
		pawn->dashed = false;
	}
}

bool pathIsClear(const Board& board, const String& from, const String& to)
{
	int indexFrom = Logic::StrToIndex(from);
	int indexTo = Logic::StrToIndex(to);

	const Piece* moved = board.getPiece(indexFrom);
	const Piece* taken = board.getPiece(indexTo);
	int* path;

	int count = 0;

	path = moved->canMoveTo(from, to, count);


	if (!path)
	{
		return false;
	}

	for (int i = 0; i < count; i++)
	{
		if (!board.isFree(path[i]))
		{
			delete [] path;
			return false;
		}

	}

	if (taken && taken->getColor() == moved->getColor())
	{
		delete [] path;
		return false;
	}

	delete [] path;
	return true;
}

Move* PawnMoveLogic(const Piece* piece, const Board& board, const String& from, const String& to)
{

	int count = 0;
	int* path = piece->canMoveTo(from, to, count);

	int indexTo = Logic::StrToIndex(to);
	int indexFrom = Logic::StrToIndex(from);

	const Piece* taken = board.getPiece(indexTo);

	
	if (path && pathIsClear(board, from, to))
	{
		Move* mv = Factory::getMove(MoveType::normal, from, to, piece, taken);
		return mv;
	}


	const Pawn* pawn = dynamic_cast<const Pawn*>(piece);

	if (pawn->canTake(from, to))
	{
		taken = board.getPiece(indexTo);

		if (taken && taken->getColor() != piece->getColor())
		{
			Move* mv = Factory::getMove(MoveType::normal, from, to, piece, taken);
			return mv;
		}

	}

	int fromRow, fromCol, toRow, toCol;

	setCoords(fromRow, fromCol, toRow, toCol, indexFrom, indexTo);

	int dRow = fromRow - toRow;
	int dCol = abs(fromCol - toCol);

	if ((piece->getColor() == PieceColor::white && dRow == 1) || (piece->getColor() == PieceColor::black && dRow == -1))
	{
		if (dCol == 1)
		{
			int takenIndex = indexFrom - dRow * 8;
			taken = board.getPiece(takenIndex);

			const Pawn* takenPawn = dynamic_cast<const Pawn*>(taken);

			if (takenPawn != nullptr && takenPawn->dashed && takenPawn->getColor() != pawn->getColor() && board.isFree(indexTo))
			{
					Move* mv = Factory::getMove(MoveType::enPassant, from, to, piece, taken);
					dynamic_cast<EnPassant*>(mv)->taken = takenIndex;

				return mv;
			}
		}
	}

	return nullptr;

}

Move* KingLogic(const Piece* piece, const Board& board, const String& from, const String& to)
{

	int indexFrom = Logic::StrToIndex(from);
	int indexTo = Logic::StrToIndex(to);

	int fromRow = 0, fromCol = 0, toRow = 0, toCol = 0;
	setCoords(fromRow, fromCol, toRow, toCol, indexFrom, indexTo);

	if (abs(fromRow - toRow) <= 1 && abs(fromCol - toCol) <= 1)
	{
		const Piece* target = board.getPiece(to);
		if (target == nullptr || target->getColor() != piece->getColor())
		{
			 return new NormalMove(from, to, piece, target);
		}
		return nullptr;
	}

	return nullptr;
}

Move* Logic::makeMove(PieceColor color, int fromIndex, int toIndex)
{
	Board& board = Board::getInstance();
	String from;
	String to;

	if (fromIndex == -1 && toIndex == -1)
	{
		std::cin >> from;

		if (from == "remi")
		{
			return new Remi();
		}

		std::cin >>  to;

		if (to == "remi")
		{
			return new Remi();

		}

		if (!validPosition(from) || !validPosition(to))
			return nullptr;

		fromIndex = StrToIndex(from);
		toIndex = StrToIndex(to);
	}
	else
	{
		
		from = indexToText(fromIndex);
		to = indexToText(toIndex);
	}

	if (!validInedx(fromIndex) || !validInedx(toIndex))
		return nullptr;

	Piece* moved = board.getPiece(fromIndex);
	Piece* captured = board.getPiece(toIndex);

	if (!moved || moved->getColor() != color)
		return nullptr;

	if (moved->getType() == PieceType::pawn)
		return PawnMoveLogic(moved, board, from, to);

	if (moved->getType() == PieceType::king)
		return KingLogic(moved, board, from, to);

	if (!pathIsClear(board, from, to))
		return nullptr;

	return Factory::getMove(MoveType::normal, from, to, moved, captured);
}
int* Logic::WPawnCan(const String& from, const String& to, int& count)
{
	
	int fromIndex = StrToIndex(from);
	int toIndex = StrToIndex(to);

	if (!validInedx(fromIndex) || !validInedx(toIndex))
	{
		return nullptr;
	}

	int* path = new int[2];

	if (fromIndex - 8 == toIndex) 
	{
		path[count++] = toIndex;
		return path;
	}

	if (fromIndex - 8 * 2 == toIndex)
	{
		path[count++] = toIndex + 8;
		path[count++] = toIndex;
		return path;
	}

	delete path;
	return nullptr;
}

int* Logic::BPawnCan(const String& from, const String& to, int& count)
{
	

	int fromIndex = StrToIndex(from);
	int toIndex = StrToIndex(to);

	if (!validInedx(fromIndex) || !validInedx(toIndex))
	{
		return nullptr;
	}
	int* path = new int[2];

	if (fromIndex + 8 == toIndex)
	{
		path[count++] = toIndex;
		return path;
	}

	if (fromIndex + 8 * 2 == toIndex)
	{
		path[count++] = toIndex - 8;
		path[count++] = toIndex;
		return path;
	}

	delete path;
	return nullptr;
}

bool Logic::WPawnTake(const String& from, const String& to)
{
	size_t fromIndex = StrToIndex(from);
	size_t toIndex = StrToIndex(to);

	int fromRow = 0, fromCol = 0, toRow = 0, toCol = 0;
	setCoords(fromRow, fromCol, toRow, toCol, fromIndex, toIndex);
	
	if (fromRow - 1 == toRow)
	{
		if (fromCol == 1 + toCol || fromCol == toCol - 1)
			return true;
	}
	return false;
}

bool Logic::BPawnTake(const String& from, const String& to)
{
	size_t fromIndex = StrToIndex(from);
	size_t toIndex = StrToIndex(to);

	int fromRow = 0, fromCol = 0, toRow = 0, toCol = 0;
	setCoords(fromRow, fromCol, toRow, toCol, fromIndex, toIndex);

	if (fromRow + 1 == toRow)
	{
		if (fromCol == 1 + toCol || fromCol == toCol - 1)
			return true;
	}
	return false;
}

int* Logic::KnightCan(const String& from, const String& to, int& count)
{
	int fromIndex = StrToIndex(from);
	int toIndex = StrToIndex(to);

	if (!validInedx(fromIndex) || !validInedx(toIndex))
	{
		return nullptr;
	}

	int* path = new int[1];

	int fromRow = 0, fromCol = 0, toRow = 0, toCol = 0;
	setCoords(fromRow, fromCol, toRow, toCol, fromIndex, toIndex);

	int dRow = std::abs(toRow - fromRow);
	int dCol = std::abs(toCol - fromCol);

	if ((dRow == 2 && dCol == 1) || (dRow == 1 && dCol == 2))
	{
		path[count++] = toIndex;
		return path;
	}

	delete path;
	return nullptr;
}

int* Logic::BishopCan(const String& from, const String& to, int& count)
{
	int* path = new int[7];

	int fromIndex = StrToIndex(from);
	int toIndex = StrToIndex(to);

	int fromRow = 0, fromCol = 0, toRow = 0, toCol = 0;
	setCoords(fromRow, fromCol, toRow, toCol, fromIndex, toIndex);

	int dRow = toRow - fromRow;
	int dCol = toCol - fromCol;

	if (std::abs(dRow) != std::abs(dCol) || dRow == 0)
	{
		delete path;
		return nullptr; 
	}
		
	int stepRow = (dRow > 0) ? 1 : -1;
	int stepCol = (dCol > 0) ? 1 : -1;

	int lastRow = fromRow + stepRow;
	int lastCol = fromCol + stepCol;

	while (lastRow != toRow && lastCol != toCol)
	{
		path[count++] = lastRow * 8 + lastCol;
		lastRow += stepRow;
		lastCol += stepCol;
	}

	return path;
}


int* Logic::RookCan(const String & from, const String & to, int& count)
{
		count = 0;
		if (!validPosition(from) || !validPosition(to))
			return nullptr;

		int fromIndex = StrToIndex(from);
		int toIndex = StrToIndex(to);

		int fromRow, fromCol, toRow, toCol;
		setCoords(fromRow, fromCol, toRow, toCol, fromIndex, toIndex);


		if (fromRow != toRow && fromCol != toCol)
			return nullptr;

		int* path = new int[32] {};

	
		if (fromRow == toRow)
		{
			int step = (toCol > fromCol) ? 1 : -1;
			for (int col = fromCol + step; col != toCol && count < 32; col += step)
			{
				path[count++] = fromRow * 8 + col;
			}
		}

	
		else if (fromCol == toCol)
		{
			int step = (toRow > fromRow) ? 1 : -1;
			for (int row = fromRow + step; row != toRow && count < 32; row += step)
			{
				path[count++] = row * 8 + fromCol;
			}
		}

		return path;
	
}

int* Logic::QueenCan(const String& from, const String& to, int& count)
{
	int* path = BishopCan(from, to, count);

	if (path == nullptr)
	{
		count = 0;
		path = RookCan(from, to, count);
	}

	return path;
}

int* Logic::KingCan(const String& from, const String& to, int& count)
{
	if (!validPosition(from) || !validPosition(to))
		return nullptr;

	int fromIndex = StrToIndex(from);
	int toIndex = StrToIndex(to);

	int fromRow = 0, fromCol = 0, toRow = 0, toCol = 0;
	setCoords(fromRow, fromCol, toRow, toCol, fromIndex, toIndex);

	int* path = new int[3];

	if (std::abs(toRow - fromRow) < 2 && std::abs(toCol - fromCol) < 2)
	{
		path[0] = toIndex;
		return path;
	}

	if (fromRow == toRow)
	{
		if (fromCol + 2 == toCol)
		{
			path[count++] = fromIndex + 1;
			path[count++] = fromIndex + 2;

			return path;
		}

		if (fromCol - 3 == toCol)
		{
			path[count++] = fromIndex - 1;
			path[count++] = fromIndex - 2;
			path[count++] = fromIndex - 3;

			return path;
		}
	}

	delete path;
	return nullptr;
}

void Logic::madeDash(Piece* piece, const String& from, const String& to)
{
	Pawn* pawn = dynamic_cast<Pawn*>(piece);
	if (pawn)
	{
		int indexFrom = Logic::StrToIndex(from);
		int indexTo = Logic::StrToIndex(to);

		int fromRow = 0, fromCol = 0, toRow = 0, toCol = 0;
		setCoords(fromRow, fromCol, toRow, toCol, indexFrom, indexTo);



		if (pawn->getColor() == PieceColor::white && fromRow == 6 && toRow == 4)
		{
			pawn->dashed = true;
	
		}

		else if (pawn->getColor() == PieceColor::black && fromRow == 1 && toRow == 3)
		{
			pawn->dashed = true;
		
		}

		else
		{
			pawn->dashed = false;
		}
	}
}

int Logic::findPiece(const Piece* piece)
{
	for (int i = 0; i < 64; i++)
	{
		if (Board::getInstance().getPiece(i) == piece)
		{
			return i;
		}
	}
	return -1;
}


bool Logic::isInChek(const SetOfPieces& enemySet, const SetOfPieces& friendSet)
{
	Piece* king = friendSet.getKing();
	int kingIndex = findPiece(king);

	if (!king || kingIndex == -1)
		return false;

	String kingStr = indexToText(kingIndex);

	for (int i = 0; i < enemySet.getCount(); i++)
	{
		const Piece* enemy = enemySet[i];
		int enemyIndex = findPiece(enemy);

		if (enemyIndex == -1)
			continue;

		String enemyStr = indexToText(enemyIndex);

		const Pawn* pawn = dynamic_cast<const Pawn*>(enemy);

		if (pawn)
		{
			if (pawn->canTake(enemyStr, kingStr))
			{
				const Piece* taken = Board::getInstance().getPiece(kingIndex);
				if (taken && taken->getColor() != enemy->getColor())
					return true;
			}
			continue;
		}

		int count = 0;
		int* path = enemy->canMoveTo(enemyStr, kingStr, count);

		if (path != nullptr)
		{
			if (pathIsClear(Board::getInstance(), enemyStr, kingStr))
			{
				delete[] path;
				return true;
			}
			delete[] path;
		}
	}

	return false;
}

int* Logic::canGetTo(const Piece* piece, int& count)
{
	
	int pieceIndex = findPiece(piece);
	if (!piece) 
	{
		return nullptr;
	}

	int* indexes = new int[64];

	for (int i = 0; i < 64; i++)
	{
		Move* temp = makeMove(piece->getColor(), pieceIndex, i);
		if (temp != nullptr)
		{
			indexes[count++] = i;
			delete temp;
		}
	}

	return indexes;
}

bool tryMoves(const SetOfPieces& set1, const SetOfPieces& set2, int IndexFrom, int* indexes, int count)
{
	for (int i = 0; i < count; i++)
	{
		Move* temp = Logic::makeMove(set1.getColor(), IndexFrom, indexes[i]);
		if (temp) {
			temp->apply();

			if (!Logic::isInChek(set2, set1))
			{
				temp->redo();
				delete temp;
				std::wcout << indexes[i] << std::endl;
				return true;
			}

			temp->redo();
			delete temp;
		}
	}
	return false; 
}

bool Logic::hasLeagalMovess(const Player& player1, const Player& player2)
{
	const SetOfPieces& set2 = *player1.getSet();
	const SetOfPieces& set1 = *player2.getSet();

	for (int i = 0; i < set1.getCount(); i++)
	{
		int count = 0;
		int* indexes = canGetTo(set1[i], count);
		int indexFrom = findPiece(set1[i]);

		if (indexes)
		{
			if (tryMoves(set1, set2, indexFrom, indexes, count))
			{
				std::wcout << indexFrom<<std::endl;
				delete[] indexes; 
				return true;
			}
			delete[] indexes;
		}
	}
	return false; 
}