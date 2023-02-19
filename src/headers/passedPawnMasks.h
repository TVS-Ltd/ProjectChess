#include "bitboards.h"

#pragma once

namespace PassedPawnMasks
{
    static consteval array<Bitboard, 64> calculateWhitePassedPawnMasks()
    {
        array<Bitboard, 64> masks{};

        for (uint8_t x = 0; x < 8; x = x + 1)
        {
            for (uint8_t y = 0; y < 8; y = y + 1)
            {

                for (uint8_t y1 = y + 1; y1 < 8; y1 = y1 + 1)
                {
                    if (x != 0)
                        setOne(masks[y * 8 + x], y1 * 8 + x - 1);
                    if (x != 7)
                        setOne(masks[y * 8 + x], y1 * 8 + x + 1);
                    setOne(masks[y * 8 + x], y1 * 8 + x);
                }
            }
        }

        return masks;
    }

    static consteval array<Bitboard, 64> calculateBlackPassedPawnMasks()
    {
        array<Bitboard, 64> masks{};

        for (uint8_t x = 0; x < 8; x = x + 1)
        {
            for (uint8_t y = 0; y < 8; y = y + 1)
            {

                for (int8_t y1 = y - 1; y1 >= 0; y1 = y1 - 1)
                {
                    if (x != 0)
                        setOne(masks[y * 8 + x], y1 * 8 + x - 1);
                    if (x != 7)
                        setOne(masks[y * 8 + x], y1 * 8 + x + 1);
                    setOne(masks[y * 8 + x], y1 * 8 + x);
                }
            }
        }

        return masks;
    }

    static constexpr array<Bitboard, 64> WhitePassedPawnMasks = PassedPawnMasks::calculateWhitePassedPawnMasks();
    static constexpr array<Bitboard, 64> BlackPassedPawnMasks = PassedPawnMasks::calculateBlackPassedPawnMasks();
}