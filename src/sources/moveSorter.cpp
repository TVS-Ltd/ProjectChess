#include "moveSorter.h"

MoveList MoveSorter::quickSort(const Pieces& pieces, MoveList& moves, int start, int end)
{
    // base case
    if (start >= end)
    {
        return moves;
    }

    // partitioning the array
    int pivot = partition(pieces, moves, start, end);

    // Sorting the left part
    quickSort(pieces, moves, start, pivot - 1);

    // Sorting the right part
    quickSort(pieces, moves, pivot + 1, end);

    return moves;
}


int32_t MoveSorter::evaluateMove(const Pieces& pieces, Move& move)
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

    Move pivot = moves[start];

    size_t count = 0;

    for (int i = start + 1; i <= end; i++)
    {
        if (MoveSorter::evaluateMove(pieces, moves[i]) <= MoveSorter::evaluateMove(pieces, pivot))
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