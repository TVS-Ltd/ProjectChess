#include "game.h"
#include "card.h"

using namespace std;

void Game::start()
{
    this->position = { "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 255, true, true, true, true, 1 };

#if TEST_WHITE_WIN
    this->position = { "rnbqkbnr/pppppppp/8/8/2B5/5Q2/PPPPPPPP/RNB1K1NR", 255, true, true, true, true, 1 };
#endif

#if TEST_BLACK_WIN
    this->position = { "2k5/8/8/8/8/8/7f/l2K4", 255, true, true, true, true, 1 };
#endif

#if TEST_DRAW
    this->position = { "k7/8/8/8/8/8/8/K", 255, true, true, true, true, 1 };
#endif

#if TEST_PROMOTION
    this->position = { "rnbqkbnr/11111111/8/8/8/8/K111111p/11111111", 255, true, true, true, true, 1 };
#endif

#if TEST_CASTLING
    this->position = { "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK11R", 255, true, true, true, true, 1 };
#endif

#if TEST_EN_PASSANT
    this->position = { "rnbqkbnr/pppppppp/8/1111P111/8/8/PPPP1PPP/RNBQKBNR", 255, true, true, true, true, 1 };
#endif

#if TEST_THREEE_POS_RULE
    this->position = { "rnbqkbnr/8/8/8/8/8/8/RNBQKBNR", 255, true, true, true, true, 1 };
#endif

#if TEST_CARD_MOVES
    this->position = { "4k3/b7/8/8/8/8/P1Q3R1/1N2K3", 255, true, true, true, true, 1 };
#endif
    cout << "Welcome to the chess engine!" << endl;

#if LOG_TO_FILE
    log << "Welcome to the chess engine!";
#endif

    chooseGameMode();
}

bool Game::whiteVictory()
{
    bool blackHaventGotMoves = (LegalMoveGen::generate(this->position, Pieces::Black).size() == 0);
    bool blackInCheck = PsLegalMoveMaskGen::inDanger(this->position.pieces, bsf(this->position.pieces.pieceBitboards[Pieces::Black][Pieces::King]), Pieces::Black);

    if (playerSide == Pieces::White && blackHaventGotMoves && blackInCheck)
    {
        cout << GREEN << endl;
        cout << "White win!" << END << endl;

#if LOG_TO_FILE
        log << GREEN << "White win!" << END;
#endif
    }
    else
        if (blackHaventGotMoves && blackInCheck)
        {
            cout << RED << endl;
            cout << "White win!" << END << endl;

#if LOG_TO_FILE
            log << RED << "White win!" << END;
#endif
        }
        else
            if (playerSide == Pieces::Black && timerBlack.getElapsedSeconds() > timerLose)
            {
                cout << RED << "White win!" << END << endl;
                clog << RED << "Time is over!" << END << endl;

#if LOG_TO_FILE
                log << GREEN << "White win!" << END;
#endif

                return true;
            }
            else
                if (playerSide == Pieces::White && timerBlack.getElapsedSeconds() > timerLose)
                {
                    cout << GREEN << "White win!" << END << endl;
                    clog << RED << "Time is over!" << END << endl;

#if LOG_TO_FILE
                    log << GREEN << "White win!" << END;
#endif

                    return true;
                }



    return (blackHaventGotMoves && blackInCheck);
}

bool Game::blackVictory()
{
    bool whiteHaventGotMoves = (LegalMoveGen::generate(this->position, Pieces::White).size() == 0);
    bool whiteInCheck = PsLegalMoveMaskGen::inDanger(this->position.pieces, bsf(this->position.pieces.pieceBitboards[Pieces::White][Pieces::King]), Pieces::White);

    if (playerSide == Pieces::Black && whiteHaventGotMoves && whiteInCheck)
    {
        cout << GREEN << endl;
        cout << "Black win!" << END << endl;

#if LOG_TO_FILE
        log << GREEN << "Black win!" << END;
#endif
    }
    else if (whiteHaventGotMoves && whiteInCheck)
    {
        cout << RED << endl;
        cout << "Black win!" << END << endl;

#if LOG_TO_FILE
        log << RED << "Black win!" << END;
#endif
    }
    else
        if (playerSide == Pieces::White && timerWhite.getElapsedSeconds() > timerLose)
        {
            cout << RED << "Black win!" << END << endl;
            clog << RED << "Time is over!" << END << endl;

#if LOG_TO_FILE
            log << RED << "Black win!" << END;
#endif

            return true;
        }
        else
            if (playerSide == Pieces::Black && timerWhite.getElapsedSeconds() > timerLose)
            {
                cout << GREEN << "Black win!" << END << endl;
                clog << RED << "Time is over!" << END << endl;

#if LOG_TO_FILE
                log << RED << "Black win!" << END;
#endif

                return true;
            }



    return (whiteHaventGotMoves && whiteInCheck);
}

bool Game::draw()
{
    bool whiteHaventGotMoves = (LegalMoveGen::generate(this->position, Pieces::White).size() == 0);
    bool whiteInCheck = PsLegalMoveMaskGen::inDanger(this->position.pieces, bsf(this->position.pieces.pieceBitboards[Pieces::White][Pieces::King]), Pieces::White);

    bool blackHaventGotMoves = (LegalMoveGen::generate(this->position, Pieces::Black).size() == 0);
    bool blackInCheck = PsLegalMoveMaskGen::inDanger(this->position.pieces, bsf(this->position.pieces.pieceBitboards[Pieces::Black][Pieces::King]), Pieces::Black);

    bool fiftyMovesRule = (this->position.fiftyMovesCtr >= 50);
    bool threeMovesRule = (this->position.repetitionHistory.getRepetionNumber(this->position.hash) >= 3);

    bool whiteMove = this->isWhiteMove();
    bool blackMove = !whiteMove;

    if (whiteHaventGotMoves && !whiteInCheck && whiteMove)
    {
        cout << YELLOW << "[DRAW] White haven't got moves." << END << endl;

#if LOG_TO_FILE
        log << YELLOW << "[DRAW] White haven't got moves." << END;
#endif

        return true;
    }

    if (blackHaventGotMoves && !blackInCheck && blackMove)
    {
        cout << YELLOW << "[DRAW] Black haven't got moves." << END << endl;

#if LOG_TO_FILE
        log << YELLOW << "[DRAW] Black haven't got moves." << END;
#endif

        return true;
    }

    if (fiftyMovesRule)
    {
        cout << YELLOW << "[DRAW] Fifty moves rule." << END << endl;

#if LOG_TO_FILE
        log << YELLOW << "[DRAW] Fifty moves rule." << END;
#endif

        return true;
    }

    if (threeMovesRule)
    {
        cout << YELLOW << "[DRAW] Three moves rule." << END << endl;

#if LOG_TO_FILE
        log << YELLOW << "[DRAW] Three moves rule." << END;
#endif

        return true;
    }

    if (timerWhite.getElapsedSeconds() == timerBlack.getElapsedSeconds() && timerWhite.getElapsedSeconds() > timerLose)
    {
        cout << YELLOW << "[DRAW] Time is up." << END << endl;

#if LOG_TO_FILE
        log << YELLOW << "[DRAW] Time is up." << END;
#endif

        return true;
    }

    return false;
}

bool Game::gameFinished()
{
    if (this->whiteVictory() || this->blackVictory() || this->draw())
    {
        timerWhite.reset();
        timerBlack.reset();

        return true;
    }

    return false;
}

bool Game::isWhiteMove() const
{
    return (this->position.MoveCtr - floor(this->position.MoveCtr) < 1e-7);
}

bool Game::isBlackMove() const
{
    return !isWhiteMove();
}

Move Game::getMove()
{
    Move PlayerMove;
    string from, to;

    cout << "\nEnter move (Example: e2 e4): ";
    cin >> from >> to;

    PlayerMove.From = (from[0] - 'a') + (from[1] - '1') * 8;
    PlayerMove.To = (to[0] - 'a') + (to[1] - '1') * 8;

    return PlayerMove;
}

void Game::sideChoose()
{
    cout << "Choose your side:" << endl;
    cout << "1. White" << endl;
    cout << "2. Black" << endl;

    int choice;

    do
    {
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 2)
        {
            cin.clear();
            cin.ignore(1000, '\n');

            cout << RED << "[ERROR] Invalid input. Please try again." << END << endl;
        }
    } while (cin.fail() || choice < 1 || choice > 2);

    if (choice == 1)
    {
        cout << WHITE << "You are playing as white." << END << endl;

#if LOG_TO_FILE
        log << WHITE << "User is playing as white." << END;
#endif

        playerSide = Pieces::White;
        aiSide = Pieces::Black;
    }
    else
    {
        cout << BLACK << WHITEBG << "You are playing as black." << END << endl;

#if LOG_TO_FILE
        log << BLACK << WHITEBG << "User is playing as black." << END;
#endif

        playerSide = Pieces::Black;
        aiSide = Pieces::White;
    }
}

bool Game::movePlayer(uint8_t side) // Returns true if player move is valid
{
    this->moves = LegalMoveGen::generate(this->position, side);

    this->playerMove = getMove();
    bool moveFound = false;

    for (uint8_t i = 0; i < moves.size(); i++)
    {
        if (moves[i].From == playerMove.From && moves[i].To == playerMove.To && (moves[i].Flag != Move::Flag::PromoteToKnight && moves[i].Flag != Move::Flag::PromoteToBishop && moves[i].Flag != Move::Flag::PromoteToRook && moves[i].Flag != Move::Flag::PromoteToQueen))
        {
            move = moves[i];
            moveFound = true;

            break;
        }
        else if (moves[i].From == playerMove.From && moves[i].To == playerMove.To)
        {
            // Choose promotion piece
            cout << "\nChoose promotion piece: " << endl;
            cout << "1. Knight" << endl;
            cout << "2. Bishop" << endl;
            cout << "3. Rook" << endl;
            cout << "4. Queen" << endl;

            int promotionPiece = 0;
            cin >> promotionPiece;

            switch (promotionPiece)
            {
            case 1:
                move = moves[i];
                move.Flag = Move::Flag::PromoteToKnight;
                moveFound = true;
                break;

            case 2:
                move = moves[i];
                move.Flag = Move::Flag::PromoteToBishop;
                moveFound = true;
                break;

            case 3:
                move = moves[i];
                move.Flag = Move::Flag::PromoteToRook;
                moveFound = true;
                break;

            case 4:
                move = moves[i];
                move.Flag = Move::Flag::PromoteToQueen;
                moveFound = true;
                break;

            default:
                cout << RED << "\n[ERROR] Illegal move!" << END << endl;
                continue;
                break;
            }

            break;
        }
    }

    if (!moveFound)
    {
        cout << RED << "\n[ERROR] Illegal move!" << END << endl;
        return false;
    }

    this->position.move(move);

    if (move.DefenderType != 255)
    {
        cout << "\n";

        switch (move.DefenderType)
        {
        case 0:
            cout << YELLOW << "Pawn has been captured!" << END << endl;
            break;

        case 1:
            cout << YELLOW << "Knight has been captured!" << END << endl;
            break;

        case 2:
            cout << YELLOW << "Bishop has been captured!" << END << endl;
            break;

        case 3:
            cout << YELLOW << "Rook has been captured!" << END << endl;
            break;

        case 4:
            cout << YELLOW << "Queen has been captured!" << END << endl;
            break;

        default:
            cout << RED << "[ERROR] Unknown piece type!" << END << endl;
            break;
        }
    }

    cout << "\nMove has been made!" << endl;
    cout << position << endl;

    return true;
}

void Game::PvE()
{
    sideChoose();

    if (playerSide == Pieces::White)
    {
        while (true)
        {

            cout << position << endl;

#if LOG_TO_FILE
            log << position;
#endif

            if (!movePlayer(playerSide))
            {
                continue;
            }

            // Check if game is finished
            if (this->gameFinished())
            {
                cout << position << endl;

#if LOG_TO_FILE
                log << position;
#endif

                break;
            }

            // AI move
            move = ai.bestMove(position, aiSide, 0, 10000);

            position.move(move);

            // Check if game is finished
            if (this->gameFinished())
            {
                cout << position << endl;

#if LOG_TO_FILE
                log << position;
#endif

                break;
            }

        }
    }
    else
    {
        cout << position << endl;

#if LOG_TO_FILE
        log << position;
#endif

        while (true)
        {

            // AI move
            move = ai.bestMoveRoyal(position, aiSide, 0, 10000);
            position.moveRoyal(move);

            cout << position << endl;

#if LOG_TO_FILE
            log << position;
#endif

            // Check if game is finished
            if (this->gameFinished())
            {
                cout << position << endl;

#if LOG_TO_FILE
                log << position;
#endif

                break;
            }

            // Player move
            while (!movePlayer(playerSide))
            {
                continue;
            }

            // Check if game is finished
            if (this->gameFinished())
            {
                cout << position << endl;

#if LOG_TO_FILE
                log << position;
#endif

                break;
            }
        }
    }
}

void Game::PvP()
{
    sideChoose();

    cout << position << endl;

#if LOG_TO_FILE
    log << position;
#endif

    if (playerSide == Pieces::White)
    {
        while (true)
        {

            // Player 1 move
            cout << GREEN << "\nPlayer 1 move:" << END << endl;

#if LOG_TO_FILE
            log << GREEN << "\nPlayer 1 move:" << END;
#endif

            while (!movePlayer(playerSide))
            {
                continue;
            }

            // Check if game is finished
            if (this->gameFinished())
            {
                cout << position << endl;

#if LOG_TO_FILE
                log << position;
#endif

                break;
            }

            // Player 2 move
            cout << YELLOW << "\nPlayer 2 move:" << END << endl;

#if LOG_TO_FILE
            log << YELLOW << "\nPlayer 2 move:" << END;
#endif

            while (!movePlayer(aiSide))
            {
                continue;
            }

            // Check if game is finished
            if (this->gameFinished())
            {
                cout << position << endl;

#if LOG_TO_FILE
                log << position;
#endif

                break;
            }
        }
    }
    else
    {
        while (true)
        {

            // Player 2 move
            cout << YELLOW << "\nPlayer 2 move:" << END << endl;

#if LOG_TO_FILE
            log << YELLOW << "\nPlayer 2 move:" << END;
#endif

            while (!movePlayer(aiSide))
            {
                continue;
            }

            // Check if game is finished
            if (this->gameFinished())
            {
                cout << position << endl;

#if LOG_TO_FILE
                log << position;
#endif

                break;
            }

            // Player 1 move
            cout << GREEN << "\nPlayer 1 move:" << END << endl;

#if LOG_TO_FILE
            log << GREEN << "\nPlayer 1 move:" << END;
#endif

            while (!movePlayer(playerSide))
            {
                continue;
            }

            // Check if game is finished
            if (this->gameFinished())
            {
                cout << position << endl;

#if LOG_TO_FILE
                log << position;
#endif

                break;
            }
        }
    }
}

void Game::EvE()
{
    playerSide = Pieces::White;
    aiSide = Pieces::Black;

    int step = 0;
    while (true)
    {
        std::cout << ++step << '\n';

        cout << position << endl;

#if LOG_TO_FILE
        log << position;
#endif

        // AI 1 move
        move = ai.bestMove(position, playerSide, 0, 10000);
        position.move(move);

        // Check if game is finished
        if (this->gameFinished())
        {
            cout << position << endl;

#if LOG_TO_FILE
            log << position;
#endif

            break;
        }

        cout << position << endl;

#if LOG_TO_FILE
        log << position;
#endif

        // AI 2 move
        move = ai.bestMove(position, aiSide, 0, 10000);
        position.move(move);

        // Check if game is finished
        if (this->gameFinished())
        {
            cout << position << endl;

#if LOG_TO_FILE
            log << position;
#endif

            break;
        }

    }
}

void Game::PvPTime()
{
    //Start game in which each side has 10 minutes to make a move use class Timer
    //If one side doesn't make a move in time, he loses the game
    //If both sides don't make a move in time, draw

    sideChoose();

    cout << position << endl;

#if LOG_TO_FILE
    log << position;
#endif

    //Start timer for white
    timerWhite.start();

    if (playerSide == Pieces::White)
    {
        while (true)
        {
            // Player 1 move
            cout << GREEN << "\nPlayer 1 move:" << END << endl;

            //Output time left in format mm:ss
            cout << "Time left (min:sec): " << (timerLose - timerWhite.getElapsedSeconds()) / 60 << ":" << (60 - (timerWhite.getElapsedSeconds() - timerWhite.getElapsedMinutes() * 60)) % 60 << endl;

#if LOG_TO_FILE
            log << GREEN << "\nPlayer 1 move:" << END;
#endif

            while (!movePlayer(playerSide))
            {
                continue;
            }

            // Check if game is finished
            if (this->gameFinished())
            {
                cout << position << endl;

#if LOG_TO_FILE
                log << position;
#endif

                break;
            }

            //Stop timer for white
            timerWhite.stop();

            //Start timer for black
            timerBlack.start();

            // Player 2 move
            cout << YELLOW << "\nPlayer 2 move:" << END << endl;

            //Output time left in format mm:ss
            cout << "Time left (min:sec): " << (timerLose - timerBlack.getElapsedSeconds()) / 60 << ":" << (60 - (timerBlack.getElapsedSeconds() - timerBlack.getElapsedMinutes() * 60)) % 60 << endl;

#if LOG_TO_FILE
            log << YELLOW << "\nPlayer 2 move:" << END;
#endif

            while (!movePlayer(aiSide))
            {
                continue;
            }

            // Check if game is finished
            if (this->gameFinished())
            {
                cout << position << endl;

#if LOG_TO_FILE
                log << position;
#endif

                break;
            }

            //Stop timer for black
            timerBlack.stop();

            //Start timer for white
            timerWhite.start();
        }
    }
    else
    {
        while (true)
        {
            // Player 1 move
            cout << GREEN << "\nPlayer 2 move:" << END << endl;

            //Output time left in format mm:ss
            cout << "Time left (min:sec): " << (timerLose - timerWhite.getElapsedSeconds()) / 60 << ":" << (60 - (timerWhite.getElapsedSeconds() - timerWhite.getElapsedMinutes() * 60)) % 60 << endl;

#if LOG_TO_FILE
            log << GREEN << "\nPlayer 2 move:" << END;
#endif

            while (!movePlayer(aiSide))
            {
                continue;
            }

            // Check if game is finished
            if (this->gameFinished())
            {
                cout << position << endl;

#if LOG_TO_FILE
                log << position;
#endif

                break;
            }

            //Stop timer for white
            timerWhite.stop();

            //Start timer for black
            timerBlack.start();

            // Player 1 move
            cout << YELLOW << "\nPlayer 1 move:" << END << endl;

            //Output time left in format mm:ss
            cout << "Time left (min:sec): " << (timerLose - timerBlack.getElapsedSeconds()) / 60 << ":" << (60 - (timerBlack.getElapsedSeconds() - timerBlack.getElapsedMinutes() * 60)) % 60 << endl;

#if LOG_TO_FILE
            log << YELLOW << "\nPlayer 1 move:" << END;
#endif

            while (!movePlayer(playerSide))
            {
                continue;
            }

            // Check if game is finished
            if (this->gameFinished())
            {
                cout << position << endl;

#if LOG_TO_FILE
                log << position;
#endif

                break;
            }

            //Stop timer for black
            timerBlack.stop();

            //Start timer for white
            timerWhite.start();
        }
    }
}

void Game::chooseLimitedTimeMode()
{
    //Choose time mode for game (Blitz, Rapid, Classical)
    cout << "Choose time mode: " << endl;
    cout << "1. Blitz (5 min)" << endl;
    cout << "2. Rapid (10 min)" << endl;
    cout << "3. Classical (15 min)" << endl;

    int timeMode = 0;
    cin >> timeMode;

    switch (timeMode)
    {
    case 1:
#if LOG_TO_FILE
        log << "Time mode: Blitz (5 min)";
#endif

        timerLose = 5 * 60; //5 minutes
        PvPTime();
        break;

    case 2:
#if LOG_TO_FILE
        log << "Time mode: Rapid (10 min)";
#endif

        timerLose = 10 * 60; //10 minutes
        PvPTime();
        break;

    case 3:
#if LOG_TO_FILE
        log << "Time mode: Classical (15 min)";
#endif

        timerLose = 15 * 60; //15 minutes
        PvPTime();
        break;

    default:
        cout << "Wrong input!" << endl;
        chooseLimitedTimeMode();
        break;
    }
}

void Game::chooseGameMode()
{
    // Choose game mode
    cout << "Choose game mode: " << endl;
    cout << "1. Player vs Player" << endl;
    cout << "2. Player vs AI" << endl;
    cout << "3. AI vs AI" << endl;
    cout << "4. Player vs Player (10 min limit)" << endl;

    int gameMode = 0;
    cin >> gameMode;

    switch (gameMode)
    {
    case 1:
#if LOG_TO_FILE
        log << "Game mode: Player vs Player";
#endif

        PvP();
        break;

    case 2:
#if LOG_TO_FILE
        log << "Game mode: Player vs AI";
#endif

        PvE();
        break;

    case 3:
#if LOG_TO_FILE
        log << "Game mode: AI vs AI";
#endif
        EvE();
        break;

    case 4:
#if LOG_TO_FILE
        log << "Game mode: Player vs Player with time";
#endif

        timerWhite.reset();
        timerBlack.reset();

        chooseLimitedTimeMode();
        break;


    default:
        cout << RED << "\n[ERROR] Illegal game mode!\n" << END << endl;
        this->chooseGameMode();
        break;
    }
}