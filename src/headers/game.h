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

class Game
{
public:
    void start();

private:
    AI ai = {OpeningBookFile};
    Move move;
    MoveList moves;
    Position position;
    uint8_t playerSide;
    uint8_t aiSide;
    Move playerMove;
    #if LOG_TO_FILE
        logToFile log;
    #endif
    uint32_t timerLose = 0;
    Timer timerWhite;
    Timer timerBlack;

    bool whiteVictory();

    bool whiteRoyalVictory();

    bool blackVictory();

    bool draw();

    bool gameFinished();

    bool isWhiteMove() const;

    bool isBlackMove();

    Move getMove();

    void sideChoose();

    bool movePlayer(uint8_t side);

    void PvE();

    void PvP();

    void EvE();

    void PvPTime();

    void Gwent();
    
    void RoyalChess();

    void chooseLimitedTimeMode();

    void chooseGameMode();

    // ——————————————————Terminal colors——————————————————
const char * END = "\033[0m";

// Foreground colors
const char * RED = "\033[91m";
const char * GREEN = "\033[92m";
const char * YELLOW = "\033[93m";
const char * WHITE = "\033[97m";
const char * BLACK = "\033[90m";

// Background colors
const char * REDBG = "\033[101m";
const char * GREENBG = "\033[102m";
const char * YELLOWBG = "\033[103m";
const char * WHITEBG = "\033[107m";
const char * BLACKBG = "\033[100m";
};