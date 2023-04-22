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
    std::array<class ChessMove, 218> moves{};
    uint8_t Size;

public:
    MoveList();
	~MoveList();

    class ChessMove &operator[](uint8_t index);

    class ChessMove operator[](uint8_t index) const;

    void push_back(class ChessMove move);

    [[nodiscard]] uint8_t size() const;
};
