// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pieces.h"
#include "bitboards.h"
#include "PsLegalMoveMaskGen.h"
#include "pawnShieldMasks.h"
#include "passedPawnMasks.h"

/**
 * 
 */
class CHESSDEV_API StaticEvaluator
{
public:
    static int32_t evaluate(Pieces pieces, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling, bool whiteCastlingHappened, bool blackCastlingHappened, bool showDebugInfo = false);
    static int32_t pieceMaterial(Pieces pieces, uint8_t side);

    static constexpr int32_t Materials[6] = { 100, 305, 333, 563, 950, 1300 };

    struct Material
    {
        static constexpr int32_t Pawn = 100;
        static constexpr int32_t Knight = 305;
        static constexpr int32_t Bishop = 333;
        static constexpr int32_t Rook = 563;
        static constexpr int32_t Queen = 950;
        static constexpr int32_t EndgameMat = 1300;
    };

    struct Mobility
    {
        static constexpr int32_t Knight = 9;
        static constexpr int32_t Bishop = 4;
        static constexpr int32_t Rook = 3;
        static constexpr int32_t Queen = 3;
    };

private:
    static int32_t material(Pieces pieces);
    static int32_t mobility(Pieces pieces);

    static int32_t pawnStructureDoublePawn(Pieces pieces);
    static int32_t pawnStructureConnectedPawn(Pieces pieces);
    static int32_t pawnStructurePawnPromotion(Pieces pieces);
    
    static int32_t kingSafetyCrashedCastling(bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling, bool whiteCastlingHappened, bool blackCastlingHappened);
    static int32_t kingSafetyPawnShield(Pieces pieces, bool whiteCastlingHappened, bool blackCastlingHappened);

    static int32_t twoBishops(Pieces pieces);
    static int32_t endgame(Pieces pieces, bool whiteLeading);


    struct PawnStructure
    {
        static constexpr int32_t DoublePawn = -25;
        static constexpr int32_t ConnectedPawn = 12;
        static constexpr std::array<int32_t, 8> DefaultPawnPromotion = {0, 0, 0, 0, 10, 20, 30, 0};
        static constexpr std::array<int32_t, 8> PassedPawnPromotion = {0, 50, 50, 50, 70, 90, 110, 0};
    };

    struct KingSafety
    {
        static constexpr int32_t CrashedCastling = -50;
        static constexpr int32_t PawnShield = 33;
    };

    static constexpr int32_t TwoBishops = 50;

    struct Endgame
    {
        static constexpr int32_t MaximumPiecesForEndgame = 8;
        static constexpr int32_t AttackerKingProximityToDefenderKing = 10;
        static constexpr int32_t DistanceBetweenDefenderKingAndMiddle = 10;
    };

    friend class MoveSorter;
};
