// Fill out your copyright notice in the Description page of Project Settings.


#include "LegalMoveGen.h"

// MoveList LegalMoveGen::generate(BoardPosition position, uint8_t side, bool onlyCaptures)
// {

//     MoveList moves;

//     Bitboard pawnLeftCaptureMask = PsLegalMoveMaskGen::generatePawnLeftCapturesMask(position.pieces, side, false);
//     Bitboard pawnRightCapturesMask = PsLegalMoveMaskGen::generatePawnRightCapturesMask(position.pieces, side, false);

//     int8_t pawnLeftCapture;
//     int8_t pawnRightCapture;

//     if (side == Pieces::White)
//     {
//         pawnLeftCapture = -7;
//         pawnRightCapture = -9;
//     }
//     else
//     {
//         pawnLeftCapture = 9;
//         pawnRightCapture = 7;
//     }

//     LegalMoveGen::pawnMaskToMoves(position.pieces, pawnLeftCaptureMask, side, pawnLeftCapture, true, ChessMove::Flag::Default, moves);
//     LegalMoveGen::pawnMaskToMoves(position.pieces, pawnRightCapturesMask, side, pawnRightCapture, true, ChessMove::Flag::Default, moves);

//     if (!onlyCaptures)
//     {
//         Bitboard pawnDafaultMask = PsLegalMoveMaskGen::generatePawnDafaultMask(position.pieces, side);
//         Bitboard pawnLongMask = PsLegalMoveMaskGen::generatePawnLongMask(position.pieces, side);

//         int8_t pawnDefaultMove;
//         int8_t pawnLongMove;

//         if (side == Pieces::White)
//         {
//             pawnDefaultMove = -8;
//             pawnLongMove = -16;
//         }
//         else
//         {
//             pawnDefaultMove = 8;
//             pawnLongMove = 16;
//         }

//         LegalMoveGen::pawnMaskToMoves(position.pieces, pawnDafaultMask, side, pawnDefaultMove, false, ChessMove::Flag::Default, moves);
//         LegalMoveGen::pawnMaskToMoves(position.pieces, pawnLongMask, side, pawnLongMove, false, ChessMove::Flag::PawnLongMove, moves);
//     }

//     Bitboard allKnights = position.pieces.pieceBitboards[side][Pieces::Knight];
//     Bitboard allBishops = position.pieces.pieceBitboards[side][Pieces::Bishop];
//     Bitboard allRooks = position.pieces.pieceBitboards[side][Pieces::Rook];
//     Bitboard allQueens = position.pieces.pieceBitboards[side][Pieces::Queen];

//     uint8_t attackerPos;
//     Bitboard mask;

//     while (allKnights)
//     {
//         attackerPos = bsf(allKnights);
//         setZero(allKnights, attackerPos);
//         mask = PsLegalMoveMaskGen::generateKnightMask(position.pieces, attackerPos, side, onlyCaptures);
//         LegalMoveGen::pieceMaskToMoves(position.pieces, mask, attackerPos, Pieces::Knight, side, moves);
//     }

//     while (allBishops)
//     {
//         attackerPos = bsf(allBishops);
//         setZero(allBishops, attackerPos);
//         mask = PsLegalMoveMaskGen::generateBishopMask(position.pieces, attackerPos, side, onlyCaptures);
//         LegalMoveGen::pieceMaskToMoves(position.pieces, mask, attackerPos, Pieces::Bishop, side, moves);
//     }

//     while (allRooks)
//     {
//         attackerPos = bsf(allRooks);
//         setZero(allRooks, attackerPos);
//         mask = PsLegalMoveMaskGen::generateRookMask(position.pieces, attackerPos, side, onlyCaptures);
//         LegalMoveGen::pieceMaskToMoves(position.pieces, mask, attackerPos, Pieces::Rook, side, moves);
//     }

//     while (allQueens)
//     {
//         attackerPos = bsf(allQueens);
//         setZero(allQueens, attackerPos);
//         mask = PsLegalMoveMaskGen::generateQueenMask(position.pieces, attackerPos, side, onlyCaptures);
//         LegalMoveGen::pieceMaskToMoves(position.pieces, mask, attackerPos, Pieces::Queen, side, moves);
//     }

//     attackerPos = bsf(position.pieces.pieceBitboards[side][Pieces::King]);
//     mask = PsLegalMoveMaskGen::generateKingMask(position.pieces, attackerPos, side, onlyCaptures);
//     LegalMoveGen::pieceMaskToMoves(position.pieces, mask, attackerPos, Pieces::King, side, moves);

//     LegalMoveGen::addEnPassantCaptures(position.pieces, side, position.EnPassant, moves);

//     if (!onlyCaptures)
//     {
//         if (side == Pieces::White)
//         {
//             LegalMoveGen::addCastlingMoves(position.pieces, Pieces::White, position.WhiteLongCastling, position.WhiteShortCastling, moves);
//         }
//         else
//         {
//             LegalMoveGen::addCastlingMoves(position.pieces, Pieces::Black, position.BlackLongCastling, position.BlackShortCastling, moves);
//         }
//     }

//     return moves;
// }

// void LegalMoveGen::pieceMaskToMoves(Pieces pieces, Bitboard mask, uint8_t attackerPos, uint8_t attackerType, uint8_t attackerSide, MoveList &moves)
// {
//     uint8_t defenderPos;
//     uint8_t defenderType;

//     class ChessMove move;

//     while (mask)
//     {
//         defenderPos = bsf(mask);
//         setZero(mask, defenderPos);

//         defenderType = 255;

//         for (uint8_t i = 0; i < 6; i++)
//         {
//             if (getBit(pieces.pieceBitboards[Pieces::inverse(attackerSide)][i], defenderPos))
//             {
//                 defenderType = i;
//                 break;
//             }
//         }

//         move = {attackerPos, defenderPos, attackerType, attackerSide, defenderType, Pieces::inverse(attackerSide)};

//         if (LegalMoveGen::isLegal(pieces, move, false))
//         {
//             moves.push_back(move);
//         }
//     }
// }

// void LegalMoveGen::pawnMaskToMoves(Pieces pieces, Bitboard mask, uint8_t attackerSide, int8_t attackerIndex, bool lookForDefender, uint8_t flag, MoveList &moves)
// {
//     uint8_t defenderPos;
//     uint8_t defenderType = 255;

//     class ChessMove move;

//     while (mask)
//     {
//         defenderPos = bsf(mask);
//         setZero(mask, defenderPos);

//         if (lookForDefender)
//         {
//             defenderType = 255;

//             for (uint8_t i = 0; i < 6; i++)
//             {
//                 if (getBit(pieces.pieceBitboards[Pieces::inverse(attackerSide)][i], defenderPos))
//                 {
//                     defenderType = i;
//                     break;
//                 }
//             }
//         }

//         move = {(uint8_t)(defenderPos + attackerIndex), defenderPos, Pieces::Pawn, attackerSide, defenderType, Pieces::inverse(attackerSide), flag};

//         if (LegalMoveGen::isLegal(pieces, move, false))
//         {
//             if (defenderPos < 8 or defenderPos > 55)
//             {
//                 moves.push_back({(uint8_t)(defenderPos + attackerIndex), defenderPos, 0, attackerSide, defenderType, Pieces::inverse(attackerSide), ChessMove::Flag::PromoteToKnight});
//                 moves.push_back({(uint8_t)(defenderPos + attackerIndex), defenderPos, 0, attackerSide, defenderType, Pieces::inverse(attackerSide), ChessMove::Flag::PromoteToBishop});
//                 moves.push_back({(uint8_t)(defenderPos + attackerIndex), defenderPos, 0, attackerSide, defenderType, Pieces::inverse(attackerSide), ChessMove::Flag::PromoteToRook});
//                 moves.push_back({(uint8_t)(defenderPos + attackerIndex), defenderPos, 0, attackerSide, defenderType, Pieces::inverse(attackerSide), ChessMove::Flag::PromoteToQueen});
//             }
//             else
//             {
//                 moves.push_back(move);
//             }
//         }
//     }
// }

// bool LegalMoveGen::isLegal(Pieces pieces, class ChessMove move, bool enPassantCapture)
// {
//     setZero(pieces.pieceBitboards[move.AttackerSide][move.AttackerType], move.From);
//     setOne(pieces.pieceBitboards[move.AttackerSide][move.AttackerType], move.To);

//     if (move.DefenderType != 255)
//     {
//         setZero(pieces.pieceBitboards[move.DefenderSide][move.DefenderType], move.To);
//     }

//     if (enPassantCapture)
//     {
//         if (move.AttackerSide == Pieces::White)
//         {
//             setZero(pieces.pieceBitboards[Pieces::Black][Pieces::Pawn], move.To - 8);
//         }

//         setZero(pieces.pieceBitboards[Pieces::White][Pieces::Pawn], move.To + 8);
//     }

//     pieces.updateBitboards();

//     if (PsLegalMoveMaskGen::inDanger(pieces, bsf(pieces.pieceBitboards[move.AttackerSide][Pieces::King]), move.AttackerSide))
//     {
//         return false;
//     }

//     return true;
// }

// void LegalMoveGen::addEnPassantCaptures(Pieces pieces, uint8_t side, uint8_t enPassant, MoveList &moves)
// {
//     if (enPassant == 255)
//     {
//         return;
//     }

//     class ChessMove move;

//     if (side == Pieces::White)
//     {
//         if (enPassant % 8 != 7 and getBit(pieces.pieceBitboards[Pieces::White][Pieces::Pawn], enPassant - 7))
//         {
//             move = {(uint8_t)(enPassant - 7), enPassant, Pieces::Pawn, Pieces::White, 255, 255, ChessMove::Flag::EnPassantCapture};

//             if (LegalMoveGen::isLegal(pieces, move, true))
//             {
//                 moves.push_back(move);
//             }
//         }

//         if (enPassant % 8 != 0 and getBit(pieces.pieceBitboards[Pieces::White][Pieces::Pawn], enPassant - 9))
//         {
//             move = {(uint8_t)(enPassant - 9), enPassant, Pieces::Pawn, Pieces::White, 255, 255, ChessMove::Flag::EnPassantCapture};

//             if (LegalMoveGen::isLegal(pieces, move, true))
//             {
//                 moves.push_back(move);
//             }
//         }
//     }
//     else
//     {
//         if (enPassant % 8 != 0 and getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Pawn], enPassant + 7))
//         {
//             move = {(uint8_t)(enPassant + 7), enPassant, Pieces::Pawn, Pieces::Black, 255, 255, ChessMove::Flag::EnPassantCapture};

//             if (LegalMoveGen::isLegal(pieces, move, true))
//             {
//                 moves.push_back(move);
//             }
//         }

//         if (enPassant % 8 != 7 and getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Pawn], enPassant + 9))
//         {
//             move = {(uint8_t)(enPassant + 9), enPassant, Pieces::Pawn, Pieces::Black, 255, 255, ChessMove::Flag::EnPassantCapture};

//             if (LegalMoveGen::isLegal(pieces, move, true))
//             {
//                 moves.push_back(move);
//             }
//         }
//     }
// }

// void LegalMoveGen::addCastlingMoves(Pieces pieces, uint8_t side, bool longCastling, bool shortCastling, MoveList &moves)
// {
//     uint8_t index;
//     uint8_t longCastlingFlag;
//     uint8_t shortCastlingFlag;

//     if (side == Pieces::White)
//     {
//         index = 0;
//         longCastlingFlag = ChessMove::Flag::WhiteLongCastling;
//         shortCastlingFlag = ChessMove::Flag::WhiteShortCastling;
//     }
//     else
//     {
//         index = 56;
//         longCastlingFlag = ChessMove::Flag::BlackLongCastling;
//         shortCastlingFlag = ChessMove::Flag::BlackShortCastling;
//     }

//     if (longCastling && getBit(pieces.pieceBitboards[side][Pieces::Rook], 0 + index) && getBit(pieces.empty, 1 + index) && getBit(pieces.empty, 2 + index) && getBit(pieces.empty, 3 + index))
//     {
//         if (!PsLegalMoveMaskGen::inDanger(pieces, bsf(pieces.pieceBitboards[side][Pieces::King]), side) && !PsLegalMoveMaskGen::inDanger(pieces, 2 + index, side) && !PsLegalMoveMaskGen::inDanger(pieces, 3 + index, side))
//             moves.push_back({(uint8_t)(4 + index), (uint8_t)(2 + index), Pieces::King, side, 255, 255, longCastlingFlag});
//     }

//     if (shortCastling && getBit(pieces.pieceBitboards[side][Pieces::Rook], 7 + index) && getBit(pieces.empty, 5 + index) && getBit(pieces.empty, 6 + index))
//     {
//         if (!PsLegalMoveMaskGen::inDanger(pieces, bsf(pieces.pieceBitboards[side][Pieces::King]), side) && !PsLegalMoveMaskGen::inDanger(pieces, 5 + index, side) && !PsLegalMoveMaskGen::inDanger(pieces, 6 + index, side))
//             moves.push_back({(uint8_t)(4 + index), (uint8_t)(6 + index), Pieces::King, side, 255, 255, shortCastlingFlag});
//     }
// }



MoveList LegalMoveGen::generate(const BoardPosition& position, uint8_t side, bool onlyCaptures)
{
    MoveList moves;

    LegalMoveGen::generatePawnMoves(position, side, moves, onlyCaptures);

    LegalMoveGen::generateKnightMoves(position, side, moves, onlyCaptures);

    LegalMoveGen::generateBishopMoves(position, side, moves, onlyCaptures);

    LegalMoveGen::generateRookMoves(position, side, moves, onlyCaptures);

    LegalMoveGen::generateQueenMoves(position, side, moves, onlyCaptures);

    LegalMoveGen::generateKingMoves(position, side, moves, onlyCaptures);

    return moves;
}

void LegalMoveGen::generatePawnMoves(const BoardPosition& position, uint8_t side, MoveList& moves, bool onlyCaptures)
{
    Bitboard pawnLeftCaptureMask = PsLegalMoveMaskGen::generatePawnLeftCapturesMask(position.pieces, side, false);
    Bitboard pawnRightCapturesMask = PsLegalMoveMaskGen::generatePawnRightCapturesMask(position.pieces, side, false);

    int8_t pawnLeftCapture;
    int8_t pawnRightCapture;

    if (side == Pieces::White)
    {
        pawnLeftCapture = -7;
        pawnRightCapture = -9;
    }
    else
    {
        pawnLeftCapture = 9;
        pawnRightCapture = 7;
    }

    LegalMoveGen::pawnMaskToMoves(position.pieces, pawnLeftCaptureMask, side, pawnLeftCapture, true, ChessMove::Flag::Default, moves);
    LegalMoveGen::pawnMaskToMoves(position.pieces, pawnRightCapturesMask, side, pawnRightCapture, true, ChessMove::Flag::Default, moves);

    if (!onlyCaptures)
    {
        Bitboard pawnDafaultMask = PsLegalMoveMaskGen::generatePawnDafaultMask(position.pieces, side);
        Bitboard pawnLongMask = PsLegalMoveMaskGen::generatePawnLongMask(position.pieces, side);

        int8_t pawnDefaultMove;
        int8_t pawnLongMove;

        if (side == Pieces::White)
        {
            pawnDefaultMove = -8;
            pawnLongMove = -16;
        }
        else
        {
            pawnDefaultMove = 8;
            pawnLongMove = 16;
        }

        LegalMoveGen::pawnMaskToMoves(position.pieces, pawnDafaultMask, side, pawnDefaultMove, false, ChessMove::Flag::Default, moves);
        LegalMoveGen::pawnMaskToMoves(position.pieces, pawnLongMask, side, pawnLongMove, false, ChessMove::Flag::PawnLongMove, moves);
    }

    LegalMoveGen::addEnPassantCaptures(position.pieces, side, position.EnPassant, moves);
}

void LegalMoveGen::generateKnightMoves(const BoardPosition& position, uint8_t side, MoveList& moves, bool onlyCaptures)
{
    Bitboard allKnights = position.pieces.pieceBitboards[side][Pieces::Knight];

    uint8_t attackerPos;
    Bitboard mask;

    while (allKnights)
    {
        attackerPos = bsf(allKnights);
        setZero(allKnights, attackerPos);
        mask = PsLegalMoveMaskGen::generateKnightMask(position.pieces, attackerPos, side, onlyCaptures);
        LegalMoveGen::pieceMaskToMoves(position.pieces, mask, attackerPos, Pieces::Knight, side, moves);
    }
}

void LegalMoveGen::generateBishopMoves(const BoardPosition& position, uint8_t side, MoveList& moves, bool onlyCaptures)
{
    Bitboard allBishops = position.pieces.pieceBitboards[side][Pieces::Bishop];

    uint8_t attackerPos;
    Bitboard mask;

    while (allBishops)
    {
        attackerPos = bsf(allBishops);
        setZero(allBishops, attackerPos);
        mask = PsLegalMoveMaskGen::generateBishopMask(position.pieces, attackerPos, side, onlyCaptures);
        LegalMoveGen::pieceMaskToMoves(position.pieces, mask, attackerPos, Pieces::Bishop, side, moves);
    }
}

void LegalMoveGen::generateRookMoves(const BoardPosition& position, uint8_t side, MoveList& moves, bool onlyCaptures)
{
    Bitboard allRooks = position.pieces.pieceBitboards[side][Pieces::Rook];

    uint8_t attackerPos;
    Bitboard mask;

    while (allRooks)
    {
        attackerPos = bsf(allRooks);
        setZero(allRooks, attackerPos);
        mask = PsLegalMoveMaskGen::generateRookMask(position.pieces, attackerPos, side, onlyCaptures);
        LegalMoveGen::pieceMaskToMoves(position.pieces, mask, attackerPos, Pieces::Rook, side, moves);
    }
}

void LegalMoveGen::generateQueenMoves(const BoardPosition& position, uint8_t side, MoveList& moves, bool onlyCaptures)
{
    Bitboard allQueens = position.pieces.pieceBitboards[side][Pieces::Queen];

    uint8_t attackerPos;
    Bitboard mask;

    while (allQueens)
    {
        attackerPos = bsf(allQueens);
        setZero(allQueens, attackerPos);
        mask = PsLegalMoveMaskGen::generateQueenMask(position.pieces, attackerPos, side, onlyCaptures);
        LegalMoveGen::pieceMaskToMoves(position.pieces, mask, attackerPos, Pieces::Queen, side, moves);
    }
}

void LegalMoveGen::generateKingMoves(const BoardPosition& position, uint8_t side, MoveList& moves, bool onlyCaptures)
{
    uint8_t attackerPos;
    Bitboard mask;

    attackerPos = bsf(position.pieces.pieceBitboards[side][Pieces::King]);
    mask = PsLegalMoveMaskGen::generateKingMask(position.pieces, attackerPos, side, onlyCaptures);
    LegalMoveGen::pieceMaskToMoves(position.pieces, mask, attackerPos, Pieces::King, side, moves);

    if (!onlyCaptures)
    {
        if (side == Pieces::White)
        {
            LegalMoveGen::addCastlingMoves(position.pieces, Pieces::White, position.WhiteLongCastling, position.WhiteShortCastling, moves);
        }
        else
        {
            LegalMoveGen::addCastlingMoves(position.pieces, Pieces::Black, position.BlackLongCastling, position.BlackShortCastling, moves);
        }
    }
}

void LegalMoveGen::pieceMaskToMoves(const Pieces& pieces, Bitboard mask, uint8_t attackerPos, uint8_t attackerType, uint8_t attackerSide, MoveList& moves)
{
    uint8_t defenderPos;
    uint8_t defenderType;

    ChessMove move;

    while (mask)
    {
        defenderPos = bsf(mask);
        setZero(mask, defenderPos);

        defenderType = 255;

        for (uint8_t i = 0; i < 6; i++)
        {
            if (getBit(pieces.pieceBitboards[Pieces::inverse(attackerSide)][i], defenderPos))
            {
                defenderType = i;
                break;
            }
        }

        move = { attackerPos, defenderPos, attackerType, attackerSide, defenderType, Pieces::inverse(attackerSide), (defenderType == 255) ? MoveType::Quiet : MoveType::Capture};

        if (LegalMoveGen::isLegal(pieces, move, false))
        {
            moves.push_back(move);
        }
    }
}

MoveList LegalMoveGen::generateCardMoves(const BoardPosition& position, uint8_t side)
{
    MoveList moves;

    for (auto [type, count] : position.handsdecks[side])
    {
        if (count)
            LegalMoveGen::generateMovesByCard(position, position.pieces.empty & BitboardRows::SideRows[side], side, type, moves);
    }

    return moves;
}

MoveList LegalMoveGen::generateMovesRoyal(const BoardPosition& position, uint8_t side, bool onlyCaptures)
{
    MoveList moves;
    
    auto deck = position.handsdecks[side];
    bool joker = false;

    for (auto [type, count] : deck)
    {
        if (!count)
            continue;

        switch (type)
        {
        case Pieces::Pawn:
            LegalMoveGen::generatePawnMoves(position, side, moves, onlyCaptures);
            break;
        case Pieces::Knight:
            LegalMoveGen::generateKnightMoves(position, side, moves, onlyCaptures);
            break;
        case Pieces::Bishop:
            LegalMoveGen::generateBishopMoves(position, side, moves, onlyCaptures);
            break;
        case Pieces::Rook:
            LegalMoveGen::generateRookMoves(position, side, moves, onlyCaptures);
            break;
        case Pieces::Queen:
            LegalMoveGen::generateQueenMoves(position, side, moves, onlyCaptures);
            break;
        case Pieces::King:
            LegalMoveGen::generateKingMoves(position, side, moves, onlyCaptures);
            break;
        case Pieces::Joker:
            joker = true;
            break;
        }
    }

    for (uint8_t i = 0; i < moves.size(); i++)
        for (uint8_t j = 0; j < 6; j++)
        {
            if (moves[i] == Killers::royal[0][side][j] || moves[i] == Killers::royal[1][side][j])
            {
                moves[i].type = MoveType::Killer;
            }
        }

    if (joker)
    {
        moves = generate(position, side, onlyCaptures);

        MoveSorter::quickSort(position.pieces, moves, 0, moves.size() - 1);

        if (!onlyCaptures)
        {
            for (auto figure : Figures::figures)
            {
                uint8_t amount = countOnes(position.pieces.pieceBitboards[side][figure]);
                if (amount < Figures::figuresCount[figure])
                    generateMovesByCard(position, position.pieces.empty, side, figure, moves);
            }
        }
    }
    else
        MoveSorter::quickSort(position.pieces, moves, 0, moves.size() - 1);

    return moves;
}

void LegalMoveGen::generateMovesByCard(const BoardPosition& position, Bitboard freePieces, const uint8_t& side, const uint8_t& figureType, MoveList& moves)
{
    BoardPosition copy;
    while (freePieces)
    {
        uint8_t pos = bsf(freePieces);
        setZero(freePieces, pos);

        ChessMove move(255, pos, figureType, side, 255, Pieces::inverse(side), MoveType::LayOutCard);
        copy = position;
        copy.move(move);

        if (PsLegalMoveMaskGen::inDanger(copy.pieces, bsf(copy.pieces.pieceBitboards[side][Pieces::King]), side))
            continue;

        moves.push_back(move);
    }
}

void LegalMoveGen::pawnMaskToMoves(const Pieces& pieces, Bitboard mask, uint8_t attackerSide, int8_t attackerIndex, bool lookForDefender, uint8_t flag, MoveList& moves)
{
    uint8_t defenderPos;
    uint8_t defenderType = 255;

    ChessMove move;

    while (mask)
    {
        defenderPos = bsf(mask);
        setZero(mask, defenderPos);

        if (lookForDefender)
        {
            defenderType = 255;

            for (uint8_t i = 0; i < 6; i++)
            {
                if (getBit(pieces.pieceBitboards[Pieces::inverse(attackerSide)][i], defenderPos))
                {
                    defenderType = i;
                    break;
                }
            }
        }

        MoveType type = (defenderType == 255) ? MoveType::Quiet : MoveType::Capture;

        move = { (uint8_t)(defenderPos + attackerIndex), defenderPos, Pieces::Pawn, attackerSide, defenderType, Pieces::inverse(attackerSide), type, flag };

        if (LegalMoveGen::isLegal(pieces, move, false))
        {
            if (defenderPos < 8 or defenderPos > 55)
            {
                moves.push_back({ (uint8_t)(defenderPos + attackerIndex), defenderPos, 0, attackerSide, defenderType, Pieces::inverse(attackerSide), type, ChessMove::Flag::PromoteToKnight });
                moves.push_back({ (uint8_t)(defenderPos + attackerIndex), defenderPos, 0, attackerSide, defenderType, Pieces::inverse(attackerSide), type, ChessMove::Flag::PromoteToBishop });
                moves.push_back({ (uint8_t)(defenderPos + attackerIndex), defenderPos, 0, attackerSide, defenderType, Pieces::inverse(attackerSide), type, ChessMove::Flag::PromoteToRook });
                moves.push_back({ (uint8_t)(defenderPos + attackerIndex), defenderPos, 0, attackerSide, defenderType, Pieces::inverse(attackerSide), type, ChessMove::Flag::PromoteToQueen });
            }
            else
            {
                moves.push_back(move);
            }
        }
    }
}

bool LegalMoveGen::isLegal(Pieces pieces, ChessMove move, bool enPassantCapture)
{
    setZero(pieces.pieceBitboards[move.AttackerSide][move.AttackerType], move.From);
    setOne(pieces.pieceBitboards[move.AttackerSide][move.AttackerType], move.To);

    if (move.DefenderType != 255)
    {
        setZero(pieces.pieceBitboards[move.DefenderSide][move.DefenderType], move.To);
    }

    if (enPassantCapture)
    {
        if (move.AttackerSide == Pieces::White)
        {
            setZero(pieces.pieceBitboards[Pieces::Black][Pieces::Pawn], move.To - 8);
        }

        setZero(pieces.pieceBitboards[Pieces::White][Pieces::Pawn], move.To + 8);
    }

    pieces.updateBitboards();

    if (PsLegalMoveMaskGen::inDanger(pieces, bsf(pieces.pieceBitboards[move.AttackerSide][Pieces::King]), move.AttackerSide))
    {
        return false;
    }

    return true;
}

void LegalMoveGen::addEnPassantCaptures(const Pieces& pieces, uint8_t side, uint8_t enPassant, MoveList& moves)
{
    if (enPassant == 255)
        return;

    ChessMove move;

    if (side == Pieces::White)
    {
        if (enPassant % 8 != 7 and getBit(pieces.pieceBitboards[Pieces::White][Pieces::Pawn], enPassant - 7))
        {
            move = { (uint8_t)(enPassant - 7), enPassant, Pieces::Pawn, Pieces::White, 255, 255, MoveType::Capture, ChessMove::Flag::EnPassantCapture };

            if (LegalMoveGen::isLegal(pieces, move, true))
            {
                moves.push_back(move);
            }
        }

        if (enPassant % 8 != 0 and getBit(pieces.pieceBitboards[Pieces::White][Pieces::Pawn], enPassant - 9))
        {
            move = { (uint8_t)(enPassant - 9), enPassant, Pieces::Pawn, Pieces::White, 255, 255, MoveType::Capture, ChessMove::Flag::EnPassantCapture };

            if (LegalMoveGen::isLegal(pieces, move, true))
            {
                moves.push_back(move);
            }
        }
    }
    else
    {
        if (enPassant % 8 != 0 and getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Pawn], enPassant + 7))
        {
            move = { (uint8_t)(enPassant + 7), enPassant, Pieces::Pawn, Pieces::Black, 255, 255, MoveType::Capture, ChessMove::Flag::EnPassantCapture };

            if (LegalMoveGen::isLegal(pieces, move, true))
            {
                moves.push_back(move);
            }
        }

        if (enPassant % 8 != 7 and getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Pawn], enPassant + 9))
        {
            move = { (uint8_t)(enPassant + 9), enPassant, Pieces::Pawn, Pieces::Black, 255, 255, MoveType::Capture, ChessMove::Flag::EnPassantCapture };

            if (LegalMoveGen::isLegal(pieces, move, true))
            {
                moves.push_back(move);
            }
        }
    }
}

void LegalMoveGen::addCastlingMoves(const Pieces& pieces, uint8_t side, bool longCastling, bool shortCastling, MoveList& moves)
{
    uint8_t index;
    uint8_t longCastlingFlag;
    uint8_t shortCastlingFlag;

    if (side == Pieces::White)
    {
        index = 0;
        longCastlingFlag = ChessMove::Flag::WhiteLongCastling;
        shortCastlingFlag = ChessMove::Flag::WhiteShortCastling;
    }
    else
    {
        index = 56;
        longCastlingFlag = ChessMove::Flag::BlackLongCastling;
        shortCastlingFlag = ChessMove::Flag::BlackShortCastling;
    }

    if (longCastling && getBit(pieces.pieceBitboards[side][Pieces::Rook], 0 + index) && getBit(pieces.empty, 1 + index) && getBit(pieces.empty, 2 + index) && getBit(pieces.empty, 3 + index))
    {
        if (!PsLegalMoveMaskGen::inDanger(pieces, bsf(pieces.pieceBitboards[side][Pieces::King]), side) && !PsLegalMoveMaskGen::inDanger(pieces, 2 + index, side) && !PsLegalMoveMaskGen::inDanger(pieces, 3 + index, side))
            moves.push_back({ (uint8_t)(4 + index), (uint8_t)(2 + index), Pieces::King, side, 255, 255, MoveType::Quiet, longCastlingFlag });
    }

    if (shortCastling && getBit(pieces.pieceBitboards[side][Pieces::Rook], 7 + index) && getBit(pieces.empty, 5 + index) && getBit(pieces.empty, 6 + index))
    {
        if (!PsLegalMoveMaskGen::inDanger(pieces, bsf(pieces.pieceBitboards[side][Pieces::King]), side) && !PsLegalMoveMaskGen::inDanger(pieces, 5 + index, side) && !PsLegalMoveMaskGen::inDanger(pieces, 6 + index, side))
            moves.push_back({ (uint8_t)(4 + index), (uint8_t)(6 + index), Pieces::King, side, 255, 255, MoveType::Quiet, shortCastlingFlag });
    }
}