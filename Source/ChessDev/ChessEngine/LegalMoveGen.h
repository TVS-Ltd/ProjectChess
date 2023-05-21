// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Position.h"
#include "MoveList.h"
#include "MoveSorter.h"
#include "Constants.h"
#include "bitboards.h"
#include "PsLegalMoveMaskGen.h"

/**
 * 
 */
class CHESSDEV_API LegalMoveGen
{
public:
    static MoveList generate(const BoardPosition& position, uint8_t side, bool onlyCaptures = false);

    static void generatePawnMoves(const BoardPosition& position, uint8_t side, MoveList& moves, bool onlyCaptures = false);

    static void generateKnightMoves(const BoardPosition& position, uint8_t side, MoveList& moves, bool onlyCaptures = false);

    static void generateBishopMoves(const BoardPosition& position, uint8_t side, MoveList& moves, bool onlyCaptures = false);

    static void generateRookMoves(const BoardPosition& position, uint8_t side, MoveList& moves, bool onlyCaptures = false);

    static void generateQueenMoves(const BoardPosition& position, uint8_t side, MoveList& moves, bool onlyCaptures = false);

    static void generateKingMoves(const BoardPosition& position, uint8_t side, MoveList& moves, bool onlyCaptures = false);

    static MoveList generateCardMoves(const BoardPosition& position, uint8_t side);

    static MoveList generateMovesRoyal(const BoardPosition& position, uint8_t side, bool onlyCaptures = false);

private:
    static void generateMovesByCard(const BoardPosition& position, Bitboard freePieces, const uint8_t& side, const uint8_t& figureType, MoveList& moves);

    static void pieceMaskToMoves(const Pieces& pieces, Bitboard mask, uint8_t attackerPos, uint8_t attackerType, uint8_t attackerSide, MoveList& moves);

    static void pawnMaskToMoves(const Pieces& pieces, Bitboard mask, uint8_t attackerSide, int8_t attackerIndex, bool lookForDefender, uint8_t flag, MoveList& moves);

    static bool isLegal(Pieces pieces, ChessMove move, bool enPassantCapture);

    static void addEnPassantCaptures(const Pieces& pieces, uint8_t side, uint8_t enPassant, MoveList& moves);

    static void addCastlingMoves(const Pieces& pieces, uint8_t side, bool longCastling, bool shortCastling, MoveList& moves);
};
