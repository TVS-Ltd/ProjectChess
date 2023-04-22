// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveList.h"

MoveList::MoveList()
{
    this->Size = 0;
}

class ChessMove &MoveList::operator[](uint8_t index)
{
    return this->moves[index];
}

class ChessMove MoveList::operator[](uint8_t index) const
{
    return this->moves[index];
}

void MoveList::push_back(class ChessMove move)
{
    this->moves[this->Size] = move;
    this->Size = this->Size + 1;
}

[[nodiscard]] uint8_t MoveList::size() const
{
    return this->Size;
}

MoveList::~MoveList()
{
}
