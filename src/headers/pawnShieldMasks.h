#include "bitboards.h"

#pragma once

namespace PawnShieldMasks
{
    static consteval std::array<Bitboard, 64> calculateWhitePawnShieldMasks()
    {
        array<Bitboard, 64> whitePawnShieldMasks{};

        for (uint8_t x = 0; x < 8; x = x + 1)
        {
            for (uint8_t y = 0; y < 7; y = y + 1)
            {
                if (x != 0)
                    setOne(whitePawnShieldMasks[y * 8 + x], (y + 1) * 8 + x - 1);
                if (x != 7)
                    setOne(whitePawnShieldMasks[y * 8 + x], (y + 1) * 8 + x + 1);
                setOne(whitePawnShieldMasks[y * 8 + x], (y + 1) * 8 + x);

                if (y != 6)
                {
                    if (x != 0)
                        setOne(whitePawnShieldMasks[y * 8 + x], (y + 2) * 8 + x - 1);
                    if (x != 7)
                        setOne(whitePawnShieldMasks[y * 8 + x], (y + 2) * 8 + x + 1);
                    setOne(whitePawnShieldMasks[y * 8 + x], (y + 2) * 8 + x);
                }
            }
        }

        return whitePawnShieldMasks;
    }

    static consteval array<Bitboard, 64> calculatePawnShieldMasks()
    {
        array<Bitboard, 64> blackPawnShieldMasks{};

        for (uint8_t x = 0; x < 8; x = x + 1)
        {
            for (uint8_t y = 1; y < 8; y = y + 1)
            {
                if (x != 0)
                    setOne(blackPawnShieldMasks[y * 8 + x], (y - 1) * 8 + x - 1);
                if (x != 7)
                    setOne(blackPawnShieldMasks[y * 8 + x], (y - 1) * 8 + x + 1);
                setOne(blackPawnShieldMasks[y * 8 + x], (y - 1) * 8 + x);

                if (y != 1)
                {
                    if (x != 0)
                        setOne(blackPawnShieldMasks[y * 8 + x], (y - 2) * 8 + x - 1);
                    if (x != 7)
                        setOne(blackPawnShieldMasks[y * 8 + x], (y - 2) * 8 + x + 1);
                    setOne(blackPawnShieldMasks[y * 8 + x], (y - 2) * 8 + x);
                }
            }
        }

        return blackPawnShieldMasks;
    }

    static constexpr array<Bitboard, 64> WhitePawnShieldMasks = PawnShieldMasks::calculateWhitePawnShieldMasks();
    static constexpr array<Bitboard, 64> BlackPawnShieldMasks = PawnShieldMasks::calculatePawnShieldMasks();
}