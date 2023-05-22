// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticEvaluator.h"


int32_t StaticEvaluator::evaluate(Pieces pieces, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling, bool whiteCastlingHappened, bool blackCastlingHappened, bool showDebugInfo)
{
    int32_t evaluation = 0;

    int32_t material = StaticEvaluator::material(pieces);
    int32_t mobility = StaticEvaluator::mobility(pieces);
    int32_t doublePawn = StaticEvaluator::pawnStructureDoublePawn(pieces);
    int32_t connectedPawn = StaticEvaluator::pawnStructureConnectedPawn(pieces);
    int32_t pawnPromotion = StaticEvaluator::pawnStructurePawnPromotion(pieces);
    int32_t crashedCastling = StaticEvaluator::kingSafetyCrashedCastling(whiteLongCastling, whiteShortCastling, blackLongCastling, blackShortCastling, whiteCastlingHappened, blackCastlingHappened);
    int32_t pawnShield = StaticEvaluator::kingSafetyPawnShield(pieces, whiteCastlingHappened, blackCastlingHappened);
    int32_t twoBishops = StaticEvaluator::twoBishops(pieces);
    int32_t endgame = StaticEvaluator::endgame(pieces, material >= 0);

    evaluation = evaluation + material;
    evaluation = evaluation + mobility;
    evaluation = evaluation + doublePawn;
    evaluation = evaluation + connectedPawn;
    evaluation = evaluation + pawnPromotion;
    evaluation = evaluation + crashedCastling;
    evaluation = evaluation + pawnShield;
    evaluation = evaluation + twoBishops;
    evaluation = evaluation + endgame;

    if (showDebugInfo)
    {
        // cout << "Details of static evaluation of current position." << endl;
        // cout << "Material: " << (float)material / 100.0f << " pawns." << endl;
        // cout << "Mobility: " << (float)mobility / 100.0f << " pawns." << endl;
        // cout << "Double pawn: " << (float)doublePawn / 100.0f << " pawns." << endl;
        // cout << "Connected pawn: " << (float)connectedPawn / 100.0f << " pawns." << endl;
        // cout << "Pawn promotion: " << (float)pawnPromotion / 100.0f << " pawns." << endl;
        // cout << "Crashed castling: " << (float)crashedCastling / 100.0f << " pawns." << endl;
        // cout << "Pawn shield: " << (float)pawnShield / 100.0f << " pawns." << endl;
        // cout << "Two bishops: " << (float)twoBishops / 100.0f << " pawns." << endl;
        // cout << "Endgame: " << (float)endgame / 100.0f << " pawns." << endl;
        // cout << "Sum: " << (float)evaluation / 100.0f << " pawns." << endl;
    }

    return evaluation;
}

int32_t StaticEvaluator::pieceMaterial(Pieces pieces, uint8_t side)
{
    int32_t material = 0;

    material = material + StaticEvaluator::Material::Pawn * (countOnes(pieces.pieceBitboards[side][Pieces::Pawn]));
    material = material + StaticEvaluator::Material::Knight * (countOnes(pieces.pieceBitboards[side][Pieces::Knight]));
    material = material + StaticEvaluator::Material::Bishop * (countOnes(pieces.pieceBitboards[side][Pieces::Bishop]));
    material = material + StaticEvaluator::Material::Rook * (countOnes(pieces.pieceBitboards[side][Pieces::Rook]));
    material = material + StaticEvaluator::Material::Queen * (countOnes(pieces.pieceBitboards[side][Pieces::Queen]));

    return material;
}

int32_t StaticEvaluator::material(Pieces pieces)
{
    int32_t material = 0;

    material = material + StaticEvaluator::Material::Pawn * (countOnes(pieces.pieceBitboards[Pieces::White][Pieces::Pawn]) - countOnes(pieces.pieceBitboards[Pieces::Black][Pieces::Pawn]));
    material = material + StaticEvaluator::Material::Knight * (countOnes(pieces.pieceBitboards[Pieces::White][Pieces::Knight]) - countOnes(pieces.pieceBitboards[Pieces::Black][Pieces::Knight]));
    material = material + StaticEvaluator::Material::Bishop * (countOnes(pieces.pieceBitboards[Pieces::White][Pieces::Bishop]) - countOnes(pieces.pieceBitboards[Pieces::Black][Pieces::Bishop]));
    material = material + StaticEvaluator::Material::Rook * (countOnes(pieces.pieceBitboards[Pieces::White][Pieces::Rook]) - countOnes(pieces.pieceBitboards[Pieces::Black][Pieces::Rook]));
    material = material + StaticEvaluator::Material::Queen * (countOnes(pieces.pieceBitboards[Pieces::White][Pieces::Queen]) - countOnes(pieces.pieceBitboards[Pieces::Black][Pieces::Queen]));

    return material;
}

int32_t StaticEvaluator::mobility(Pieces pieces)
{
    int32_t mobility = 0;

    array<array<Bitboard, 6>, 2> iteration_masks = pieces.pieceBitboards;
    uint8_t index;

    int32_t knight_moves = 0;
    int32_t bishop_moves = 0;
    int32_t rook_moves = 0;
    int32_t queen_moves = 0;

    while (iteration_masks[Pieces::White][Pieces::Knight])
    {
        index = bsf(iteration_masks[Pieces::White][Pieces::Knight]);
        setZero(iteration_masks[Pieces::White][Pieces::Knight], index);
        knight_moves = knight_moves + countOnes(PsLegalMoveMaskGen::generateKnightMask(pieces, index, Pieces::White, false));
    }

    while (iteration_masks[Pieces::White][Pieces::Bishop])
    {
        index = bsf(iteration_masks[Pieces::White][Pieces::Bishop]);
        setZero(iteration_masks[Pieces::White][Pieces::Bishop], index);
        bishop_moves = bishop_moves + countOnes(PsLegalMoveMaskGen::generateBishopMask(pieces, index, Pieces::White, false));
    }

    while (iteration_masks[Pieces::White][Pieces::Rook])
    {
        index = bsf(iteration_masks[Pieces::White][Pieces::Rook]);
        setZero(iteration_masks[Pieces::White][Pieces::Rook], index);
        rook_moves = rook_moves + countOnes(PsLegalMoveMaskGen::generateRookMask(pieces, index, Pieces::White, false));
    }

    while (iteration_masks[Pieces::White][Pieces::Queen])
    {
        index = bsf(iteration_masks[Pieces::White][Pieces::Queen]);
        setZero(iteration_masks[Pieces::White][Pieces::Queen], index);
        queen_moves = queen_moves + countOnes(PsLegalMoveMaskGen::generateQueenMask(pieces, index, Pieces::White, false));
    }

    while (iteration_masks[Pieces::Black][Pieces::Knight])
    {
        index = bsf(iteration_masks[Pieces::Black][Pieces::Knight]);
        setZero(iteration_masks[Pieces::Black][Pieces::Knight], index);
        knight_moves = knight_moves - countOnes(PsLegalMoveMaskGen::generateKnightMask(pieces, index, Pieces::Black, false));
    }

    while (iteration_masks[Pieces::Black][Pieces::Bishop])
    {
        index = bsf(iteration_masks[Pieces::Black][Pieces::Bishop]);
        setZero(iteration_masks[Pieces::Black][Pieces::Bishop], index);
        bishop_moves = bishop_moves - countOnes(PsLegalMoveMaskGen::generateBishopMask(pieces, index, Pieces::Black, false));
    }

    while (iteration_masks[Pieces::Black][Pieces::Rook])
    {
        index = bsf(iteration_masks[Pieces::Black][Pieces::Rook]);
        setZero(iteration_masks[Pieces::Black][Pieces::Rook], index);
        rook_moves = rook_moves - countOnes(PsLegalMoveMaskGen::generateRookMask(pieces, index, Pieces::Black, false));
    }

    while (iteration_masks[Pieces::Black][Pieces::Queen])
    {
        index = bsf(iteration_masks[Pieces::Black][Pieces::Queen]);
        setZero(iteration_masks[Pieces::Black][Pieces::Queen], index);
        queen_moves = queen_moves - countOnes(PsLegalMoveMaskGen::generateQueenMask(pieces, index, Pieces::Black, false));
    }

    mobility = mobility + StaticEvaluator::Mobility::Knight * knight_moves;
    mobility = mobility + StaticEvaluator::Mobility::Bishop * bishop_moves;
    mobility = mobility + StaticEvaluator::Mobility::Rook * rook_moves;
    mobility = mobility + StaticEvaluator::Mobility::Queen * queen_moves;

    return mobility;
}

int32_t StaticEvaluator::pawnStructureDoublePawn(Pieces pieces)
{
    int32_t doublePawn = 0;

    int32_t double_pawn_ctr = 0;

    uint8_t whitePawns;
    uint8_t blackPawns;

    for (uint8_t x = 0; x < 8; x = x + 1)
    {
        whitePawns = countOnes(pieces.pieceBitboards[Pieces::White][Pieces::Pawn] & BitboardColumns::Columns[x]);
        blackPawns = countOnes(pieces.pieceBitboards[Pieces::Black][Pieces::Pawn] & BitboardColumns::Columns[x]);

        double_pawn_ctr = double_pawn_ctr + std::max(0, whitePawns - 1);
        double_pawn_ctr = double_pawn_ctr - std::max(0, blackPawns - 1);
    }

    doublePawn = doublePawn + StaticEvaluator::PawnStructure::DoublePawn * double_pawn_ctr;

    return doublePawn;
}

int32_t StaticEvaluator::pawnStructureConnectedPawn(Pieces pieces)
{
    int32_t connectedPawn = 0;

    int32_t connected_pawn_ctr = 0;

    Bitboard white_captures = PsLegalMoveMaskGen::generatePawnLeftCapturesMask(pieces, Pieces::White, true) | PsLegalMoveMaskGen::generatePawnRightCapturesMask(pieces, Pieces::White, true);
    Bitboard black_captures = PsLegalMoveMaskGen::generatePawnLeftCapturesMask(pieces, Pieces::Black, true) | PsLegalMoveMaskGen::generatePawnRightCapturesMask(pieces, Pieces::Black, true);

    connected_pawn_ctr = connected_pawn_ctr + countOnes(white_captures & pieces.pieceBitboards[Pieces::White][Pieces::Pawn]);
    connected_pawn_ctr = connected_pawn_ctr - countOnes(black_captures & pieces.pieceBitboards[Pieces::Black][Pieces::Pawn]);

    connectedPawn = connectedPawn + StaticEvaluator::PawnStructure::ConnectedPawn * connected_pawn_ctr;

    return connectedPawn;
}

int32_t StaticEvaluator::pawnStructurePawnPromotion(Pieces pieces)
{
    int32_t pawnPromotion = 0;

    Bitboard whitePawns = pieces.pieceBitboards[Pieces::White][Pieces::Pawn];
    Bitboard blackPawns = pieces.pieceBitboards[Pieces::Black][Pieces::Pawn];

    uint8_t index;

    while (whitePawns)
    {
        index = bsf(whitePawns);
        setZero(whitePawns, index);

        if (PassedPawnMasks::WhitePassedPawnMasks[index] & pieces.pieceBitboards[Pieces::Black][Pieces::Pawn])
            pawnPromotion = pawnPromotion + StaticEvaluator::PawnStructure::DefaultPawnPromotion[index / 8];
        else
            pawnPromotion = pawnPromotion + StaticEvaluator::PawnStructure::PassedPawnPromotion[index / 8];
    }

    while (blackPawns)
    {
        index = bsf(blackPawns);
        setZero(blackPawns, index);

        if (PassedPawnMasks::BlackPassedPawnMasks[index] & pieces.pieceBitboards[Pieces::White][Pieces::Pawn])
            pawnPromotion = pawnPromotion - StaticEvaluator::PawnStructure::DefaultPawnPromotion[7 - index / 8];
        else
            pawnPromotion = pawnPromotion - StaticEvaluator::PawnStructure::PassedPawnPromotion[7 - index / 8];
    }

    return pawnPromotion;
}

int32_t StaticEvaluator::kingSafetyCrashedCastling(bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling, bool whiteCastlingHappened, bool blackCastlingHappened)
{
    int32_t crashedCastling = 0;

    if (!whiteCastlingHappened)
    {
        if (!whiteLongCastling)
            crashedCastling = crashedCastling + StaticEvaluator::KingSafety::CrashedCastling;
        if (!whiteShortCastling)
            crashedCastling = crashedCastling + StaticEvaluator::KingSafety::CrashedCastling;
    }

    if (!blackCastlingHappened)
    {
        if (!blackLongCastling)
            crashedCastling = crashedCastling - StaticEvaluator::KingSafety::CrashedCastling;
        if (!blackShortCastling)
            crashedCastling = crashedCastling - StaticEvaluator::KingSafety::CrashedCastling;
    }

    return crashedCastling;
}

int32_t StaticEvaluator::kingSafetyPawnShield(Pieces pieces, bool whiteCastlingHappened, bool blackCastlingHappened)
{
    int32_t pawnShield = 0;

    int32_t pawnShieldCtr = 0;

    if (whiteCastlingHappened)
    {
        Bitboard whitePawns = pieces.pieceBitboards[Pieces::White][Pieces::Pawn];
        Bitboard whitePawnShield = PawnShieldMasks::WhitePawnShieldMasks[bsf(pieces.pieceBitboards[Pieces::White][Pieces::King])];
        pawnShieldCtr = pawnShieldCtr + countOnes(whitePawns & whitePawnShield);
    }

    if (blackCastlingHappened)
    {
        Bitboard blackPawns = pieces.pieceBitboards[Pieces::Black][Pieces::Pawn];
        Bitboard blackPawnShield = PawnShieldMasks::BlackPawnShieldMasks[bsf(pieces.pieceBitboards[Pieces::Black][Pieces::King])];
        pawnShieldCtr = pawnShieldCtr - countOnes(blackPawns & blackPawnShield);
    }

    pawnShield = pawnShield + StaticEvaluator::KingSafety::PawnShield * pawnShieldCtr;

    return pawnShield;
}

int32_t StaticEvaluator::twoBishops(Pieces pieces)
{
    int32_t twoBishops = 0;

    if (countOnes(pieces.pieceBitboards[Pieces::White][Pieces::Bishop]) >= 2)
        twoBishops = twoBishops + StaticEvaluator::TwoBishops;
    if (countOnes(pieces.pieceBitboards[Pieces::Black][Pieces::Bishop]) >= 2)
        twoBishops = twoBishops - StaticEvaluator::TwoBishops;

    return twoBishops;
}

int32_t StaticEvaluator::endgame(Pieces pieces, bool whiteLeading)
{
    int32_t endgame = 0;

    if (countOnes(pieces.all) > StaticEvaluator::Endgame::MaximumPiecesForEndgame)
        return endgame;

    uint8_t attacker_side;
    uint8_t defender_side;

    if (whiteLeading)
    {
        attacker_side = Pieces::White;
        defender_side = Pieces::Black;
    }
    else
    {
        attacker_side = Pieces::Black;
        defender_side = Pieces::White;
    }

    uint8_t attacker_king_p = bsf(pieces.pieceBitboards[attacker_side][Pieces::King]);
    uint8_t attacker_king_x = attacker_king_p % 8;
    uint8_t attacker_king_y = attacker_king_p / 8;

    uint8_t defender_king_p = bsf(pieces.pieceBitboards[defender_side][Pieces::King]);
    uint8_t defender_king_x = defender_king_p % 8;
    uint8_t defender_king_y = defender_king_p / 8;

    endgame = endgame + StaticEvaluator::Endgame::AttackerKingProximityToDefenderKing * (16 - std::abs(attacker_king_x - defender_king_x) - std::abs(attacker_king_y - defender_king_y));
    endgame = endgame + StaticEvaluator::Endgame::DistanceBetweenDefenderKingAndMiddle * (std::abs(defender_king_x - 3) + std::abs(defender_king_y - 4));

    if (!whiteLeading)
        endgame = -endgame;

    return endgame;
}