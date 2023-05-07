#include "moveList.h"
#include <iostream>

MoveList::MoveList()
{
    this->Size = 0;
}

Move& MoveList::operator[](uint8_t index)
{
    return this->moves[index];
}

Move MoveList::operator[](uint8_t index) const
{
    return this->moves[index];
}


void MoveList::push_back(Move move)
{
    this->moves[this->Size] = move;
    this->Size = this->Size + 1;
}

[[nodiscard]] uint8_t MoveList::size() const
{
    return this->Size;
}