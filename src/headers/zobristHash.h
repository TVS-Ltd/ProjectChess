#include <cstdint>
#include <array>
#include "bitboards.h"
#include "zobristHashConsteval.h"
#include "pieces.h"

#pragma once

// —————————————————— Zobrist Hash ——————————————————
class ZobristHash
{
public:
    ZobristHash();

    ZobristHash(Pieces pieces, bool blackMove, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling);

    friend bool operator==(ZobristHash left, ZobristHash right);

    friend bool operator<(ZobristHash left, ZobristHash right);

    void invertPiece(uint8_t square, uint8_t type, uint8_t side);

    void invertMove();

    void invertWhiteLongCastling();

    void invertWhiteShortCastling();

    void invertBlackLongCastling();

    void invertBlackShortCastling();

    uint64_t hash;
};