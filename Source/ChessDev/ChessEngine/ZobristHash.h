// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <cstdint>
#include <array>
#include "bitboards.h"
#include "zobristHashConsteval.h"
#include "Pieces.h"
#include "CoreMinimal.h"

/**
 * 
 */
class CHESSDEV_API ZobristHash
{
public:
	ZobristHash();
	~ZobristHash();

	ZobristHash(Pieces pieces, bool blackMove, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling);

    friend bool operator==(ZobristHash left, ZobristHash right);

    friend bool operator<(ZobristHash left, ZobristHash right);

    void invertPiece(uint8_t square, uint8_t type, uint8_t side);

    void invertMove();

    void invertWhiteLongCastling();

    void invertWhiteShortCastling();

    void invertBlackLongCastling();

    void invertBlackShortCastling();

    uint64_t hash;
};
