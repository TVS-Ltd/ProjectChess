// Fill out your copyright notice in the Description page of Project Settings.


#include "Move.h"

using namespace std;

ChessMove::ChessMove() = default;
ChessMove::ChessMove(uint8_t from, uint8_t to, uint8_t attackerType, uint8_t attackerSide, uint8_t defenderType, uint8_t defenderSide, MoveType type, uint8_t flag)
{
    this->From = from;
    this->To = to;

    this->AttackerType = attackerType;
    this->AttackerSide = attackerSide;

    this->DefenderType = defenderType;
    this->DefenderSide = defenderSide;

    this->Flag = flag;
    this->type = type;
}

bool ChessMove::operator==(ChessMove other)
{
    return (this->From == other.From && this->To == other.To && this->AttackerType == other.AttackerType && this->AttackerSide == other.AttackerSide && this->DefenderType == other.DefenderType && this->DefenderSide == other.DefenderSide && this->Flag == other.Flag);
}

ChessMove::~ChessMove()
{
}
