#include "repetitionHistory.h"

using namespace std;

RepetitionHistory::RepetitionHistory() = default;

void RepetitionHistory::addPosition(ZobristHash hash)
{
    this->hashes.push_back(hash);
}

void RepetitionHistory::clear()
{
    this->hashes.clear();
}

uint8_t RepetitionHistory::getRepetionNumber(ZobristHash hash)
{
    uint8_t ctr = 0;

    for (auto hash1 : this->hashes)
    {
        if (hash == hash1)
        {
            ctr++;
        }
    }

    return ctr;
}