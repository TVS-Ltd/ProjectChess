// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <cmath>
#include <sstream>
#include <vector>
#include <map>

#include "Pieces.h"
#include "Move.h"
#include "ZobristHash.h"
#include "RepetitionHistory.h"
#include "Card.h"
#include "Handsdeck.h"

#include "CoreMinimal.h"

/**
 * 
 */
class CHESSDEV_API BoardPosition
{
public:
    Pieces pieces;
    uint8_t EnPassant;

    int32_t points[2]{}, cardsNumber[2]{};

    std::map<uint8_t, uint8_t> aiDeck, playerDeck;
    std::map<uint8_t, uint8_t> handsdecks[2];

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

    BoardPosition();
    BoardPosition(const string& shortFen, uint8_t enPassant, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling, float moveCtr);

	~BoardPosition();

    ostream& operator<<(ostream &ostream);

    std::string toString();

    void move(ChessMove move);

    void moveRoyal(ChessMove move);

    void addPiece(uint8_t square, uint8_t type, uint8_t side);

private:
    void removePiece(uint8_t square, uint8_t type, uint8_t side);

    void changeEnPassant(uint8_t enPassant);

    void removeWhiteLongCastling();

    void removeWhiteShortCastling();

    void removeBlackLongCastling();

    void removeBlackShortCastling();

    void updateMoveCtr();

    void updateFiftyMovesCtr(bool breakEvent);

    void setPoints(int32_t whitePoints, int32_t blackPoints) 
    {
        points[Pieces::White] = whitePoints;
        points[Pieces::Black] = blackPoints;
    }

    void setNumberOfCards(int8_t whiteNumber, int8_t blackNumber) 
    {
        cardsNumber[Pieces::White] = whiteNumber;
        cardsNumber[Pieces::Black] = blackNumber;
    }
};
