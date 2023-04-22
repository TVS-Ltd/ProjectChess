// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <cmath>
#include <sstream>
#include "Pieces.h"
#include "Move.h"
#include "ZobristHash.h"
#include "RepetitionHistory.h"
#include "CoreMinimal.h"

/**
 * 
 */
class CHESSDEV_API BoardPosition
{
public:
    Pieces pieces;
    uint8_t EnPassant;

    bool WhiteLongCastling;
    bool WhiteShortCastling;
    bool BlackLongCastling;
    bool BlackShortCastling;

    bool whiteCastlingHappened;
    bool blackCastlingHappened;

    float MoveCtr;
    ZobristHash hash;
    float fiftyMovesCtr;
    RepetitionHistory repetitionHistory;

    BoardPosition();
    BoardPosition(const string &shortFen, uint8_t enPassant, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling, float moveCtr);

	~BoardPosition();

    ostream& operator<<(ostream &ostream);

    std::string toString();

    void move(class ChessMove move);

private:
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
