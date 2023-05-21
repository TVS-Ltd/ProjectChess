// Fill out your copyright notice in the Description page of Project Settings.


#include "Entry.h"

Entry::Entry() : Depth(-1) {}

Entry::Entry(ZobristHash hash, ChessMove move, int32_t depth, int32_t score, int32_t flag) : Hash(hash), BestMove(move), Depth(depth), Score(score), Flag(flag) {}

bool operator==(Entry left, Entry right)
{
    return (left.Hash == right.Hash);
}

bool operator<(Entry left, Entry right)
{
    return (left.Hash < right.Hash);
}
Entry::~Entry()
{
}
