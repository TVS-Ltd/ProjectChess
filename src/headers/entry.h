#include "zobristHash.h"

class Entry
{
public:
    Entry();
    Entry(ZobristHash hash, int32_t depth, uint8_t bestMoveIndex);

    friend bool operator==(Entry left, Entry right);
    friend bool operator<(Entry left, Entry right);

    ZobristHash Hash;
    int32_t Depth;
    uint8_t BestMoveIndex;
};

/// Zobrist hash returned as hash function.

class HashFunction {
public:
    size_t operator()(const Entry& t) const
    {
        return t.Hash.hash;
    }
};
