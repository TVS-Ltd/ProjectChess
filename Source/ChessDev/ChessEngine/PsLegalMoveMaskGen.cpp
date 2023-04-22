// Fill out your copyright notice in the Description page of Project Settings.


#include "PsLegalMoveMaskGen.h"

// —————————————————— Legality Check ——————————————————
Bitboard PsLegalMoveMaskGen::generatePawnDafaultMask(Pieces pieces, uint8_t side)
{
    if (side == Pieces::White)
    {
        return (pieces.pieceBitboards[Pieces::White][Pieces::Pawn] << 8) & pieces.empty;
    }

    return (pieces.pieceBitboards[Pieces::Black][Pieces::Pawn] >> 8) & pieces.empty;
}

Bitboard PsLegalMoveMaskGen::generatePawnLongMask(Pieces pieces, uint8_t side)
{
    Bitboard defaultMask = PsLegalMoveMaskGen::generatePawnDafaultMask(pieces, side);

    if (side == Pieces::White)
    {
        return ((defaultMask & BitboardRows::Rows[2]) << 8) & pieces.empty;
    }

    return ((defaultMask & BitboardRows::Rows[5]) >> 8) & pieces.empty;
}

Bitboard PsLegalMoveMaskGen::generatePawnLeftCapturesMask(Pieces pieces, uint8_t side, bool includeAllPossibleCaptures)
{
    if (side == Pieces::White)
    {
        Bitboard mask = (pieces.pieceBitboards[Pieces::White][Pieces::Pawn] << 7) & BitboardColumns::InversionColumns[7];

        if (!includeAllPossibleCaptures)
        {
            mask = mask & pieces.sideBitboards[Pieces::Black];
        }

        return mask;
    }

    Bitboard mask = (pieces.pieceBitboards[Pieces::Black][Pieces::Pawn] >> 9) & BitboardColumns::InversionColumns[7];

    if (!includeAllPossibleCaptures)
    {
        mask = mask & pieces.sideBitboards[Pieces::White];
    }

    return mask;
}

Bitboard PsLegalMoveMaskGen::generatePawnRightCapturesMask(Pieces pieces, uint8_t side, bool includeAllPossibleCaptures)
{
    if (side == Pieces::White)
    {
        Bitboard mask = (pieces.pieceBitboards[Pieces::White][Pieces::Pawn] << 9) & BitboardColumns::InversionColumns[0];

        if (!includeAllPossibleCaptures)
        {
            mask = mask & pieces.sideBitboards[Pieces::Black];
        }

        return mask;
    }

    Bitboard mask = (pieces.pieceBitboards[Pieces::Black][Pieces::Pawn] >> 7) & BitboardColumns::InversionColumns[0];

    if (!includeAllPossibleCaptures)
    {
        mask = mask & pieces.sideBitboards[Pieces::White];
    }

    return mask;
}

Bitboard PsLegalMoveMaskGen::generateKnightMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures)
{
    if (onlyCaptures)
    {
        return KnightMasks::Masks[p] & pieces.sideBitboards[Pieces::inverse(side)];
    }

    return KnightMasks::Masks[p] & pieces.inversionSideBitboards[side];
}

Bitboard PsLegalMoveMaskGen::generateBishopMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures)
{
    Bitboard nw = PsLegalMoveMaskGen::calculateRay(pieces, p, side, onlyCaptures, SlidersMasks::Direction::NorthWest, false);
    Bitboard ne = PsLegalMoveMaskGen::calculateRay(pieces, p, side, onlyCaptures, SlidersMasks::Direction::NorthEast, false);
    Bitboard sw = PsLegalMoveMaskGen::calculateRay(pieces, p, side, onlyCaptures, SlidersMasks::Direction::SouthWest, true);
    Bitboard se = PsLegalMoveMaskGen::calculateRay(pieces, p, side, onlyCaptures, SlidersMasks::Direction::SouthEast, true);

    return nw | ne | sw | se;
}

Bitboard PsLegalMoveMaskGen::generateRookMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures)
{
    Bitboard n = PsLegalMoveMaskGen::calculateRay(pieces, p, side, onlyCaptures, SlidersMasks::Direction::North, false);
    Bitboard s = PsLegalMoveMaskGen::calculateRay(pieces, p, side, onlyCaptures, SlidersMasks::Direction::South, true);
    Bitboard w = PsLegalMoveMaskGen::calculateRay(pieces, p, side, onlyCaptures, SlidersMasks::Direction::West, true);
    Bitboard e = PsLegalMoveMaskGen::calculateRay(pieces, p, side, onlyCaptures, SlidersMasks::Direction::East, false);

    return n | s | w | e;
}

Bitboard PsLegalMoveMaskGen::generateQueenMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures)
{
    Bitboard bishopMask = PsLegalMoveMaskGen::generateBishopMask(pieces, p, side, onlyCaptures);
    Bitboard rookMask = PsLegalMoveMaskGen::generateRookMask(pieces, p, side, onlyCaptures);

    return bishopMask | rookMask;
}

Bitboard PsLegalMoveMaskGen::generateKingMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures)
{
    if (onlyCaptures)
    {
        return KingMasks::Masks[p] & pieces.sideBitboards[Pieces::inverse(side)];
    }

    return KingMasks::Masks[p] & pieces.inversionSideBitboards[side];
}

bool PsLegalMoveMaskGen::inDanger(Pieces pieces, uint8_t p, uint8_t side)
{
    Bitboard oppositePawnsLeftCaptures = PsLegalMoveMaskGen::generatePawnLeftCapturesMask(pieces, Pieces::inverse(side), true);
    Bitboard oppositePawnsRightCaptures = PsLegalMoveMaskGen::generatePawnRightCapturesMask(pieces, Pieces::inverse(side), true);
    Bitboard oppositePawnsCaptures = oppositePawnsLeftCaptures | oppositePawnsRightCaptures;

    if (getBit(oppositePawnsCaptures, p))
    {
        return true;
    }

    if (PsLegalMoveMaskGen::generateKnightMask(pieces, p, side, true) & pieces.pieceBitboards[Pieces::inverse(side)][Pieces::Knight])
    {
        return true;
    }

    if (PsLegalMoveMaskGen::generateBishopMask(pieces, p, side, true) & pieces.pieceBitboards[Pieces::inverse(side)][Pieces::Bishop])
    {
        return true;
    }

    if (PsLegalMoveMaskGen::generateRookMask(pieces, p, side, true) & pieces.pieceBitboards[Pieces::inverse(side)][Pieces::Rook])
    {
        return true;
    }

    if (PsLegalMoveMaskGen::generateQueenMask(pieces, p, side, true) & pieces.pieceBitboards[Pieces::inverse(side)][Pieces::Queen])
    {
        return true;
    }

    if (PsLegalMoveMaskGen::generateKingMask(pieces, p, side, true) & pieces.pieceBitboards[Pieces::inverse(side)][Pieces::King])
    {
        return true;
    }

    return false;
}

Bitboard PsLegalMoveMaskGen::calculateRay(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures, uint8_t direction, bool bsr)
{
    Bitboard blockers = SlidersMasks::Masks[p][direction] & pieces.all;

    if (blockers == 0)
    {
        if (onlyCaptures)
        {
            return 0;
        }

        return SlidersMasks::Masks[p][direction];
    }

    uint8_t blockingSquare;

    if (bsr)
    {
        blockingSquare = bsrFunc(blockers);
    } else
    {
        blockingSquare = bsf(blockers);
    }
            

    Bitboard moves;

    if (onlyCaptures)
    {
        moves = 0;
    } else
    {
        moves = SlidersMasks::Masks[p][direction] ^ SlidersMasks::Masks[blockingSquare][direction];
    }

    if (getBit(pieces.sideBitboards[side], blockingSquare))
    {
        setZero(moves, blockingSquare);
    } else
    {
        setOne(moves, blockingSquare);
    }

    return moves;
}
