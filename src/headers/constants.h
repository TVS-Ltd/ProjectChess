#pragma once
#include "move.h"
#include <future>

namespace Constants {
    struct Infinity
    {
        static constexpr int32_t Negative = -1e+9;
        static constexpr int32_t Positive = 1e+9;
    };

    const Move UnknownMove = Move(0, 0, 0, 0, 0, 0, MoveType::Unknown);
    const int32_t UnknownScore = 2e9;
    const int32_t MaximalDepth = 12;
}

namespace Killers {
    static Move classic[15][2];
    static Move royal[15][2][6];
}

namespace Figures {
    static std::vector<uint8_t> figures = { 0, 1, 2, 3, 4, 5 };
    static std::vector<uint8_t> figuresCount = { 8, 2, 2, 2, 1, 1, 0 };
    static std::vector<uint8_t> points = { 10, 30, 25, 20, 50 };
}