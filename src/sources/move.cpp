#include "move.h"

using namespace std;

Move::Move()
{
    From = -1;
    To = -1;
}
Move::Move(uint8_t from, uint8_t to, uint8_t attackerType, uint8_t attackerSide, uint8_t defenderType, uint8_t defenderSide, uint8_t flag)
{
    this->From = from;
    this->To = to;

    this->AttackerType = attackerType;
    this->AttackerSide = attackerSide;

    this->DefenderType = defenderType;
    this->DefenderSide = defenderSide;

    this->Flag = flag;
}

bool operator==(Move left, Move right)
{
    return (left.From == right.From && left.To == right.To && left.AttackerType == right.AttackerType && left.AttackerSide == right.AttackerSide && left.DefenderType == right.DefenderType && left.DefenderSide == right.DefenderSide && left.Flag == right.Flag);
}
