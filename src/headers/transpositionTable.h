#include <unordered_set>
#include "entry.h"

#pragma once

class TranspositionTable
{
public:
    TranspositionTable();

    void addEntry(Entry entry);

    Entry tryToFindBestMove(ZobristHash hash);

    int32_t size();

private:
    unordered_set<Entry, HashFunction> Set;
};