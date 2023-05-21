// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveList.h"
#include "Pieces.h"
#include "StaticEvaluator.h"

/**
 * 
 */
class CHESSDEV_API MoveSorter
{
public:
    static void quickSort(const Pieces& pieces, MoveList& moves, int start, int end);

private:
    static int32_t evaluateMove(const Pieces& pieces, ChessMove& move);

    static size_t partition(const Pieces& pieces, MoveList& moves, int start, int end);
};
