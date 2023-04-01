#include <unordered_set>
#include "entry.h"

#pragma once

class TranspositionTable
{
public:
    TranspositionTable();

    void addEntry(Entry entry);

    uint8_t tryToFindBestMoveIndex(ZobristHash hash);

private:
    unordered_set<Entry, HashFunction> Set;
};
