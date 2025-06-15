#include "Remi.h"
#include "Game.h"

Move* Remi::clone() const 
{
    return new Remi(*this);
}

void Remi::apply() 
{
    Game::getInstance().setStatus(gameStatus::remiOffer);
}


void Remi::redo()
{

}