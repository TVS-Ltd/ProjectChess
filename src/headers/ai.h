#include <future>
#include <cmath>
//#include <unistd.h>
#include <chrono>
#include "position.h"
#include "moveList.h"
#include "transpositionTable.h"
#include "staticEvaluator.h"
#include "openingBook.h"
#include "moveSorter.h"

#pragma once

class AI
{
public:
    AI();
    AI(const string& openingBookPath);

    /**
    * Calculate the best AI move.
    *
    * @param position Current board position.
    * @param side AI side (Black/White).
    * @param minMs,maxMs Time bounds given for best move calculation.
    *
    * @return Best move.
    *
    * @exception This function does not throw exceptions.
    */
    Move bestMove(const Position& position, uint8_t side, int32_t minMs, int32_t maxMs);

private:
    OpeningBook openingBook;

    TranspositionTable TransposTable;

    static tuple<int32_t, Move> BestMove(const Position& position, uint8_t side, int32_t depth, int32_t evaluation, TranspositionTable& TransposTable);

    static tuple<int32_t, Move> alphaBetaMin(Position position, int32_t alpha, int32_t beta, int32_t depth_left, int32_t currentDepth, TranspositionTable& TransposTable);

    static tuple<int32_t, Move> alphaBetaMax(Position position, int32_t alpha, int32_t beta, int32_t depth_left, int32_t currentDepth, TranspositionTable& TransposTable);

    static int32_t alphaBetaMinOnlyCaptures(const Position& position, int32_t alpha, int32_t beta, int32_t currentDepth);

    static int32_t alphaBetaMaxOnlyCaptures(const Position& position, int32_t alpha, int32_t beta, int32_t currentDepth);

    struct Infinity
    {
        static constexpr int32_t Negative = -1e+9;
        static constexpr int32_t Positive = 1e+9;
    };
};
