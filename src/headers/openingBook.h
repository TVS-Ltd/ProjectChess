#include <fstream>
#include <future>
#include <sstream>
#include "position.h"
#include "moveList.h"
#include "legalMoveGen.h"

#pragma once

class OpeningBook
{
public:
    OpeningBook();
    OpeningBook(const string& path);

    tuple<Move, int32_t> tryToFindMove(const Position& position);

private:
    vector<vector<Move>> moves;
};