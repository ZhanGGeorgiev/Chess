#pragma once
#include "Kletka.h"


constexpr int MAX_KLETKI = 64;

class Board
{
private:
    Kletka** kletki;
    void free();

public:
    static Board& getInstance();

    Board();

    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;
    Board(Board&&) = delete;
    Board& operator=(Board&&) = delete;

    ~Board();

    const Kletka* getKletka(size_t i)const;
    Kletka* getKletka(size_t i);

    bool isFree(size_t i)const;
    void placePiece(const Piece* piece, size_t i);
    void placePiece(const Piece* piece, const String& str);

    const Piece* getPiece(size_t i)const;
    Piece* getPiece(size_t i);

    const Piece* getPiece(const String& str)const;
    Piece* getPiece(const String& str);

    const Kletka* getKletka(const String& str)const;
    Kletka* getKletka(const String& str);

    void removePiece(int i);
    void setKletka(size_t i, Kletka* k);
    
    void print()const;

    void movePieces(const String& from, const String& to);
};

void writeBoard(std::ofstream& ofs, const Board& board);
void readBoard(std::ifstream& ifs, Board& board);