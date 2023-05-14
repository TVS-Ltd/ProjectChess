#include <future>
#include <cmath>
//#include <unistd.h>
#include <chrono>
#include "position.h"
#include "moveList.h"
#include "transpositionTable.h"
#include "staticEvaluator.h"
#include "openingBook.h"
#include "moveSorter.h"
#include "constants.h"

#pragma once

enum class NodeType
{
    PV,
    NONPV
};

static atomic<bool> stopSearch;

static int64_t evaluated;
static int32_t cutOffs;
static int32_t aspirationWindow = 100;

static int node_count = 0;
static uint8_t AIside;

class AI
{
public:
    AI();
    AI(const string& openingBookPath);

    /**
    * Calculate the best AI move.
    *
    * @param position Current board position.
    * @param side AI side (Black/White).
    * @param minMs,maxMs Time bounds given for best move calculation.
    *
    * @return Best move.
    *
    * @exception This function does not throw exceptions.
    */
    Move bestMove(Position position, uint8_t side, int32_t minMs, int32_t maxMs);

    Move bestMoveCCG(Position position, uint8_t side, int32_t minMs, int32_t maxMs);

private:
    OpeningBook openingBook;

    static tuple<int32_t, Move> aspirationSearch(const Position& position, uint8_t side, int32_t depth, int32_t evaluation, TranspositionTable& TransposTable);

    static tuple<int32_t, Move> aspirationSearchCCG(const Position& position, uint8_t side, int32_t depth, int32_t evaluation, TranspositionTable& TransposTable);

    static tuple<int32_t, Move> searchRoot(Position position, uint8_t side, int32_t alpha, int32_t beta, int32_t depth_left, int32_t currentDepth, TranspositionTable& TransposTable);

    static tuple<int32_t, Move> searchRootCCG(Position position, uint8_t side, int32_t alpha, int32_t beta, int32_t depth_left, int32_t currentDepth, TranspositionTable& TransposTable);

    template<NodeType node_type>
    static int32_t search(Position position, uint8_t side, int32_t alpha, int32_t beta, int32_t depth_left, int32_t currentDepth, TranspositionTable& TransposTable) {
        node_count++;
        
        if (stopSearch)
            return alpha;

        if (depth_left <= 0)
        {
            return AI::quiescence(position, side, alpha, beta, currentDepth);
        }

        if (position.fiftyMovesCtr >= 50 or position.repetitionHistory.getRepetionNumber(position.hash) >= 3)
            return 0;

        MoveList moves = LegalMoveGen::generate(position, side);

        for (uint8_t i = 0; i < moves.size(); i++)
        {
            if (moves[i] == Variables::killers[currentDepth][0] || moves[i] == Variables::killers[currentDepth][1])
            {
                moves[i].type = MoveType::Killer;
            }
        }

        MoveSorter::quickSort(position.pieces, moves, 0, moves.size() - 1);

        Move move, bestMove = Constants::UnknownMove;

        bool in_check = PsLegalMoveMaskGen::inDanger(position.pieces, bsf(position.pieces.pieceBitboards[side][Pieces::King]), side);

        if (moves.size() == 0)
        {
            if (in_check)
            {
                int32_t score = Constants::Infinity::Positive - currentDepth;
                return (side == Pieces::White) ? -score : score;
            }
            return 0;
        }

        // null move pruning with verified search

        int32_t score;

        if (!in_check && position.allowNullMove)
        {
            position.allowNullMove = false;
            int32_t reductionDepth = (depth_left > 2) ? 3 : 2;
            score = -search<NodeType::NONPV>(position, Pieces::inverse(side), -beta, -beta + 1, depth_left - 1 - reductionDepth, currentDepth + 1, TransposTable);
            position.allowNullMove = true;

            if (score >= beta)
            {
                if (depth_left > 4)
                    score = search<NodeType::NONPV>(position, side, beta - 1, beta, depth_left - reductionDepth, currentDepth + 1, TransposTable);

                if (score >= beta)
                    return beta;
            }
        }
        
        Entry tt_result = TransposTable.tryToFindBestMove(position.hash);

        uint8_t i = 0;
        int32_t scoreType = Entry::UBound;

        Position copy = position;

        if (tt_result.Depth != -1)
        {
            bestMove = tt_result.BestMove;

            if (tt_result.Depth >= depth_left)
            {
                if (tt_result.Flag == Entry::Valid)
                    return tt_result.Score;
                else
                    if (tt_result.Flag == Entry::LBound)
                        alpha = max(alpha, tt_result.Score);
                    else
                        beta = min(beta, tt_result.Score);
            }
        }
        else
            bestMove = moves[i++];

        copy.move(bestMove);
        score = -search<node_type>(copy, Pieces::inverse(side), -beta, -alpha, depth_left - !in_check, currentDepth + 1, TransposTable);

        if (score >= beta)
        {
#pragma omp critical
            {
                if (bestMove.type == MoveType::Quiet && Variables::killers[currentDepth][0] != moves[i])
                {
                    Variables::killers[currentDepth][1] = Variables::killers[currentDepth][0];
                    Variables::killers[currentDepth][0] = bestMove;
                }
            }

            if (tt_result.Depth == -1 && !stopSearch)
                TransposTable.addEntry({ position.hash, bestMove, depth_left, beta, Entry::LBound });

            return beta;
        }
        if (score > alpha) {
            alpha = score;
            scoreType = Entry::Valid;
        }

        std::vector<int32_t> scores(moves.size(), -1);

        for (i; i < moves.size(); i = i + 1)
        {
            copy = position;
            copy.move(moves[i]);

            score = -search<node_type>(copy, Pieces::inverse(side), -alpha - 1, -alpha, depth_left - !in_check, currentDepth + 1, TransposTable);

            if (score > alpha)
                score = -search<NodeType::NONPV>(copy, Pieces::inverse(side), -beta, -alpha, depth_left - !in_check, currentDepth + 1, TransposTable);

            if (score >= beta)
            {
#pragma omp critical
                {
                    if (moves[i].type == MoveType::Quiet && Variables::killers[currentDepth][0] != moves[i])
                    {
                        Variables::killers[currentDepth][1] = Variables::killers[currentDepth][0];
                        Variables::killers[currentDepth][0] = moves[i];
                    }
                }

                if (!stopSearch && bestMove.type != MoveType::Unknown)
                    TransposTable.addEntry({ position.hash, bestMove, depth_left, beta, Entry::LBound });

                return beta;
            }

            if (score > alpha)
            {
                bestMove = moves[i];
                alpha = score;
                scoreType = Entry::Valid;
            }

            scores[i] = score;
        }

        if (!stopSearch && bestMove.type != MoveType::Unknown)
            TransposTable.addEntry({ position.hash, bestMove, depth_left, alpha, Entry::UBound });
        return alpha;
    }

    template<NodeType node_type>
    static int32_t searchCCG(Position position, uint8_t side, int32_t alpha, int32_t beta, int32_t depth_left, int32_t currentDepth, TranspositionTable& TransposTable) {
        node_count++;

        if (stopSearch)
            return alpha;

        if (depth_left <= 0)
        {
            return AI::quiescence(position, side, alpha, beta, currentDepth);
        }

        if (position.fiftyMovesCtr >= 50 or position.repetitionHistory.getRepetionNumber(position.hash) >= 3)
            return 0;

        MoveList moves = LegalMoveGen::generate(position, side);
        {
            MoveList cardMoves = LegalMoveGen::generateCardMoves(position, side);

            for (uint8_t i = 0; i < moves.size(); i++)
            {
                if (moves[i].type != MoveType::LayOutCard && (moves[i] == Variables::killers[currentDepth][0] || moves[i] == Variables::killers[currentDepth][1]))
                {
                    moves[i].type = MoveType::Killer;
                }
            }

            MoveSorter::quickSort(position.pieces, moves, 0, moves.size() - 1);
            MoveSorter::quickSort(position.pieces, cardMoves, 0, cardMoves.size() - 1);

            moves.unite(cardMoves);
        }

        Move move, bestMove = Constants::UnknownMove;

        bool in_check = PsLegalMoveMaskGen::inDanger(position.pieces, bsf(position.pieces.pieceBitboards[side][Pieces::King]), side);

        if (moves.size() == 0)
        {
            if (in_check)
            {
                int32_t score = Constants::Infinity::Positive - currentDepth;
                return (side == Pieces::White) ? -score : score;
            }
            return 0;
        }

        // null move pruning with verified search

        int32_t score;

        if (!in_check && position.allowNullMove)
        {
            position.allowNullMove = false;
            int32_t reductionDepth = (depth_left > 2) ? 3 : 2;
            score = -searchCCG<NodeType::NONPV>(position, Pieces::inverse(side), -beta, -beta + 1, depth_left - 1 - reductionDepth, currentDepth + 1, TransposTable);
            position.allowNullMove = true;

            if (score >= beta)
            {
                if (depth_left > 4)
                    score = searchCCG<NodeType::NONPV>(position, side, beta - 1, beta, depth_left - reductionDepth, currentDepth + 1, TransposTable);

                if (score >= beta)
                    return beta;
            }
        }

        int8_t points = 0;
        if (position.cardsNumber[side] > 0)
        {
            points = position.points[side] / position.cardsNumber[side];

            if (side == AIside)
            {
                std::vector<card> deck = position.AIdeck;

                uint8_t i = 0;
                while (pointsByFigure[deck[i].getFigure()] < points)
                    i++;

                position.cards[side].addCard(deck[i]);
                position.points[side] -= pointsByFigure[deck[i].getFigure()];
                position.AIdeck.erase(position.AIdeck.begin() + i);
            }
            else
            {
                std::string figureType;
                if (points >= 50) {
                    figureType = "Queen";
                }
                else if (points >= 30) {
                    figureType = "Knight";
                }
                else if (points >= 25) {
                    figureType = "Bishop";
                }
                else if (points >= 20) {
                    figureType = "Rook";
                }
                else if (points >= 10) {
                    figureType = "Pawn";
                }
                else {
                    return ((side == Pieces::White) ? Constants::Infinity::Negative : Constants::Infinity::Positive);
                }

                position.points[side] -= pointsByFigure[figureType];
                position.cards[side].addCard(card(figureType, "-", "-"));
            }
            position.cardsNumber[side]--;

        }

        Position copy = position;

        Entry tt_result = TransposTable.tryToFindBestMove(position.hash);

        uint8_t i = 0;
        int32_t scoreType = Entry::UBound;

        if (tt_result.Depth != -1)
        {
            bestMove = tt_result.BestMove;

            if (tt_result.Depth >= depth_left)
            {
                if (tt_result.Flag == Entry::Valid)
                    return tt_result.Score;
                else
                    if (tt_result.Flag == Entry::LBound)
                        alpha = max(alpha, tt_result.Score);
                    else
                        beta = min(beta, tt_result.Score);
            }
        }
        else
            bestMove = moves[i++];

        copy.move(bestMove);
        score = -searchCCG<node_type>(copy, Pieces::inverse(side), -beta, -alpha, depth_left - !in_check, currentDepth + 1, TransposTable);

        if (score >= beta)
        {
#pragma omp critical
            {
                if (bestMove.type == MoveType::Quiet && Variables::killers[currentDepth][0] != moves[i])
                {
                    Variables::killers[currentDepth][1] = Variables::killers[currentDepth][0];
                    Variables::killers[currentDepth][0] = bestMove;
                }
            }

            if (bestMove.type != MoveType::LayOutCard && tt_result.Depth == -1 && !stopSearch)
                TransposTable.addEntry({ position.hash, bestMove, depth_left, beta, Entry::LBound });

            return beta;
        }
        if (score > alpha) {
            alpha = score;
            scoreType = Entry::Valid;
        }

        std::vector<int32_t> scores(moves.size(), -1);

        for (i; i < moves.size(); i = i + 1)
        {
            copy = position;
            copy.move(moves[i]);

            score = -searchCCG<node_type>(copy, Pieces::inverse(side), -alpha - 1, -alpha, depth_left - !in_check, currentDepth + 1, TransposTable);

            if (score > alpha)
                score = -searchCCG<NodeType::NONPV>(copy, Pieces::inverse(side), -beta, -alpha, depth_left - !in_check, currentDepth + 1, TransposTable);

            if (score >= beta)
            {
#pragma omp critical
                {
                    if (moves[i].type == MoveType::Quiet && Variables::killers[currentDepth][0] != moves[i])
                    {
                        Variables::killers[currentDepth][1] = Variables::killers[currentDepth][0];
                        Variables::killers[currentDepth][0] = moves[i];
                    }
                }

                if (bestMove.type != MoveType::LayOutCard && bestMove.type != MoveType::Unknown && !stopSearch)
                    TransposTable.addEntry({ position.hash, bestMove, depth_left, beta, Entry::LBound });

                return beta;
            }

            if (score > alpha)
            {
                bestMove = moves[i];
                alpha = score;
                scoreType = Entry::Valid;
            }

            scores[i] = score;
        }

        if (bestMove.type != MoveType::LayOutCard && bestMove.type != MoveType::Unknown && !stopSearch)
            TransposTable.addEntry({ position.hash, bestMove, depth_left, alpha, Entry::UBound });
        return alpha;
    }
    
    static int32_t quiescence(Position position, uint8_t side, int32_t alpha, int32_t beta, int32_t currentDepth);
};