// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "bitboards.h"
#include "kingMasks.h"
#include "knightMasks.h"
#include "sliderMaks.h"
#include "Pieces.h"

/**
 * 
 */
class CHESSDEV_API PsLegalMoveMaskGen
{
public:
	static Bitboard generatePawnDafaultMask(Pieces pieces, uint8_t side);

    static Bitboard generatePawnLongMask(Pieces pieces, uint8_t side);

    static Bitboard generatePawnLeftCapturesMask(Pieces pieces, uint8_t side, bool includeAllPossibleCaptures);

    static Bitboard generatePawnRightCapturesMask(Pieces pieces, uint8_t side, bool includeAllPossibleCaptures);

    static Bitboard generateKnightMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures);

    static Bitboard generateBishopMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures);

    static Bitboard generateRookMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures);

    static Bitboard generateQueenMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures);

    static Bitboard generateKingMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures);

    static bool inDanger(Pieces pieces, uint8_t p, uint8_t side);

private:
    static Bitboard calculateRay(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures, uint8_t direction, bool bsr);
};
