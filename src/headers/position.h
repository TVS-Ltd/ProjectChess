#include <cmath>
#include <vector>
#include "pieces.h"
#include "move.h"
#include "zobristHash.h"
#include "repetitionHistory.h"
#include "card.h"
#include "handsdeck.h"

#pragma once

// —————————————————— Position ——————————————————
class Position
{
public:
    Pieces pieces;
    uint8_t EnPassant;

    handsdeck cards[2];

    bool WhiteLongCastling;
    bool WhiteShortCastling;
    bool BlackLongCastling;
    bool BlackShortCastling;

    bool whiteCastlingHappened;
    bool blackCastlingHappened;

    bool allowNullMove = true;

    float MoveCtr;
    ZobristHash hash;
    float fiftyMovesCtr;
    RepetitionHistory repetitionHistory;

    Position();
    Position(const string& shortFen, uint8_t enPassant, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling, float moveCtr);

    friend ostream& operator<<(ostream& ostream, Position position);

    void move(Move move);

    void addPiece(uint8_t square, uint8_t type, uint8_t side);

    void removePiece(uint8_t square, uint8_t type, uint8_t side);

    void changeEnPassant(uint8_t enPassant);

    void removeWhiteLongCastling();

    void removeWhiteShortCastling();

    void removeBlackLongCastling();

    void removeBlackShortCastling();

    void updateMoveCtr();

    void updateFiftyMovesCtr(bool breakEvent);
};