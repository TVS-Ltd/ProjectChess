#include <vector>
#include "zobristHash.h"

#pragma once

class RepetitionHistory
{
public:
    RepetitionHistory();

    void addPosition(ZobristHash hash);

    void clear();

    uint8_t getRepetionNumber(ZobristHash hash);

private:
    vector<ZobristHash> hashes;
};