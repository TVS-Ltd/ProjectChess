#include "pieces.h"
#include "bitboards.h"
#include "psLegalMoveMaskGen.h"
#include "pawnShieldMasks.h"
#include "passedPawnMasks.h"

#pragma once

class StaticEvaluator
{
public:
    static int32_t evaluate(Pieces pieces, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling, bool whiteCastlingHappened, bool blackCastlingHappened, bool showDebugInfo = false);

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

    struct Material
    {
        static constexpr int32_t Pawn = 100;
        static constexpr int32_t Knight = 305;
        static constexpr int32_t Bishop = 333;
        static constexpr int32_t Rook = 563;
        static constexpr int32_t Queen = 950;
    };

    struct Mobility
    {
        static constexpr int32_t Knight = 9;
        static constexpr int32_t Bishop = 4;
        static constexpr int32_t Rook = 3;
        static constexpr int32_t Queen = 3;
    };

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