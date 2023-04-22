// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <future>
#include <cmath>
#include <unistd.h>
#include <chrono>
#include <algorithm>
#include "CoreMinimal.h"
#include "Position.h"
#include "MoveList.h"
#include "TranspositionTable.h"
#include "StaticEvaluator.h"
#include "OpeningBook.h"
#include "MoveSorter.h"

#pragma once

class CHESSDEV_API AI
{
public:
    AI();
    AI(const string &openingBookPath);

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
    ChessMove bestMove(const BoardPosition &position, uint8_t side, int32_t minMs, int32_t maxMs);

private:
    OpeningBook openingBook;

    static tuple<int32_t, ChessMove> BestMove(const BoardPosition &position, uint8_t side, int32_t depth, TranspositionTable &TransposTable);

    static tuple<int32_t, ChessMove> alphaBetaMin(BoardPosition position, int32_t alpha, int32_t beta, int32_t depth_left, int32_t currentDepth, TranspositionTable &TransposTable);

    static tuple<int32_t, ChessMove> alphaBetaMax(BoardPosition position, int32_t alpha, int32_t beta, int32_t depth_left, int32_t currentDepth, TranspositionTable &TransposTable);

    static int32_t alphaBetaMinOnlyCaptures(const BoardPosition &position, int32_t alpha, int32_t beta, int32_t currentDepth);

    static int32_t alphaBetaMaxOnlyCaptures(const BoardPosition &position, int32_t alpha, int32_t beta, int32_t currentDepth);

    struct Infinity
    {
        static constexpr int32_t Negative = -1e+9;
        static constexpr int32_t Positive = 1e+9;
    };
};
