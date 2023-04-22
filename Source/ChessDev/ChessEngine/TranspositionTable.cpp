// Fill out your copyright notice in the Description page of Project Settings.


#include "TranspositionTable.h"

TranspositionTable::TranspositionTable() = default;

void TranspositionTable::addEntry(Entry entry)
{
    auto hashCopy = this->Set.find(entry);

    if (hashCopy == this->Set.end() || hashCopy->Depth < entry.Depth)
    {
        this->Set.insert(entry);
    }
}

uint8_t TranspositionTable::tryToFindBestMoveIndex(ZobristHash hash)
{
    auto entry = this->Set.find({hash, 0, 0});

    if (entry == this->Set.end())
    {
        return 255;
    }

    return entry->BestMoveIndex;
}

TranspositionTable::~TranspositionTable()
{
}
