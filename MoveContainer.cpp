#include "MoveContainer.h"
#include "Factory.h"
#include "Logic.h"

void MoveContainer::free()
{
	for (size_t i = 0; i < count; i++)
	{
		delete moves[i];
	}
	delete[] moves;
}

MoveContainer::~MoveContainer()
{
	free();
}

MoveContainer::MoveContainer()
{
	cap = 32;
	count = 0;
    moves = new Move * [cap] {};
}

void MoveContainer::resize()
{
	cap *= 2;

    Move** newMoves = new Move * [cap] {};

	for (size_t i = 0; i < count; i++)
	{
		newMoves[i] = moves[i]->clone();
	}

	free();

	moves = newMoves;
}

void MoveContainer::makeMove(Player& player1, Player& player2)
{
    if (cap == count)
        resize();

    Move* mv = nullptr;

    while (true)
    {
        mv = Logic::makeMove(player1.getColor());

        if (mv == nullptr)
            continue;

        mv->apply();

        if (!Logic::isInChek(*player2.getSet(), *player1.getSet()))
        {
            moves[count++] = mv;

            mv = nullptr;
            break;
        }
        std::wcout << "chek"<<std::endl;
        mv->redo();
        delete mv;
        mv = nullptr;
    }
}