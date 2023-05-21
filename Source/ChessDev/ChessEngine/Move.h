// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "bitboards.h"

enum class MoveType {
    Killer,
    Capture,
    Quiet,
    LayOutCard,
    Unknown
};

/**
 * 
 */
class CHESSDEV_API ChessMove
{
public:
	ChessMove();
	~ChessMove();

	ChessMove(uint8_t from, uint8_t to, uint8_t attackerType, uint8_t attackerSide, uint8_t defenderType, uint8_t defenderSide, MoveType type, uint8_t flag = 0);

    bool operator==(ChessMove other);

    uint8_t From;
    uint8_t To;

    uint8_t AttackerType;
    uint8_t AttackerSide;

    uint8_t DefenderType;
    uint8_t DefenderSide;

    uint8_t Flag;

    MoveType type;

    struct Flag
    {
        static constexpr uint8_t Default = 0;

        static constexpr uint8_t PawnLongMove = 1;
        static constexpr uint8_t EnPassantCapture = 2;

        static constexpr uint8_t WhiteLongCastling = 3;
        static constexpr uint8_t WhiteShortCastling = 4;
        static constexpr uint8_t BlackLongCastling = 5;
        static constexpr uint8_t BlackShortCastling = 6;

        static constexpr uint8_t PromoteToKnight = 7;
        static constexpr uint8_t PromoteToBishop = 8;
        static constexpr uint8_t PromoteToRook = 9;
        static constexpr uint8_t PromoteToQueen = 10;
    };
};
