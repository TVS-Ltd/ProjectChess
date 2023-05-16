#include <cmath>
#include <vector>
#include "pieces.h"
#include "move.h"
#include "zobristHash.h"
#include "repetitionHistory.h"
#include "card.h"
#include "handsdeck.h"
#include <map>

#pragma once

static map<int8_t, std::string>
figureByPoins
= { {10, "Pawn"}, {20, "Rook"}, {25, "Bishop"}, {30, "Knight"}, {50, "Queen"} };
static map<std::string, int8_t>
pointsByFigure
= { {"Pawn", 10}, {"Rook", 20}, {"Bishop", 25}, {"Knight", 30}, {"Queen", 50} };
static map<std::string, uint8_t> typeByFigure
= { {"Pawn", Pieces::Pawn}, {"Rook", Pieces::Rook}, {"Bishop", Pieces::Bishop}, {"Knight", Pieces::Knight}, {"Queen", Pieces::Queen} };

// —————————————————— Position ——————————————————
class Position
{
public:
    Pieces pieces;
    uint8_t EnPassant;

    handsdeck cards[2];
    std::vector<card> AIdeck, playerDeck;
    int8_t points[2]{}, cardsNumber[2]{};

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

    void moveRoyal(Move move);

    void addPiece(uint8_t square, uint8_t type, uint8_t side);

    void removePiece(uint8_t square, uint8_t type, uint8_t side);

    void changeEnPassant(uint8_t enPassant);

    void removeWhiteLongCastling();

    void removeWhiteShortCastling();

    void removeBlackLongCastling();

    void removeBlackShortCastling();

    void updateMoveCtr();

    void updateFiftyMovesCtr(bool breakEvent);

    void setPoints(int8_t whitePoints, int8_t blackPoints) {
        points[Pieces::White] = whitePoints;
        points[Pieces::Black] = blackPoints;
    }

    void setNumberOfCards(int8_t whiteNumber, int8_t blackNumber) {
        cardsNumber[Pieces::White] = whiteNumber;
        cardsNumber[Pieces::Black] = blackNumber;
    }
};