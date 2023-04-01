#include "entry.h"

Entry::Entry() = default;
Entry::Entry(ZobristHash hash, int32_t depth, uint8_t bestMoveIndex)
{
    this->Hash = hash;
    this->Depth = depth;
    this->BestMoveIndex = bestMoveIndex;
}

bool operator==(Entry left, Entry right)
{
    return (left.Hash == right.Hash);
}

bool operator<(Entry left, Entry right)
{
    return (left.Hash < right.Hash);
}
