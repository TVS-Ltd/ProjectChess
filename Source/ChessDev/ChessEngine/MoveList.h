// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <array>
#include "CoreMinimal.h"
#include "Move.h"

/**
 * 
 */
class CHESSDEV_API MoveList
{
private:
    std::array<ChessMove, 280> moves{};
    uint8_t Size;

public:
    MoveList();
	~MoveList();

    ChessMove &operator[](uint8_t index);

    ChessMove operator[](uint8_t index) const;

    void push_back(ChessMove move);

    [[nodiscard]] uint8_t size() const;

    void unite(MoveList other);
};
