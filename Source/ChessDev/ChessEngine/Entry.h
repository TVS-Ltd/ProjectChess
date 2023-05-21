// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ZobristHash.h"
#include "Move.h"

/**
 * 
 */
class CHESSDEV_API Entry
{
public:
	Entry();
    Entry(ZobristHash hash, ChessMove move, int32_t depth, int32_t score, int32_t flag);
	~Entry();

    friend bool operator==(Entry left, Entry right);
    friend bool operator<(Entry left, Entry right);

    ZobristHash Hash;
    ChessMove BestMove;
    int32_t Depth, Score, Flag;

    static constexpr uint8_t LBound = 0;
    static constexpr uint8_t UBound = 1;
    static constexpr uint8_t Valid = 2;
};

class CHESSDEV_API HashFunction {
public:
    size_t operator()(const Entry& t) const
    {
        return t.Hash.hash;
    }
};