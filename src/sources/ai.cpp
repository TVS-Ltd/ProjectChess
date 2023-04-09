#include "ai.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#define nsecs std::chrono::high_resolution_clock::now().time_since_epoch().count()

static atomic<bool> stopSearch;

static int64_t evaluated;
static int32_t maxDepth;
static int32_t cutOffs;
static int32_t width = 10000;

using namespace std;

void usleep(__int64 usec)
{
    HANDLE timer;
    LARGE_INTEGER ft;

    ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}

AI::AI() = default;
AI::AI(const string& openingBookPath)
{
    this->openingBook = { openingBookPath };
}

Move AI::bestMove(const Position& position, uint8_t side, int32_t minMs, int32_t maxMs)
{
    std::cout << endl;

    std::cout << "Thinking..." << endl;

    bool debug = false;

#if DEBUG
    debug = true;
#endif

    StaticEvaluator::evaluate(position.pieces, position.WhiteLongCastling, position.WhiteShortCastling, position.BlackLongCastling, position.BlackShortCastling, position.whiteCastlingHappened, position.blackCastlingHappened, debug);

    int64_t timeStart = nsecs;
    stopSearch = false;


    tuple<Move, int32_t> openingBookResult = this->openingBook.tryToFindMove(position);

#if DEBUG
    cout << "\033[103m" << "Number of available moves in the opening book: " << get<1>(openingBookResult) << "." << "\033[0m" << endl;
#endif

    if (get<1>(openingBookResult))
    {
        usleep(max((int64_t)0, (minMs - (nsecs - timeStart) / (int64_t)1e+6) * (int64_t)1e+3));

        return get<0>(openingBookResult);
    }

    std::cout << "\033[93m" << "Search started." << "\033[0m" << endl;

    int32_t bestMoveEvaluation = 0;
    Move bestMove;

    future<tuple<int32_t, Move>> bestMoveThread;

    bool updateBestMove;

    for (int32_t i = 1; i < 1e+3 && !stopSearch; i = i + 1)
    {
        evaluated = 0;
        maxDepth = 0;
        cutOffs = 0;

        bestMoveThread = async(AI::BestMove, position, side, i, bestMoveEvaluation, ref(TransposTable));

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
        cout << "Depth: "<< i << "\nEval: " << bestMoveEvaluation << '\n';
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


tuple<int32_t, Move> AI::BestMove(const Position& position, uint8_t side, int32_t depth, int32_t evaluation, TranspositionTable& TransposTable)
{
    tuple<int32_t, Move> score;
    int32_t L = evaluation - width, R = evaluation + width;
    if (side == Pieces::White)
    {
        score = AI::alphaBetaMax(position, L, R, depth, 0, TransposTable);
        if (get<0>(score) < L)
            score = AI::alphaBetaMax(position, AI::Infinity::Negative, L, depth, 0, TransposTable);
        else
            if (get<0>(score) > R)
                score = AI::alphaBetaMax(position, R, AI::Infinity::Positive, depth, 0, TransposTable);
        return score;
    }

    score = AI::alphaBetaMin(position, AI::Infinity::Negative, AI::Infinity::Positive, depth, 0, TransposTable);
    if (get<0>(score) < L)
        score = AI::alphaBetaMin(position, AI::Infinity::Negative, L, depth, 0, TransposTable);
    else
        if (get<0>(score) > R)
            score = AI::alphaBetaMin(position, R, AI::Infinity::Positive, depth, 0, TransposTable);
    return score;
}

tuple<int32_t, Move> AI::alphaBetaMin(Position position, int32_t alpha, int32_t beta, int32_t depth_left, int32_t currentDepth, TranspositionTable& TransposTable)
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

    Move move, bestMove;
    bool moveWasFound = false;

    bool in_check = PsLegalMoveMaskGen::inDanger(position.pieces, bsf(position.pieces.pieceBitboards[Pieces::Black][Pieces::King]), Pieces::Black);

    if (moves.size() == 0)
    {
        if (in_check)
            return make_tuple(AI::Infinity::Positive - currentDepth, Move());
        return std::make_tuple(0, Move());
    }

    int32_t evaluation, score;

    Position copy = position;

    Entry tt_result = TransposTable.tryToFindBestMove(position.hash);

    uint8_t i = 0;

    if (tt_result.Depth != -1) {
        move = tt_result.BestMove;

        if (tt_result.Depth >= depth_left)
        {
            if (tt_result.Flag == Entry::Valid)
                return make_tuple(tt_result.Score, tt_result.BestMove);
            else
                if (tt_result.Flag == Entry::LBound)
                    alpha = max(alpha, tt_result.Score);
                else
                    beta = min(beta, tt_result.Score);
        }
    }
    else 
        move = moves[i++];

    copy.move(move);
    score = get<0>(AI::alphaBetaMax(copy, alpha, beta, depth_left - !in_check, currentDepth + 1, TransposTable));

    if (score <= alpha)
    {
        if (tt_result.Depth == -1 && !stopSearch)
            TransposTable.addEntry({ position.hash, move, depth_left, alpha, Entry::UBound });
        
        return make_tuple(alpha, move);
    }
    else 
        score = AI::Infinity::Positive;

    for (i; i < moves.size(); i = i + 1)
    {
        copy = position;
        copy.move(moves[i]);

        evaluation = get<0>(AI::alphaBetaMax(copy, alpha, alpha + 1, depth_left - !in_check, currentDepth + 1, TransposTable));

        if (evaluation > alpha && evaluation < beta) {
            evaluation = get<0>(AI::alphaBetaMax(copy, alpha, beta, depth_left - !in_check, currentDepth + 1, TransposTable));
        }

        if (evaluation <= alpha)
        {
            if (!moveWasFound)  
                return make_tuple(AI::alphaBetaMinOnlyCaptures(position, alpha, beta, currentDepth), Move());

            if (!stopSearch)
                TransposTable.addEntry({ position.hash, bestMove, depth_left, alpha, Entry::UBound });
            return make_tuple(alpha, bestMove);
        }

        score = min(score, evaluation);

        if (evaluation < beta)
        {
            bestMove = moves[i];
            moveWasFound = true;
            beta = evaluation;
        }

    }
    if (!moveWasFound) 
        return make_tuple(AI::alphaBetaMinOnlyCaptures(position, alpha, beta, currentDepth), Move());

    if (score > alpha && score < beta) {
        if (!stopSearch)
            TransposTable.addEntry({ position.hash, bestMove, depth_left, score, Entry::Valid });
        return std::make_tuple(beta, bestMove);
    }
    if (!stopSearch)
        TransposTable.addEntry({ position.hash, bestMove, depth_left, beta, Entry::LBound });
    return std::make_tuple(beta, bestMove);
}
tuple<int32_t, Move> AI::alphaBetaMax(Position position, int32_t alpha, int32_t beta, int32_t depth_left, int32_t currentDepth, TranspositionTable& TransposTable)
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

    Move move, bestMove;
    bool moveWasFound = false;

    bool in_check = PsLegalMoveMaskGen::inDanger(position.pieces, bsf(position.pieces.pieceBitboards[Pieces::White][Pieces::King]), Pieces::White);

    if (moves.size() == 0)
    {
        if (in_check)
            return std::make_tuple(AI::Infinity::Negative + currentDepth, Move());
        return std::make_tuple(0, Move());
    }

    int32_t evaluation, score;

    Position copy = position;

    Entry tt_result = TransposTable.tryToFindBestMove(position.hash);

    uint8_t i = 0;

    if (tt_result.Depth != -1)
    {
        move = tt_result.BestMove;

        if (tt_result.Depth >= depth_left)
        {
            if (tt_result.Flag == Entry::Valid)
                return make_tuple(tt_result.Score, tt_result.BestMove);
            else
                if (tt_result.Flag == Entry::LBound)
                    alpha = max(alpha, tt_result.Score);
                else
                    beta = min(beta, tt_result.Score);
        }
    }
    else 
        move = moves[i++];

    copy.move(move);
    score = get<0>(AI::alphaBetaMin(copy, beta - 1, beta, depth_left - !in_check, currentDepth + 1, TransposTable));

    if (score >= beta)
    {
        if (tt_result.Depth == -1 && !stopSearch)
            TransposTable.addEntry({ position.hash, move, depth_left, beta, Entry::LBound });

        return make_tuple(beta, move);
    }
    else 
        score = AI::Infinity::Negative;

    for (i; i < moves.size(); i = i + 1)
    {
        copy = position;
        copy.move(moves[i]);

        evaluation = get<0>(AI::alphaBetaMin(copy, beta - 1, beta, depth_left - !in_check, currentDepth + 1, TransposTable));

        if (evaluation > alpha && evaluation < beta)
            evaluation = get<0>(AI::alphaBetaMin(copy, alpha, beta, depth_left - !in_check, currentDepth + 1, TransposTable));

        if (evaluation >= beta)
        {
            if (!moveWasFound) 
                return make_tuple(AI::alphaBetaMaxOnlyCaptures(position, alpha, beta, currentDepth), Move());

            if (!stopSearch)
                TransposTable.addEntry({ position.hash, bestMove, depth_left, beta, Entry::LBound });
            
            return make_tuple(beta, bestMove);
        }

        score = max(score, evaluation);

        if (evaluation > alpha)
        {
            bestMove = moves[i];
            moveWasFound = true;
            alpha = evaluation;
        }
    }
    if (!moveWasFound) 
        return make_tuple(AI::alphaBetaMaxOnlyCaptures(position, alpha, beta, currentDepth), Move());

    if (score > alpha && score < beta) {
        if (!stopSearch)
            TransposTable.addEntry({ position.hash, bestMove, depth_left, score, Entry::Valid });
        return make_tuple(score, bestMove);
    }
    if (!stopSearch)
        TransposTable.addEntry({ position.hash, bestMove, depth_left, alpha, Entry::UBound });
    return make_tuple(alpha, bestMove);
}

int32_t AI::alphaBetaMinOnlyCaptures(const Position& position, int32_t alpha, int32_t beta, int32_t currentDepth)
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

    //if (currentDepth > 15) return evaluation;


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
        evaluation = AI::alphaBetaMaxOnlyCaptures(copy, alpha, alpha + 1, currentDepth + 1);

        if (evaluation > alpha && evaluation < beta)
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

int32_t AI::alphaBetaMaxOnlyCaptures(const Position& position, int32_t alpha, int32_t beta, int32_t currentDepth)
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

    //if (currentDepth > 15) return evaluation;

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

    evaluation = AI::alphaBetaMinOnlyCaptures(copy, alpha, beta, currentDepth + 1);

    if (evaluation >= beta)
    {
        return beta;
    }

    if (evaluation > alpha)
    {
        alpha = evaluation;
    }

    for (uint8_t i = 1; i < moves.size(); i++)
    {
        move = moves[i];

        copy = position;
        copy.move(move);

        evaluation = AI::alphaBetaMinOnlyCaptures(copy, beta - 1, beta, currentDepth + 1);

        if (evaluation > alpha && evaluation < beta)
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
