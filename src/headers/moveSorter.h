#include "moveList.h"
#include "pieces.h"
#include "staticEvaluator.h"

#pragma once

class MoveSorter
{
public:
    static MoveList quickSort(const Pieces& pieces, MoveList& moves, int start, int end);

private:
    static int32_t evaluateMove(const Pieces& pieces, Move& move);

    static size_t partition(const Pieces& pieces, MoveList& moves, int start, int end);
};
