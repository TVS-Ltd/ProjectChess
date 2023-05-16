#include "ai.h"
#include <omp.h>

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#define nsecs std::chrono::high_resolution_clock::now().time_since_epoch().count()

using namespace std;


double timex = 0;
int64_t countx = 0;


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

    for (int8_t i = 0; i < Constants::MaximalDepth; i++) {
        Variables::killers[i][0] = Variables::killers[i][1] = Constants::UnknownMove;
        for (int8_t j = 0; j < 6; j++)
            Variables::killersRoyal[i][0][j] = Variables::killersRoyal[i][1][j] = Constants::UnknownMove;
    }
}
Move AI::bestMove(Position position, uint8_t side, int32_t minMs, int32_t maxMs)
{
    for (int8_t i = 0; i < Constants::MaximalDepth; i++) {
        Variables::killers[i][0] = Variables::killers[i][1] = Constants::UnknownMove;
    }

    node_count = 0;
    std::cout << endl;

    std::cout << "Thinking..." << endl;

    bool debug = false;

#if DEBUG
    debug = true;
#endif

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
    TranspositionTable TransposTable;

    bool updateBestMove;

    for (int i = 1; i < 7; i = i + 1)
    {
        evaluated = 0;
        cutOffs = 0;

        bestMoveThread = async(AI::aspirationSearch, position, side, i, bestMoveEvaluation, ref(TransposTable));

        //updateBestMove = true;
        /*while (bestMoveThread.wait_for(chrono::seconds(0)) != future_status::ready)
        {
            if ((nsecs - timeStart) / (int32_t)1e+6 >= maxMs)
            {
                updateBestMove = false;
                break;
            }
            usleep(20000);
        }*/
        //if (updateBestMove || i == 1)
        {
            auto score = bestMoveThread.get();
            if (get<1>(score) != Constants::UnknownMove)
                tie(bestMoveEvaluation, bestMove) = score;
        }
        /*else
        {
            stopSearch = true;
            break;
        }*/


        //cout << (int)bestMove.From << "->" << (int)bestMove.To << '\n';
#if DEBUG
        cout << "Depth: " << i << "\nEval: " << bestMoveEvaluation << '\n';
        cout << "Base depth: " << setw(4) << i << "." << setw(21) << " Maximal depth: " << setw(4) << maxDepth << "." << setw(18) << " Evaluation: " << setw(6) << (float)bestMoveEvaluation / 100.0f << " pawns." << setw(34) << " Evaluated (this iteration): " << setw(10) << evaluated << "." << setw(51) << "Transposition table cutoffs (this iteration): " << setw(10) << cutOffs << "." << setw(25) << "Time (full search): " << setw(10) << (nsecs - timeStart) / (int32_t)1e+6 << " ms." << endl;
#endif
    }

    usleep(max((int64_t)0, (minMs - (nsecs - timeStart) / (int64_t)1e+6) * (int64_t)1e+3));

    cout << "\033[92m" << "Search finished." << "\033[0m" << endl;

    return bestMove;
}

Move AI::bestMoveCCG(Position position, uint8_t side, int32_t minMs, int32_t maxMs)
{
    node_count = 0;
    std::cout << endl;

    std::cout << "Thinking..." << endl;

    bool debug = false;

#if DEBUG
    debug = true;
#endif

    int64_t timeStart = nsecs;
    stopSearch = false;

    std::cout << "\033[93m" << "Search started." << "\033[0m" << endl;

    int32_t bestMoveEvaluation = 0;
    Move bestMove;

    future<tuple<int32_t, Move>> bestMoveThread;
    TranspositionTable TransposTable;

    bool updateBestMove;
    AIside = side;

    for (int i = 1; i < 5; i = i + 1)
    {
        evaluated = 0;
        cutOffs = 0;

        bestMoveThread = async(AI::aspirationSearchCCG, position, side, i, bestMoveEvaluation, ref(TransposTable));

        //updateBestMove = true;
        /*while (bestMoveThread.wait_for(chrono::seconds(0)) != future_status::ready)
        {
            if ((nsecs - timeStart) / (int32_t)1e+6 >= maxMs)
            {
                updateBestMove = false;
                break;
            }
            usleep(20000);
        }*/
        //if (updateBestMove || i == 1)
        {
            auto score = bestMoveThread.get();
            if (get<1>(score) != Constants::UnknownMove)
                tie(bestMoveEvaluation, bestMove) = score;
        }
        /*else
        {
            stopSearch = true;
            break;
        }*/


        //cout << (int)bestMove.From << "->" << (int)bestMove.To << '\n';
#if DEBUG
        cout << "Depth: " << i << "\nEval: " << bestMoveEvaluation << '\n';
        cout << "Base depth: " << setw(4) << i << "." << setw(21) << " Maximal depth: " << setw(4) << maxDepth << "." << setw(18) << " Evaluation: " << setw(6) << (float)bestMoveEvaluation / 100.0f << " pawns." << setw(34) << " Evaluated (this iteration): " << setw(10) << evaluated << "." << setw(51) << "Transposition table cutoffs (this iteration): " << setw(10) << cutOffs << "." << setw(25) << "Time (full search): " << setw(10) << (nsecs - timeStart) / (int32_t)1e+6 << " ms." << endl;
#endif
    }

    usleep(max((int64_t)0, (minMs - (nsecs - timeStart) / (int64_t)1e+6) * (int64_t)1e+3));

    cout << "\033[92m" << "Search finished." << "\033[0m" << endl;

    return bestMove;
}

Move AI::bestMoveRoyal(Position position, uint8_t side, int32_t minMs, int32_t maxMs)
{
    AIside = side;

    handsdeck cur = position.cards[AIside];
    while (!cur.checkIsEmpty())
    {
        std::cout << cur.getCard(0) << ' ';
    }
    std::cout << '\n';

    node_count = 0;
    std::cout << endl;

    std::cout << "Thinking..." << endl;

    bool debug = false;

#if DEBUG
    debug = true;
#endif

    int64_t timeStart = nsecs;
    stopSearch = false;

    std::cout << "\033[93m" << "Search started." << "\033[0m" << endl;

    int32_t bestMoveEvaluation = 0;
    Move bestMove = Constants::UnknownMove;

    future<tuple<int32_t, Move>> bestMoveThread;
    TranspositionTable TransposTable;

    bool updateBestMove;
    AIside = side;

    for (int i = 1; i < 4; i = i + 1)
    {
        //std::cout << i << " ::: \n";

        evaluated = 0;
        cutOffs = 0;

        bestMoveThread = async(AI::aspirationSearchRoyal, position, side, i, bestMoveEvaluation, ref(TransposTable));

        //updateBestMove = true;
        /*while (bestMoveThread.wait_for(chrono::seconds(0)) != future_status::ready)
        {
            if ((nsecs - timeStart) / (int32_t)1e+6 >= maxMs)
            {
                updateBestMove = false;
                break;
            }
            usleep(20000);
        }*/
        //if (updateBestMove || i == 1)
        {
            auto score = bestMoveThread.get();
            if (get<1>(score) != Constants::UnknownMove)
                tie(bestMoveEvaluation, bestMove) = score;
        }
        /*else
        {
            stopSearch = true;
            break;
        }*/


        //cout << (int)bestMove.From << "->" << (int)bestMove.To << '\n';
#if DEBUG
        cout << "Depth: " << i << "\nEval: " << bestMoveEvaluation << '\n';
        cout << "Base depth: " << setw(4) << i << "." << setw(21) << " Maximal depth: " << setw(4) << maxDepth << "." << setw(18) << " Evaluation: " << setw(6) << (float)bestMoveEvaluation / 100.0f << " pawns." << setw(34) << " Evaluated (this iteration): " << setw(10) << evaluated << "." << setw(51) << "Transposition table cutoffs (this iteration): " << setw(10) << cutOffs << "." << setw(25) << "Time (full search): " << setw(10) << (nsecs - timeStart) / (int32_t)1e+6 << " ms." << endl;
#endif
    }

    double duration = nsecs - timeStart;

    timex += duration;
    countx++;

    usleep(max((int64_t)0, (minMs - (nsecs - timeStart) / (int64_t)1e+6) * (int64_t)1e+3));

    cout << "\033[92m" << "Search finished." << "\033[0m" << endl;


    if (bestMove == Constants::UnknownMove)
    {
        cout << "There isn't any moves\n";
    }
    return bestMove;
}

tuple<int32_t, Move> AI::aspirationSearch(const Position& position, uint8_t side, int32_t depth, int32_t evaluation, TranspositionTable& TransposTable) {
    tuple<int32_t, Move> score;
    int32_t L = evaluation - aspirationWindow, R = evaluation + aspirationWindow;

    score = AI::searchRoot(position, side, L, R, depth, 0, TransposTable);

    if (get<0>(score) <= L)
        score = AI::searchRoot(position, side, Constants::Infinity::Negative, R, depth, 0, TransposTable);
    else
        if (get<0>(score) >= R)
            score = AI::searchRoot(position, side, L, Constants::Infinity::Positive, depth, 0, TransposTable);

    return score;
}

tuple<int32_t, Move> AI::aspirationSearchCCG(const Position& position, uint8_t side, int32_t depth, int32_t evaluation, TranspositionTable& TransposTable) {
    tuple<int32_t, Move> score;
    int32_t L = evaluation - aspirationWindow, R = evaluation + aspirationWindow;

    score = AI::searchRootCCG(position, side, L, R, depth, 0, TransposTable);

    if (get<0>(score) <= L)
        score = AI::searchRootCCG(position, side, Constants::Infinity::Negative, R, depth, 0, TransposTable);
    else
        if (get<0>(score) >= R)
            score = AI::searchRootCCG(position, side, L, Constants::Infinity::Positive, depth, 0, TransposTable);

    return score;
}

tuple<int32_t, Move> AI::aspirationSearchRoyal(const Position& position, uint8_t side, int32_t depth, int32_t evaluation, TranspositionTable& TransposTable)
{
    tuple<int32_t, Move> score;
    int32_t L = evaluation - aspirationWindow, R = evaluation + aspirationWindow;

    score = AI::searchRootRoyal(position, side, L, R, depth, 0, TransposTable);

    if (get<0>(score) <= L)
        score = AI::searchRootRoyal(position, side, Constants::Infinity::Negative, R, depth, 0, TransposTable);
    else
        if (get<0>(score) >= R)
            score = AI::searchRootRoyal(position, side, L, Constants::Infinity::Positive, depth, 0, TransposTable);

    return score;
}

tuple<int32_t, Move> AI::searchRoot(Position position, uint8_t side, int32_t alpha, int32_t beta, int32_t depth_left, int32_t currentDepth, TranspositionTable& TransposTable) {
    MoveList moves = LegalMoveGen::generate(position, side);

    for (uint8_t i = 0; i < moves.size(); i++)
    {
        if (moves[i].type != MoveType::LayOutCard && (moves[i] == Variables::killers[currentDepth][0] || moves[i] == Variables::killers[currentDepth][1]))
        {
            moves[i].type = MoveType::Killer;
        }
    }

    MoveSorter::quickSort(position.pieces, moves, 0, moves.size() - 1);

    bool in_check = PsLegalMoveMaskGen::inDanger(position.pieces, bsf(position.pieces.pieceBitboards[side][Pieces::King]), side);

    int32_t score;
    Position copy;
    Move bestMove = Constants::UnknownMove;

    int32_t sz = moves.size(), new_depth = depth_left - !in_check;

#pragma omp parallel private(copy)
    {
#pragma omp for schedule (dynamic)
        for (int8_t i = 0; i < sz; i = i + 1)
        {
            copy = position;
            copy.move(moves[i]);

            if (i == 0) {
                score = -AI::search<NodeType::PV>(copy, Pieces::inverse(side), -beta, -alpha, new_depth, currentDepth + 1, TransposTable);
            }
            else {
                score = -AI::search<NodeType::NONPV>(copy, Pieces::inverse(side), -alpha - 1, -alpha, new_depth, currentDepth + 1, TransposTable);

                if (score > alpha)
                    score = -AI::search<NodeType::PV>(copy, Pieces::inverse(side), -beta, -alpha, new_depth, currentDepth + 1, TransposTable);
            }
#pragma omp critical
            {
                if (score > alpha) {
                    bestMove = moves[i];

                    if (score >= beta && moves[i].type == MoveType::Quiet && Variables::killers[0][0] != moves[i])
                    {
                        Variables::killers[0][1] = Variables::killers[0][0];
                        Variables::killers[0][0] = moves[i];
                    }

                    alpha = score;
                }
            }
        }
    }

    if (bestMove == Constants::UnknownMove && moves.size() > 0)
        bestMove = moves[0];
    return make_tuple(alpha, bestMove);
}

tuple<int32_t, Move> AI::searchRootCCG(Position position, uint8_t side, int32_t alpha, int32_t beta, int32_t depth_left, int32_t currentDepth, TranspositionTable& TransposTable) {
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


    bool in_check = PsLegalMoveMaskGen::inDanger(position.pieces, bsf(position.pieces.pieceBitboards[side][Pieces::King]), side);

    int32_t score;
    Position copy;
    Move bestMove = Constants::UnknownMove;

    int32_t sz = moves.size(), new_depth = depth_left - !in_check;

#pragma omp parallel private(copy)
    {
#pragma omp for schedule (dynamic)
        for (int8_t i = 0; i < sz; i = i + 1)
        {
            copy = position;
            copy.move(moves[i]);

            if (i == 0) {
                score = -AI::searchCCG<NodeType::PV>(copy, Pieces::inverse(side), -beta, -alpha, new_depth, currentDepth + 1, TransposTable);
            }
            else {
                score = -AI::searchCCG<NodeType::NONPV>(copy, Pieces::inverse(side), -alpha - 1, -alpha, new_depth, currentDepth + 1, TransposTable);

                if (score > alpha)
                    score = -AI::searchCCG<NodeType::PV>(copy, Pieces::inverse(side), -beta, -alpha, new_depth, currentDepth + 1, TransposTable);
            }
#pragma omp critical
            {
                if (score > alpha) {
                    bestMove = moves[i];

                    if (moves[i].type != MoveType::LayOutCard && score >= beta && moves[i].type == MoveType::Quiet && Variables::killers[0][0] != moves[i])
                    {
                        Variables::killers[0][1] = Variables::killers[0][0];
                        Variables::killers[0][0] = moves[i];
                    }

                    alpha = score;
                }
            }
        }
    }

    if (bestMove == Constants::UnknownMove)
        bestMove = moves[0];
    return make_tuple(alpha, bestMove);
}

tuple<int32_t, Move> AI::searchRootRoyal(Position position, uint8_t side, int32_t alpha, int32_t beta, int32_t depth_left, int32_t currentDepth, TranspositionTable& TransposTable)
{
    MoveList moves = LegalMoveGen::generateMovesRoyal(position, side);

    if (moves.size() == 0)
        return make_tuple(Constants::UnknownScore, Constants::UnknownMove);

    bool in_check = PsLegalMoveMaskGen::inDanger(position.pieces, bsf(position.pieces.pieceBitboards[side][Pieces::King]), side);

    int32_t score;
    Position copy;
    Move bestMove = Constants::UnknownMove;

    int32_t sz = moves.size(), new_depth = depth_left - !in_check;

#pragma omp parallel private(copy)
    {
#pragma omp for schedule (dynamic)
        for (int8_t i = 0; i < sz; i = i + 1)
        {
            copy = position;
            copy.moveRoyal(moves[i]);

            if (i == 0) {
                score = -AI::searchRoyal<NodeType::PV>(copy, Pieces::inverse(side), -beta, -alpha, new_depth, currentDepth + 1, TransposTable);
            }
            else {
                score = -AI::searchRoyal<NodeType::NONPV>(copy, Pieces::inverse(side), -alpha - 1, -alpha, new_depth, currentDepth + 1, TransposTable);

                if (score > alpha)
                    score = -AI::searchRoyal<NodeType::PV>(copy, Pieces::inverse(side), -beta, -alpha, new_depth, currentDepth + 1, TransposTable);
            }
#pragma omp critical
            {
                if (score > alpha) {
                    bestMove = moves[i];

                    if (score >= beta && moves[i].type == MoveType::Quiet && Variables::killersRoyal[0][0][moves[i].AttackerType] != moves[i])
                    {
                        Variables::killersRoyal[0][1][moves[i].AttackerType] = Variables::killersRoyal[0][0][moves[i].AttackerType];
                        Variables::killersRoyal[0][0][moves[i].AttackerType] = moves[i];
                    }

                    alpha = score;
                }
            }
        }
    }

    if (bestMove == Constants::UnknownMove)
        bestMove = moves[0];
    return make_tuple(alpha, bestMove);
}

int32_t AI::quiescence(Position position, uint8_t side, int32_t alpha, int32_t beta, int32_t currentDepth) {
    
    node_count++;

    if (stopSearch)
    {
        return alpha;
    }

    int32_t stand_pat = 0, evaluation;

    evaluated++;

    bool in_check = PsLegalMoveMaskGen::inDanger(position.pieces, bsf(position.pieces.pieceBitboards[side][Pieces::King]), side);

    if (!in_check)
    {
        stand_pat = StaticEvaluator::evaluate(position.pieces, position.WhiteLongCastling, position.WhiteShortCastling, position.BlackLongCastling, position.BlackShortCastling, position.whiteCastlingHappened, position.blackCastlingHappened);
        if (side == Pieces::Black)
            stand_pat = -stand_pat;

        if (stand_pat >= beta)
            return beta;

        int32_t delta = Pieces::Queen;
        Bitboard promotingLine = (side == Pieces::White ? BitboardRows::Rows[7] : BitboardRows::Rows[1]);
        if (position.pieces.pieceBitboards[Pieces::Pawn][side] & promotingLine) {
            delta += Pieces::Queen - Pieces::Pawn;
        }

        if (stand_pat + delta < alpha)
            return alpha;

        if (alpha < stand_pat)
            alpha = stand_pat;
    }

    MoveList moves = LegalMoveGen::generate(position, side, true);
    MoveSorter::quickSort(position.pieces, moves, 0, moves.size() - 1);

    Position copy;

    int32_t pieceMaterial = StaticEvaluator::pieceMaterial(position.pieces, Pieces::inverse(side));
    for (uint8_t i = 0; i < moves.size(); i++)
    {
        if (!in_check && (stand_pat + StaticEvaluator::Materials[moves[i].DefenderType] + 200) < alpha &&
            (pieceMaterial - StaticEvaluator::Materials[moves[i].DefenderType]) > StaticEvaluator::Material::EndgameMat &&
            moves[i].Flag < 7)
            continue;

        copy = position;
        copy.move(moves[i]);

        if (i == 0) {
            evaluation = -quiescence(copy, Pieces::inverse(side), -beta, -alpha, currentDepth + 1);
        }
        else {
            evaluation = -quiescence(copy, Pieces::inverse(side), -alpha - 1, -alpha, currentDepth + 1);

            if (evaluation > alpha)
                evaluation = -quiescence(copy, Pieces::inverse(side), -beta, -alpha, currentDepth + 1);

        }

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

int32_t AI::quiescenceRoyal(Position position, uint8_t side, int32_t alpha, int32_t beta, int32_t currentDepth)
{
    node_count++;

    if (stopSearch)
    {
        return alpha;
    }

    int32_t stand_pat = 0, evaluation;

    evaluated++;

    bool in_check = PsLegalMoveMaskGen::inDanger(position.pieces, bsf(position.pieces.pieceBitboards[side][Pieces::King]), side);

    if (!in_check)
    {
        stand_pat = StaticEvaluator::evaluate(position.pieces, position.WhiteLongCastling, position.WhiteShortCastling, position.BlackLongCastling, position.BlackShortCastling, position.whiteCastlingHappened, position.blackCastlingHappened);
        if (side == Pieces::Black)
            stand_pat = -stand_pat;

        if (stand_pat >= beta)
            return beta;

        int32_t delta = Pieces::Queen;
        Bitboard promotingLine = (side == Pieces::White ? BitboardRows::Rows[7] : BitboardRows::Rows[1]);
        if (position.pieces.pieceBitboards[Pieces::Pawn][side] & promotingLine) {
            delta += Pieces::Queen - Pieces::Pawn;
        }

        if (stand_pat + delta < alpha)
            return alpha;

        if (alpha < stand_pat)
            alpha = stand_pat;
    }

    MoveList moves;// = LegalMoveGen::generateMovesRoyal(position, side, true);

    std::map<std::string, uint8_t> counts;

    auto deck = (side == AIside) ? position.AIdeck : position.playerDeck;

    std::vector<card> new_cards;
    for (uint8_t i = 0; i < deck.size(); i++)
    {
        std::string figure = deck[i].getFigure();
        if (!counts[figure])
        {
            new_cards.push_back(deck[i]);
        }
        counts[figure]++;
    }

    double sum = 0;

    Position copy;

    int32_t pieceMaterial = StaticEvaluator::pieceMaterial(position.pieces, Pieces::inverse(side));

    int32_t N = deck.size(), A = alpha, B = beta;

    for (auto card : new_cards)
    {
        alpha = A, beta = B;

        position.cards[side].addCard(card);

        moves = LegalMoveGen::generateMovesRoyal(position, side, true);

        if (moves.size() == 0)
        {
            evaluation = -quiescenceRoyal(copy, Pieces::inverse(side), -alpha - 1, -alpha, currentDepth + 1);

            if (evaluation > alpha)
                evaluation = -quiescenceRoyal(copy, Pieces::inverse(side), -beta, -alpha, currentDepth + 1);

            if (evaluation >= beta)
            {
                alpha = beta;
            }

            if (evaluation > alpha)
            {
                alpha = evaluation;
            }
        }
        else
        {
            copy = position;

            uint8_t i = 0;

            for (i; i < moves.size(); i = i + 1)
            {
                if (!in_check && (stand_pat + StaticEvaluator::Materials[moves[i].DefenderType] + 200) < alpha &&
                    (pieceMaterial - StaticEvaluator::Materials[moves[i].DefenderType]) > StaticEvaluator::Material::EndgameMat &&
                    moves[i].Flag < 7)
                    continue;

                copy = position;
                copy.moveRoyal(moves[i]);

                if (i == 0) {
                    evaluation = -quiescenceRoyal(copy, Pieces::inverse(side), -beta, -alpha, currentDepth + 1);
                }
                else {
                    evaluation = -quiescenceRoyal(copy, Pieces::inverse(side), -alpha - 1, -alpha, currentDepth + 1);

                    if (evaluation > alpha)
                        evaluation = -quiescenceRoyal(copy, Pieces::inverse(side), -beta, -alpha, currentDepth + 1);

                }

                if (evaluation >= beta)
                {
                    alpha = beta;
                    break;
                }

                if (evaluation > alpha)
                {
                    alpha = evaluation;
                }
            }
        }
       
        sum += (double)(counts[card.getFigure()] / N) * alpha;

        position.cards[side].delete_card(card.getFigure()[0]);
    }

    return sum;
}
