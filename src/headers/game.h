#include "bitboards.h"
#include "handsdeck.h"
#include "card.h"
#include "ai.h"
#include "move.h"
#include "timer.h"
#include <sqlite3.h>
#include <iostream>
#include <cstring>
#include <regex>
#include "deck.h"
#pragma once

#if LINUX
// ONLY FOR LINUX
#define OpeningBookFile "/usr/bin/OpeningBook.txt"
#else
// FOR WINDOWS
#define OpeningBookFile "./src/OpeningBook.txt"
#endif

using namespace std;

/**
 * The main class, which handles the game launch.
*/
class Game
{
public:
    void start();

private:
    AI ai = { OpeningBookFile }; // The main AI object.
    Move move;
    MoveList moves;
    Position position; // Stores the current board position.
    uint8_t playerSide; // Stores player's side.
    uint8_t aiSide; // Stores AI's side.
    Move playerMove;
#if LOG_TO_FILE
    logToFile log;
#endif
    uint32_t timerLose = 0;
    Timer timerWhite;
    Timer timerBlack;

    /**
     * Checks the white win conditions.
     *
     * @return Returns True if White wins, False otherwise.
     *
     * @exception This function does not throw exceptions.
    */
    bool whiteVictory();

    bool whiteRoyalVictory();

    bool blackVictory();

    /**
     * Checks the draw conditions.
     *
     * @return Returns True if game should end with draw, False otherwise.
     *
     * @exception This function does not throw exceptions.
    */
    bool draw();

    /**
     * Checks the current board position for win/lose/draw.
     *
     * @return Returns True if game ends, False otherwise.
     *
     * @exception This function does not throw exceptions.
    */
    bool gameFinished();

    /**
     * Checks if the current turn is for White.
     *
     * @return Returns True if it is white's turn, False otherwise.
     *
     * @exception This function does not throw exceptions.
    */
    bool isWhiteMove() const;

    /**
     * Checks if the current turn is for Black.
     *
     * @return Returns True if it is black's turn, False otherwise.
     *
     * @exception This function does not throw exceptions.
    */
    bool isBlackMove() const;

    /**
     * Gets move input from the terminal.
     *
     * @return Returns the move, which has been made.
     *
     * @exception This function does not throw exceptions.
    */
    Move getMove();

    /**
     * Assigns player's and AI's sides.
     *
     * @exception This function does not throw exceptions.
    */
    void sideChoose();

    bool movePlayer(uint8_t side);

    bool movePlayerForRoyalChess(uint8_t side, uint8_t playerTypeOfFigure);

    void PvE();

    void PvP();

    void EvE();

    void PvPTime();

    void Gwent();
    
    void RoyalChess();

    void chooseLimitedTimeMode();

    /**
     * Function for choosing the game mode using the terminal.
    */
    void chooseGameMode();

    // ——————————————————Terminal colors——————————————————
    const char* END = "\033[0m";

    // Foreground colors
    const char* RED = "\033[91m";
    const char* GREEN = "\033[92m";
    const char* YELLOW = "\033[93m";
    const char* WHITE = "\033[97m";
    const char* BLACK = "\033[90m";

    // Background colors
    const char* REDBG = "\033[101m";
    const char* GREENBG = "\033[102m";
    const char* YELLOWBG = "\033[103m";
    const char* WHITEBG = "\033[107m";
    const char* BLACKBG = "\033[100m";
};