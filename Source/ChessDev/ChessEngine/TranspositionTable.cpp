// Fill out your copyright notice in the Description page of Project Settings.


#include "TranspositionTable.h"

TranspositionTable::TranspositionTable() = default;

TranspositionTable::~TranspositionTable() {}

void TranspositionTable::addEntry(Entry entry)
{
#pragma omp critical
    {
        auto hashCopy = this->Set.find(entry);

        if (hashCopy == this->Set.end())
        {
            this->Set.insert(entry);
        }
        else
            if (hashCopy->Depth < entry.Depth)
            {
                this->Set.erase(hashCopy);
                this->Set.insert(entry);
            }
    }
}

Entry TranspositionTable::tryToFindBestMove(ZobristHash hash)
{
    std::unordered_set<Entry, HashFunction>::iterator entry;
#pragma omp critical
    {
        entry = this->Set.find({ hash, ChessMove(), 0, 0, 0 });
    }
    return ((entry == this->Set.end()) ? Entry() : *entry);
}

int32_t TranspositionTable::size()
{
    return Set.size();
}


