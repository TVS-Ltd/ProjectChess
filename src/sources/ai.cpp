#include "ai.h"

#define nsecs std::chrono::high_resolution_clock::now().time_since_epoch().count()

static atomic<bool> stopSearch;

static int64_t evaluated;
static int32_t maxDepth;
static int32_t cutOffs;

    AI::AI() = default;
    AI::AI(const string &openingBookPath)
    {
        this->openingBook = {openingBookPath};
    }

    Move AI::bestMove(const Position &position, uint8_t side, int32_t minMs, int32_t maxMs)
    {
        cout << endl;

        cout << "Thinking..." << endl;

        bool debug = false;

#if DEBUG
        debug = true;
#endif

        StaticEvaluator::evaluate(position.pieces, position.WhiteLongCastling, position.WhiteShortCastling, position.BlackLongCastling, position.BlackShortCastling, position.whiteCastlingHappened, position.blackCastlingHappened, debug);

        int64_t timeStart = nsecs;
        stopSearch = false;
        TranspositionTable TransposTable;

        tuple<Move, int32_t> openingBookResult = this->openingBook.tryToFindMove(position);

#if DEBUG
        cout << "\033[103m" << "Number of available moves in the opening book: " << get<1>(openingBookResult) << "." << "\033[0m" << endl;
#endif

        if (get<1>(openingBookResult))
        {
            usleep(max((int64_t)0, (minMs - (nsecs - timeStart) / (int64_t)1e+6) * (int64_t)1e+3));

            return get<0>(openingBookResult);
        }

        cout << "\033[93m" << "Search started." << "\033[0m" << endl;

        int32_t bestMoveEvaluation;
        Move bestMove;

        future<tuple<int32_t, Move>> bestMoveThread;

        bool updateBestMove;

        for (int32_t i = 1; i < 1e+3; i = i + 1)
        {
            evaluated = 0;
            maxDepth = 0;
            cutOffs = 0;

            bestMoveThread = async(AI::BestMove, position, side, i, ref(TransposTable));

            updateBestMove = true;

            while (bestMoveThread.wait_for(chrono::seconds(0)) != future_status::ready)
            {
                if ((nsecs - timeStart) / (int32_t)1e+6 >= maxMs)
                {
                    updateBestMove = false;
                    break;
                }

                usleep(20000);
            }

            if (updateBestMove || i == 1)
                tie(bestMoveEvaluation, bestMove) = bestMoveThread.get();
            else
            {
                stopSearch = true;
                break;
            }

#if DEBUG
            cout << "Base depth: " << setw(4) << i << "." << setw(21) << " Maximal depth: " << setw(4) << maxDepth << "." << setw(18) << " Evaluation: " << setw(6) << (float)bestMoveEvaluation / 100.0f << " pawns." << setw(34) << " Evaluated (this iteration): " << setw(10) << evaluated << "." << setw(51) << "Transposition table cutoffs (this iteration): " << setw(10) << cutOffs << "." << setw(25) << "Time (full search): " << setw(10) << (nsecs - timeStart) / (int32_t)1e+6 << " ms." << endl;
#endif

            if (bestMoveEvaluation > AI::Infinity::Positive - 2000 || bestMoveEvaluation < AI::Infinity::Negative + 2000)
                break;
        }

        usleep(max((int64_t)0, (minMs - (nsecs - timeStart) / (int64_t)1e+6) * (int64_t)1e+3));

        cout << "\033[92m" << "Search finished." << "\033[0m" << endl;

        return bestMove;
    }

    OpeningBook openingBook;

    tuple<int32_t, Move> AI::BestMove(const Position &position, uint8_t side, int32_t depth, TranspositionTable &TransposTable)
    {
        if (side == Pieces::White)
            return AI::alphaBetaMax(position, AI::Infinity::Negative, AI::Infinity::Positive, depth, 0, TransposTable);

        return AI::alphaBetaMin(position, AI::Infinity::Negative, AI::Infinity::Positive, depth, 0, TransposTable);
    }

    tuple<int32_t, Move> AI::alphaBetaMin(Position position, int32_t alpha, int32_t beta, int32_t depth_left, int32_t currentDepth, TranspositionTable &TransposTable)
    {
        if (stopSearch)
            return make_tuple(0, Move());
        if (currentDepth > maxDepth)
            maxDepth = currentDepth;

        if (depth_left == 0)
            return make_tuple(AI::alphaBetaMinOnlyCaptures(position, alpha, beta, currentDepth), Move());

        if (position.fiftyMovesCtr >= 50 or position.repetitionHistory.getRepetionNumber(position.hash) >= 3)
            return make_tuple(0, Move());

        MoveList moves = LegalMoveGen::generate(position, Pieces::Black);
        moves = MoveSorter::quickSort(position.pieces, moves, 0, moves.size() - 1);

        Move move;
        Move bestMove;
        uint8_t bestMoveIndex;

        bool in_check = PsLegalMoveMaskGen::inDanger(position.pieces, bsf(position.pieces.pieceBitboards[Pieces::Black][Pieces::King]), Pieces::Black);

        if (moves.size() == 0)
        {
            if (in_check)
                return make_tuple(AI::Infinity::Positive - currentDepth, Move());
            return std::make_tuple(0, Move());
        }

        int32_t evaluation;

        Position copy;

        uint8_t tt_result = TransposTable.tryToFindBestMoveIndex(position.hash);

        for (uint8_t i = 0; i < moves.size(); i = i + 1)
        {
            if (tt_result >= moves.size())
                move = moves[i];
            else
            {
                if (i == 0)
                    move = moves[tt_result];
                else
                {
                    if (i == tt_result)
                        move = moves[0];
                    else
                        move = moves[i];
                }
            }

            copy = position;
            copy.move(move);
            evaluation = get<0>(AI::alphaBetaMax(copy, alpha, beta, depth_left - !in_check, currentDepth + 1, TransposTable));

            if (evaluation <= alpha)
            {
                if (tt_result >= moves.size() or i != 0)
                    TransposTable.addEntry({position.hash, depth_left, bestMoveIndex});
                else
                    cutOffs = cutOffs + 1;
                return make_tuple(alpha, bestMove);
            }

            if (evaluation < beta)
            {
                bestMove = move;
                bestMoveIndex = i;
                beta = evaluation;
            }
        }

        TransposTable.addEntry({position.hash, depth_left, bestMoveIndex});
        return std::make_tuple(beta, bestMove);
    }
    tuple<int32_t, Move> AI::alphaBetaMax(Position position, int32_t alpha, int32_t beta, int32_t depth_left, int32_t currentDepth, TranspositionTable &TransposTable)
    {
        if (stopSearch)
            return std::make_tuple(0, Move());
        if (currentDepth > maxDepth)
            maxDepth = currentDepth;

        if (depth_left == 0)
            return std::make_tuple(AI::alphaBetaMaxOnlyCaptures(position, alpha, beta, currentDepth), Move());

        if (position.fiftyMovesCtr >= 50 or position.repetitionHistory.getRepetionNumber(position.hash) >= 3)
            return std::make_tuple(0, Move());

        MoveList moves = LegalMoveGen::generate(position, Pieces::White);
        moves = MoveSorter::quickSort(position.pieces, moves, 0, moves.size() - 1);
        Move move;
        Move bestMove;
        uint8_t bestMoveIndex;

        bool in_check = PsLegalMoveMaskGen::inDanger(position.pieces, bsf(position.pieces.pieceBitboards[Pieces::White][Pieces::King]), Pieces::White);

        if (moves.size() == 0)
        {
            if (in_check)
                return std::make_tuple(AI::Infinity::Negative + currentDepth, Move());
            return std::make_tuple(0, Move());
        }

        int32_t evaluation;

        Position copy;

        uint8_t tt_result = TransposTable.tryToFindBestMoveIndex(position.hash);

        for (uint8_t i = 0; i < moves.size(); i = i + 1)
        {
            if (tt_result >= moves.size())
                move = moves[i];
            else
            {
                if (i == 0)
                    move = moves[tt_result];
                else
                {
                    if (i == tt_result)
                        move = moves[0];
                    else
                        move = moves[i];
                }
            }

            copy = position;
            copy.move(move);
            evaluation = get<0>(AI::alphaBetaMin(copy, alpha, beta, depth_left - !in_check, currentDepth + 1, TransposTable));

            if (evaluation >= beta)
            {
                if (tt_result >= moves.size() or i != 0)
                    TransposTable.addEntry({position.hash, depth_left, bestMoveIndex});
                else
                    cutOffs = cutOffs + 1;
                return make_tuple(beta, bestMove);
            }

            if (evaluation > alpha)
            {
                bestMove = move;
                bestMoveIndex = i;
                alpha = evaluation;
            }
        }

        TransposTable.addEntry({position.hash, depth_left, bestMoveIndex});
        return make_tuple(alpha, bestMove);
    }

    int32_t AI::alphaBetaMinOnlyCaptures(const Position &position, int32_t alpha, int32_t beta, int32_t currentDepth)
    {
        if (stopSearch)
        {
            return 0;
        }

        if (currentDepth > maxDepth)
        {
            maxDepth = currentDepth;
        }

        int32_t evaluation = StaticEvaluator::evaluate(position.pieces, position.WhiteLongCastling, position.WhiteShortCastling, position.BlackLongCastling, position.BlackShortCastling, position.whiteCastlingHappened, position.blackCastlingHappened);
        evaluated++;

        if (evaluation <= alpha)
        {
            return alpha;
        }

        if (evaluation < beta)
        {
            beta = evaluation;
        }

        MoveList moves = LegalMoveGen::generate(position, Pieces::Black, true);
        moves = MoveSorter::quickSort(position.pieces, moves, 0, moves.size() - 1);
        Move move;

        Position copy;

        for (uint8_t i = 0; i < moves.size(); i++)
        {
            move = moves[i];

            copy = position;
            copy.move(move);
            evaluation = AI::alphaBetaMaxOnlyCaptures(copy, alpha, beta, currentDepth + 1);

            if (evaluation <= alpha)
            {
                return alpha;
            }

            if (evaluation < beta)
            {
                beta = evaluation;
            }
        }

        return beta;
    }

    int32_t AI::alphaBetaMaxOnlyCaptures(const Position &position, int32_t alpha, int32_t beta, int32_t currentDepth)
    {
        if (stopSearch)
        {
            return 0;
        }

        if (currentDepth > maxDepth)
        {
            maxDepth = currentDepth;
        }

        int32_t evaluation = StaticEvaluator::evaluate(position.pieces, position.WhiteLongCastling, position.WhiteShortCastling, position.BlackLongCastling, position.BlackShortCastling, position.whiteCastlingHappened, position.blackCastlingHappened);
        evaluated++;

        if (evaluation >= beta)
        {
            return beta;
        }

        if (evaluation > alpha)
        {
            alpha = evaluation;
        }

        MoveList moves = LegalMoveGen::generate(position, Pieces::White, true);
        moves = MoveSorter::quickSort(position.pieces, moves, 0, moves.size() - 1);
        Move move;

        Position copy;

        for (uint8_t i = 0; i < moves.size(); i++)
        {
            move = moves[i];

            copy = position;
            copy.move(move);

            evaluation = AI::alphaBetaMinOnlyCaptures(copy, alpha, beta, currentDepth + 1);

            if (evaluation >= beta)
            {
                return beta;
            }

            if (evaluation > alpha)
            {
                alpha = evaluation;
            }
        }

        return alpha;
    }
