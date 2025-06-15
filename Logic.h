#pragma once
#include "Move.h"
#include "Board.h"
#include "String.h"

#include "SetOfPieces.h"
#include "Player.h"

class Logic
{
	
public:
	static int* WPawnCan(const String& from, const String& to, int& count);
	static int* BPawnCan(const String& from, const String& to, int& count);

	static int* BishopCan(const String& from, const String& to, int& count);
	static int* RookCan(const String& from, const String& to, int& count);
	static int* KnightCan(const String& from, const String& to, int& count);
	static int* QueenCan(const String& from, const String& to, int& count);
	static int* KingCan(const String& from, const String& to, int& count);

	static void executeMove(const Move& mv);

	static bool WPawnTake(const String& from, const String& to);
	static bool BPawnTake(const String& from, const String& to);
	static int StrToIndex(const String& str);
	static void placeSetOnBoard(const SetOfPieces& other, Board& board);

	static Move* makeMove(PieceColor color, int fromIndex = -1, int toIndex = -1);

	static void madeDash(Piece* piece, const String& from, const String& to);
	static void removeDashStatus(Piece* piece);

	static bool hasLeagalMovess(const Player& player1, const Player& player2);


	static int* canGetTo(const Piece* piece, int& count);

	static int findPiece(const Piece* piece);
	static bool isInChek(const SetOfPieces& enemySet, const SetOfPieces& friendSet);

private:

	static void placeStartPawn(const Piece* cur, Board& board);
	static void placeTheOther(const Piece* cur, Board& board);
};

