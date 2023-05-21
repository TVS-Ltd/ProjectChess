// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveList.h"

MoveList::MoveList()
{
    this->Size = 0;
}

MoveList::~MoveList() {}

ChessMove& MoveList::operator[](uint8_t index)
{
    return this->moves[index];
}

ChessMove MoveList::operator[](uint8_t index) const
{
    return this->moves[index];
}

void MoveList::push_back(ChessMove move)
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
    uint8_t otherSize = min((uint8_t)8, other.size());

    for (uint8_t i = 0; i < this->Size; i++) 
    {
        this->moves[i + otherSize] = this->moves[i];
    }

    for (uint8_t i = 0; i < otherSize; i++) 
    {
        this->moves[i] = other[i];
    }

    this->Size += otherSize;
}


