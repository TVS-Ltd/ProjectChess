// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "ZobristHash.h"
#include "CoreMinimal.h"

/**
 * 
 */
class CHESSDEV_API RepetitionHistory
{
public:
	RepetitionHistory();
	~RepetitionHistory();

    void addPosition(ZobristHash hash);

    void clear();

    uint8_t getRepetionNumber(ZobristHash hash) const;

private:
    vector<ZobristHash> hashes;
};
