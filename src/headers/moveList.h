#include "move.h"

#pragma once

class MoveList
{
private:
    array<Move, 280> moves{};
    uint8_t Size;

public:
    MoveList();

    Move& operator[](uint8_t index);

    Move operator[](uint8_t index) const;

    void push_back(Move move);

    [[nodiscard]] uint8_t size() const;

    void unite(MoveList other);
};