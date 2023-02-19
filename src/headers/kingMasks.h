#include "bitboards.h"

namespace KingMasks
{
    static consteval uint8_t absSubtract(uint8_t left, uint8_t right)
    {
        if (left >= right)
        {
            return left - right;
        }

        return right - left;
    }

    static consteval array<Bitboard, 64> calculateMasks()
    {
        array<Bitboard, 64> masks{};

        uint8_t dx;
        uint8_t dy;

        for (uint8_t x0 = 0; x0 < 8; x0++)
        {
            for (uint8_t y0 = 0; y0 < 8; y0++)
            {
                for (uint8_t x1 = 0; x1 < 8; x1++)
                {
                    for (uint8_t y1 = 0; y1 < 8; y1++)
                    {

                        dx = KingMasks::absSubtract(x0, x1);
                        dy = KingMasks::absSubtract(y0, y1);

                        if (dx <= 1 && dy <= 1)
                        {
                            setOne(masks[y0 * 8 + x0], y1 * 8 + x1);
                        }
                    }
                }
            }
        }

        return masks;
    }

    static constexpr array<Bitboard, 64> Masks = KingMasks::calculateMasks();
}