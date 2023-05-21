// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include <iostream>

#include "bitboards.h"

#include "CoreMinimal.h"


/**
 * 
 */
class CHESSDEV_API Pieces
{
public:
	Pieces();
	Pieces(const std::string& shortFen);

	~Pieces();

    array<array<Bitboard, 6>, 2> pieceBitboards{};
    array<Bitboard, 2> sideBitboards{};
    array<Bitboard, 2> inversionSideBitboards{};
    Bitboard all;
    Bitboard empty;

    static constexpr uint8_t Pawn = 0;
    static constexpr uint8_t Knight = 1;
    static constexpr uint8_t Bishop = 2;
    static constexpr uint8_t Rook = 3;
    static constexpr uint8_t Queen = 4;
    static constexpr uint8_t King = 5;
    static constexpr uint8_t Joker = 6;

    static constexpr uint8_t White = 0;
    static constexpr uint8_t Black = 1;

    friend ostream &operator<<(ostream& ostream, Pieces pieces);

    friend bool operator==(Pieces left, Pieces right);

    void updateBitboards();

    static uint8_t inverse(uint8_t side);
};
