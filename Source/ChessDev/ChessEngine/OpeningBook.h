// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <fstream>
#include <future>
#include <sstream>
#include "Position.h"
#include "MoveList.h"
#include "LegalMoveGen.h"
#include "CoreMinimal.h"

/**
 * 
 */
class CHESSDEV_API OpeningBook
{
public:
public:
    OpeningBook();
    OpeningBook(const string &path);

	~OpeningBook();

    tuple<class ChessMove, int32_t> tryToFindMove(const BoardPosition &position);

private:
    vector<vector<class ChessMove>> moves;
};
