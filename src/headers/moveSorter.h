#include "moveList.h"
#include "pieces.h"
#include "staticEvaluator.h"

#pragma once

class MoveSorter
{
public:
    static MoveList quickSort(Pieces pieces, MoveList moves, int start, int end);

private:
    static int32_t evaluateMove(Pieces pieces, Move move);

    static size_t partition(Pieces pieces, MoveList moves, int start, int end);
};