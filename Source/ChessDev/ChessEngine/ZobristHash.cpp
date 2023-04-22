// Fill out your copyright notice in the Description page of Project Settings.


#include "ZobristHash.h"

using namespace std;

ZobristHash::ZobristHash() = default;

ZobristHash::ZobristHash(Pieces pieces, bool blackMove, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling)
{
    this->hash = 0;

    if (blackMove)
    {
        this->invertMove();
    }

    if (whiteLongCastling)
    {
        this->invertWhiteLongCastling();
    }

    if (whiteShortCastling)
    {
        this->invertWhiteShortCastling();
    }

    if (blackLongCastling)
    {
        this->invertBlackLongCastling();
    }

    if (blackShortCastling)
    {
        this->invertBlackShortCastling();
    }

    uint8_t side;

    for (uint8_t square = 0; square < 64; square++)
    {
        if (getBit(pieces.sideBitboards[Pieces::White], square))
        {
            side = Pieces::White;
        }
        else if (getBit(pieces.sideBitboards[Pieces::Black], square))
        {
            side = Pieces::Black;
        }
        else
        {
            continue;
        }

        for (uint8_t type = 0; type < 6; type++)
        {
            if (getBit(pieces.pieceBitboards[side][type], square))
            {
                this->invertPiece(square, type, side);
                break;
            }
        }
    }
}

bool operator==(ZobristHash left, ZobristHash right)
{
    return (left.hash == right.hash);
}

bool operator<(ZobristHash left, ZobristHash right)
{
    return (left.hash < right.hash);
}

void ZobristHash::invertPiece(uint8_t square, uint8_t type, uint8_t side)
{
    this->hash = this->hash ^ ZobristHashConsteval::Constants[square][side][type];
}

void ZobristHash::invertMove()
{
    this->hash = this->hash ^ ZobristHashConsteval::BlackMove;
}

void ZobristHash::invertWhiteLongCastling()
{
    this->hash = this->hash ^ ZobristHashConsteval::WhiteLongCastling;
}

void ZobristHash::invertWhiteShortCastling()
{
    this->hash = this->hash ^ ZobristHashConsteval::WhiteShortCastling;
}

void ZobristHash::invertBlackLongCastling()
{
    this->hash = this->hash ^ ZobristHashConsteval::BlackLongCastling;
}

void ZobristHash::invertBlackShortCastling()
{
    this->hash = this->hash ^ ZobristHashConsteval::BlackShortCastling;
}

ZobristHash::~ZobristHash()
{
}
