// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <unordered_set>

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

    Entry tryToFindBestMove(ZobristHash hash);

    int32_t size();

private:
    std::unordered_set<Entry, HashFunction> Set;

};
