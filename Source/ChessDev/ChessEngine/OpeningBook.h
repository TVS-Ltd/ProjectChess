// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <fstream>
#include <future>
#include <sstream>
#include <tuple>

#include "Position.h"
#include "MoveList.h"
#include "LegalMoveGen.h"

#include "Misc/Paths.h"
#include "CoreMinimal.h"

/**
 * 
 */
class CHESSDEV_API OpeningBook
{
public:
public:
    OpeningBook();
    OpeningBook(std::string path);

	~OpeningBook();

    std::tuple<ChessMove, int32_t> tryToFindMove(const BoardPosition& position);

private:
    vector<vector<ChessMove>> moves;
};
