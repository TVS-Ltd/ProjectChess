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

void MoveList::unite(MoveList other)
{
    uint8_t otherSize = min((uint8_t)5, other.size());
    for (uint8_t i = 0; i < this->Size; i++) {
        this->moves[i + otherSize] = this->moves[i];
    }
    for (uint8_t i = 0; i < otherSize; i++) {
        this->moves[i] = other[i];
    }
    this->Size += otherSize;
    
    

    /*for (int i = 0; i < this->Size; i++)
        std::cout << i << " : " << (int)this->moves[i].From << ' ' << (int)this->moves[i].To << " \n";*/
}
//
//void MoveList::clear() {
//    this->Size = 0;
//}