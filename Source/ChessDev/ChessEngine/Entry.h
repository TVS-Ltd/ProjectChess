// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZobristHash.h"

/**
 * 
 */
class CHESSDEV_API Entry
{
public:
	Entry();
    Entry(ZobristHash hash, int32_t depth, uint8_t bestMoveIndex);
	~Entry();

    friend bool operator<(Entry left, Entry right);

    ZobristHash Hash;
    int32_t Depth;
    uint8_t BestMoveIndex;
};
