#include "bitboards.h"

namespace SlidersMasks
{
    struct Direction
    {
        static constexpr int8_t North = 0;
        static constexpr int8_t South = 1;
        static constexpr int8_t West = 2;
        static constexpr int8_t East = 3;

        static constexpr int8_t NorthWest = 4;
        static constexpr int8_t NorthEast = 5;
        static constexpr int8_t SouthWest = 6;
        static constexpr int8_t SouthEast = 7;
    };

    static consteval Bitboard calculateMask(uint8_t p, int8_t direction)
    {
        Bitboard mask = 0;

        int8_t x = p % 8;
        int8_t y = p / 8;

        for (;;)
        {
            switch (direction)
            {
                case SlidersMasks::Direction::North:
                    y++;
                break;

                case SlidersMasks::Direction::South:
                    y--;
                break;

                case SlidersMasks::Direction::West:
                    x--;
                break;

                case SlidersMasks::Direction::East:
                    x++;
                break;

                case SlidersMasks::Direction::NorthWest:
                    y++;
                    x--;
                break;

                case SlidersMasks::Direction::NorthEast:
                    y++;
                    x++;
                break;

                case SlidersMasks::Direction::SouthWest:
                    y--;
                    x--;
                break;

                case SlidersMasks::Direction::SouthEast:
                    y--;
                    x++;
                break;
            }

            if (x > 7 || x < 0 || y > 7 || y < 0)
            {
                break;
            }

            setOne(mask, y * 8 + x);
        }

        return mask;
    }

    static consteval array<array<Bitboard, 8>, 64> calculateMasks()
    {
        array<array<Bitboard, 8>, 64> masks{};

        for (uint8_t i = 0; i < 64; i++)
        {
            for (uint8_t j = 0; j < 8; j++)
            {
                masks[i][j] = SlidersMasks::calculateMask(i, j);
            }
        }

        return masks;
    }

    static constexpr std::array<std::array<Bitboard, 8>, 64> Masks = SlidersMasks::calculateMasks();
};