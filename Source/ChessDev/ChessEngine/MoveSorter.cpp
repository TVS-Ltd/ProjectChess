// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveSorter.h"

// MoveList MoveSorter::quickSort(Pieces pieces, MoveList moves, int start, int end)
// {
//     // base case
//     if (start >= end)
//     {
//         return moves;
//     }

//     // partitioning the array
//     int pivot = partition(pieces, moves, start, end);

//     // Sorting the left part
//     quickSort(pieces, moves, start, pivot - 1);

//     // Sorting the right part
//     quickSort(pieces, moves, pivot + 1, end);

//     return moves;
// }

// int32_t MoveSorter::evaluateMove(Pieces pieces, class ChessMove move)
// {
//     int32_t evaluation = 0;

//     if (move.AttackerType != Pieces::Pawn)
//     {
//         Bitboard opponent_pawn_attacks = PsLegalMoveMaskGen::generatePawnLeftCapturesMask(pieces, Pieces::inverse(move.AttackerSide), true) | PsLegalMoveMaskGen::generatePawnRightCapturesMask(pieces, Pieces::inverse(move.AttackerSide), true);

//         if (getBit(opponent_pawn_attacks, move.To))
//         {
//             switch (move.AttackerType)
//             {
//             case Pieces::Knight:
//                 evaluation = evaluation - StaticEvaluator::Material::Knight;
//                 break;

//             case Pieces::Bishop:
//                 evaluation = evaluation - StaticEvaluator::Material::Bishop;
//                 break;

//             case Pieces::Rook:
//                 evaluation = evaluation - StaticEvaluator::Material::Rook;
//                 break;

//             case Pieces::Queen:
//                 evaluation = evaluation - StaticEvaluator::Material::Queen;
//                 break;
//             }
//         }
//     }

//     if (move.DefenderType != 255)
//     {
//         switch (move.DefenderType)
//         {
//         case Pieces::Pawn:
//             evaluation = evaluation + 1000 * StaticEvaluator::Material::Pawn;
//             break;

//         case Pieces::Knight:
//             evaluation = evaluation + 1000 * StaticEvaluator::Material::Knight;
//             break;

//         case Pieces::Bishop:
//             evaluation = evaluation + 1000 * StaticEvaluator::Material::Bishop;
//             break;

//         case Pieces::Rook:
//             evaluation = evaluation + 1000 * StaticEvaluator::Material::Rook;
//             break;

//         case Pieces::Queen:
//             evaluation = evaluation + 1000 * StaticEvaluator::Material::Queen;
//             break;
//         }

//         switch (move.DefenderType)
//         {
//         case Pieces::Pawn:
//             evaluation = evaluation - StaticEvaluator::Material::Pawn;
//             break;

//         case Pieces::Knight:
//             evaluation = evaluation - StaticEvaluator::Material::Knight;
//             break;

//         case Pieces::Bishop:
//             evaluation = evaluation - StaticEvaluator::Material::Bishop;
//             break;

//         case Pieces::Rook:
//             evaluation = evaluation - StaticEvaluator::Material::Rook;
//             break;

//         case Pieces::Queen:
//             evaluation = evaluation - StaticEvaluator::Material::Queen;
//             break;
//         }
//     }

//     return evaluation;
// }

// size_t MoveSorter::partition(Pieces pieces, MoveList moves, int start, int end)
// {

//     class ChessMove pivot = moves[start];

//     size_t count = 0;

//     for (int i = start + 1; i <= end; i++) 
//     {
//         if (MoveSorter::evaluateMove(pieces, moves[i]) <= MoveSorter::evaluateMove(pieces, pivot))
//         {
//             count++;
//         }
//     }

//     // Giving pivot element its correct position
//     size_t pivotIndex = start + count;
//     swap(moves[pivotIndex], moves[start]);

//     // Sorting left and right parts of the pivot element
//     size_t i = start, j = end;

//     while (i < pivotIndex && j > pivotIndex) 
//     {

//         while (MoveSorter::evaluateMove(pieces, moves[i]) <= MoveSorter::evaluateMove(pieces, pivot)) 
//         {
//             i++;
//         }

//         while (MoveSorter::evaluateMove(pieces, moves[j]) > MoveSorter::evaluateMove(pieces, pivot)) 
//         {
//             j--;
//         }

//         if (i < pivotIndex && j > pivotIndex) 
//         {
//             swap(moves[i++], moves[j--]);
//         }
//     }

//     return pivotIndex;
// }



void MoveSorter::quickSort(const Pieces& pieces, MoveList& moves, int start, int end)
{
    // base case
    if (start >= end)
    {
        return;
    }

    // partitioning the array
    int pivot = partition(pieces, moves, start, end);

    // Sorting the left part
    quickSort(pieces, moves, start, pivot - 1);

    // Sorting the right part
    quickSort(pieces, moves, pivot + 1, end);
}

int32_t MoveSorter::evaluateMove(const Pieces& pieces, ChessMove& move)
{
    int32_t evaluation = 0;

    if (move.AttackerType != Pieces::Pawn)
    {
        Bitboard opponent_pawn_attacks = PsLegalMoveMaskGen::generatePawnLeftCapturesMask(pieces, Pieces::inverse(move.AttackerSide), true) | PsLegalMoveMaskGen::generatePawnRightCapturesMask(pieces, Pieces::inverse(move.AttackerSide), true);

        if (getBit(opponent_pawn_attacks, move.To))
        {
            switch (move.AttackerType)
            {
            case Pieces::Knight:
                evaluation = evaluation - StaticEvaluator::Material::Knight;
                break;

            case Pieces::Bishop:
                evaluation = evaluation - StaticEvaluator::Material::Bishop;
                break;

            case Pieces::Rook:
                evaluation = evaluation - StaticEvaluator::Material::Rook;
                break;

            case Pieces::Queen:
                evaluation = evaluation - StaticEvaluator::Material::Queen;
                break;
            }
        }
    }

    if (move.From == 255) {
        if (move.AttackerType == Pieces::Pawn) {
        }
        else if (move.AttackerType == Pieces::Knight) {
            evaluation += StaticEvaluator::Mobility::Knight * countOnes(PsLegalMoveMaskGen::generateKnightMask(pieces, move.To, move.AttackerSide, false));
        }
        else if (move.AttackerType == Pieces::Bishop) {
            evaluation += StaticEvaluator::Mobility::Bishop * countOnes(PsLegalMoveMaskGen::generateBishopMask(pieces, move.To, move.AttackerSide, false));
        }
        else if (move.AttackerType == Pieces::Queen) {
            evaluation += StaticEvaluator::Mobility::Queen * countOnes(PsLegalMoveMaskGen::generateQueenMask(pieces, move.To, move.AttackerSide, false));
        }
        else if (move.AttackerType == Pieces::Rook) {
            evaluation += StaticEvaluator::Mobility::Rook * countOnes(PsLegalMoveMaskGen::generateRookMask(pieces, move.To, move.AttackerSide, false));
        }

        Pieces copy = pieces;
        setOne(copy.pieceBitboards[move.AttackerSide][move.AttackerType], move.To);

        bool in_check = PsLegalMoveMaskGen::inDanger(copy, bsf(pieces.pieceBitboards[Pieces::inverse(move.AttackerSide)][Pieces::King]), Pieces::inverse(move.AttackerSide));
        if (in_check)
            evaluation += 300;

        return evaluation;
    }

    if (move.DefenderType != 255)
    {
        switch (move.DefenderType)
        {
        case Pieces::Pawn:
            evaluation = evaluation + 1000 * StaticEvaluator::Material::Pawn;
            break;

        case Pieces::Knight:
            evaluation = evaluation + 1000 * StaticEvaluator::Material::Knight;
            break;

        case Pieces::Bishop:
            evaluation = evaluation + 1000 * StaticEvaluator::Material::Bishop;
            break;

        case Pieces::Rook:
            evaluation = evaluation + 1000 * StaticEvaluator::Material::Rook;
            break;

        case Pieces::Queen:
            evaluation = evaluation + 1000 * StaticEvaluator::Material::Queen;
            break;
        }

        switch (move.AttackerType)
        {
        case Pieces::Pawn:
            evaluation = evaluation - StaticEvaluator::Material::Pawn;
            break;

        case Pieces::Knight:
            evaluation = evaluation - StaticEvaluator::Material::Knight;
            break;

        case Pieces::Bishop:
            evaluation = evaluation - StaticEvaluator::Material::Bishop;
            break;

        case Pieces::Rook:
            evaluation = evaluation - StaticEvaluator::Material::Rook;
            break;

        case Pieces::Queen:
            evaluation = evaluation - StaticEvaluator::Material::Queen;
            break;
        }
    }

    return evaluation;
}

size_t MoveSorter::partition(const Pieces& pieces, MoveList& moves, int start, int end)
{
    ChessMove pivot = moves[start];

    size_t count = 0;

    for (int i = start + 1; i <= end; i++)
    {
        if ((moves[i].type == MoveType::Quiet && pivot.type == MoveType::Killer) || (MoveSorter::evaluateMove(pieces, moves[i]) <= MoveSorter::evaluateMove(pieces, pivot)))
        {
            count++;
        }
    }

    // Giving pivot element its correct position
    size_t pivotIndex = start + count;
    swap(moves[pivotIndex], moves[start]);

    // Sorting left and right parts of the pivot element
    size_t i = start, j = end;

    while (i < pivotIndex && j > pivotIndex)
    {

        while (MoveSorter::evaluateMove(pieces, moves[i]) <= MoveSorter::evaluateMove(pieces, pivot))
        {
            i++;
        }

        while (MoveSorter::evaluateMove(pieces, moves[j]) > MoveSorter::evaluateMove(pieces, pivot))
        {
            j--;
        }

        if (i < pivotIndex && j > pivotIndex)
        {
            swap(moves[i++], moves[j--]);
        }
    }

    return pivotIndex;
}