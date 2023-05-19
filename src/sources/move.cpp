#include "move.h"

using namespace std;

Move::Move()
{
}
Move::Move(uint8_t from, uint8_t to, uint8_t attackerType, uint8_t attackerSide, uint8_t defenderType, uint8_t defenderSide, MoveType type, uint8_t flag)
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

bool operator==(Move left, Move right)
{
    return (left.From == right.From && left.To == right.To && left.AttackerType == right.AttackerType && left.AttackerSide == right.AttackerSide && left.DefenderType == right.DefenderType && left.DefenderSide == right.DefenderSide && left.Flag == right.Flag);
}