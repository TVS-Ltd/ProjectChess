#include "zobristHash.h"
#include "move.h"

class Entry
{
public:
    Entry();
    Entry(ZobristHash hash, Move move, int32_t depth, int32_t score, int32_t flag);

    friend bool operator==(Entry left, Entry right);
    friend bool operator<(Entry left, Entry right);

    ZobristHash Hash;
    Move BestMove;
    int32_t Depth, Score, Flag;

    static constexpr uint8_t LBound = 0;
    static constexpr uint8_t UBound = 1;
    static constexpr uint8_t Valid = 2;
};

/// Zobrist hash returned as hash function.

class HashFunction {
public:
    size_t operator()(const Entry& t) const
    {
        return t.Hash.hash;
    }
};