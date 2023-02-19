#include "bitboards.h"

namespace ZobristHashConsteval
{
    namespace randomKeyGenerator
    {
        static constexpr uint64_t Seed = 0x98f107;
        static constexpr uint64_t Multiplier = 0x71abc9;
        static constexpr uint64_t Summand = 0xff1b3f;
    }

    static consteval uint64_t nextRandom(uint64_t previous)
    {
        return ZobristHashConsteval::randomKeyGenerator::Multiplier * previous + ZobristHashConsteval::randomKeyGenerator::Summand;
    }

    static consteval array<array<array<uint64_t, 6>, 2>, 64> calculateConstants()
    {
        array<array<array<uint64_t, 6>, 2>, 64> constants{};

        uint64_t previous = ZobristHashConsteval::randomKeyGenerator::Seed;

        for (uint8_t square = 0; square < 64; square++)
        {
            for (uint8_t side = 0; side < 2; side++)
            {
                for (uint8_t type = 0; type < 6; type++)
                {
                    previous = ZobristHashConsteval::nextRandom(previous);
                    constants[square][side][type] = previous;
                }
            }
        }

        return constants;
    }

    static constexpr array<array<array<uint64_t, 6>, 2>, 64> Constants = calculateConstants();
    static constexpr uint64_t BlackMove = nextRandom(Constants[63][1][5]);
    static constexpr uint64_t WhiteLongCastling = nextRandom(BlackMove);
    static constexpr uint64_t WhiteShortCastling = nextRandom(WhiteLongCastling);
    static constexpr uint64_t BlackLongCastling = nextRandom(WhiteShortCastling);
    static constexpr uint64_t BlackShortCastling = nextRandom(BlackLongCastling);
}