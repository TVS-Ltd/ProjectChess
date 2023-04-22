// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <set>
#include "Entry.h"
#include "CoreMinimal.h"

/**
 * 
 */
class CHESSDEV_API TranspositionTable
{
public:
	TranspositionTable();
	~TranspositionTable();

    void addEntry(Entry entry);

    uint8_t tryToFindBestMoveIndex(ZobristHash hash);

private:
    set<Entry> Set;
};
