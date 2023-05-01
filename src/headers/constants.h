#pragma once
#include "move.h"
#include <future>

namespace Constants {
    struct Infinity
    {
        static constexpr int32_t Negative = -1e+9;
        static constexpr int32_t Positive = 1e+9;
    };

	const Move UnknownMove = Move(0, 0, 0, 0, 0, 0, 0);
    const int32_t UnknownScore = 2e9;
}

namespace Variables {
    static mutex mtx;
    static condition_variable parallel;
    static int32_t threadsNumber = 4;
}
