// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces.h"

using namespace std;

// Pieces::Pieces() = default;

// Pieces::Pieces(const string &shortFen)
// {
//     uint8_t x = 0;
//     uint8_t y = 7;

//     uint8_t side;

//     for (auto buff : shortFen)
//     {
//         if (buff == '/')
//         {
//             x = 0;
//             y--;
//         } else 
//         if (isdigit(buff))
//         {
//             x = x + buff - '0';
//         } else
//         {
//             if (isupper(buff))
//             {
//                 buff = tolower(buff);
//                 side = Pieces::White;
//             } else
//             {
//                 side = Pieces::Black;
//             }

//             switch (buff)
//             {
//                 case 'p':
//                     setOne(this->pieceBitboards[side][Pieces::Pawn], y * 8 + x);
//                 break;

//                 case 'n':
//                     setOne(this->pieceBitboards[side][Pieces::Knight], y * 8 + x);
//                 break;

//                 case 'b':
//                     setOne(this->pieceBitboards[side][Pieces::Bishop], y * 8 + x);
//                 break;

//                 case 'r':
//                     setOne(this->pieceBitboards[side][Pieces::Rook], y * 8 + x);
//                 break;

//                 case 'q':
//                     setOne(this->pieceBitboards[side][Pieces::Queen], y * 8 + x);
//                 break;

//                 case 'k':
//                     setOne(this->pieceBitboards[side][Pieces::King], y * 8 + x);
//                 break;
//             }

//             x++;
//         }
//     }

//     this->updateBitboards();
// }

// ostream &operator<<(ostream &ostream, Pieces pieces)
// {
//     ostream << " ";

//     for (uint8_t i = 0; i < 8; i++)
//     {
//         ostream << " -----";
//     }

//     ostream << "\n8";

//     for (int8_t y = 7; y >= 0; y--)
//     {
//         for (uint8_t x = 0; x < 8; x++)
//         {
//             ostream << "|  ";

//             if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Pawn], y * 8 + x))
//             {
//                 ostream << "♟";
//             }
//             else if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Knight], y * 8 + x))
//             {
//                 ostream << "♞";
//             }
//             else if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Bishop], y * 8 + x))
//             {
//                 ostream << "♝";
//             }
//             else if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Rook], y * 8 + x))
//             {
//                 ostream << "♜";
//             }
//             else if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Queen], y * 8 + x))
//             {
//                 ostream << "♛";
//             }
//             else if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::King], y * 8 + x))
//             {
//                 ostream << "♚";
//             } else

//             if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Pawn], y * 8 + x))
//             {
//                 ostream << "♙";
//             }
//             else if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Knight], y * 8 + x))
//             {
//                 ostream << "♘";
//             }
//             else if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Bishop], y * 8 + x))
//             {
//                 ostream << "♗";
//             }
//             else if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Rook], y * 8 + x))
//             {
//                 ostream << "♖";
//             }
//             else if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Queen], y * 8 + x))
//             {
//                 ostream << "♕";
//             }
//             else if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::King], y * 8 + x))
//             {
//                 ostream << "♔";
//             } else
//             {
//                 ostream << " ";
//             }

//             ostream << "  ";
//         }

//             ostream << "|\n  ";

//             for (uint8_t i = 0; i < 8; i++)
//             {
//                 ostream << "----- ";
//             }

//             ostream << "\n";

//             if (y != 0)
//             {
//                 ostream << (int)y;
//             }
//         }

//         ostream << "    a     b     c     d     e     f     g     h  \n\n";

//         return ostream;
//     }

// bool operator==(Pieces left, Pieces right)
// {
//     for (uint8_t i = 0; i < 2; i++)
//     {
//         for (uint8_t j = 0; j < 6; j++)
//         {
//             if (left.pieceBitboards[i][j] != right.pieceBitboards[i][j])
//             {
//                 return false;
//             }
//         }
//     }

//     return true;
// }

// void Pieces::updateBitboards()
// {
//     this->sideBitboards[White] = this->pieceBitboards[White][Pawn] | this->pieceBitboards[White][Knight] | this->pieceBitboards[White][Bishop] | this->pieceBitboards[White][Rook] | this->pieceBitboards[White][Queen] | this->pieceBitboards[White][King];

//     this->sideBitboards[Black] = this->pieceBitboards[Black][Pawn] | this->pieceBitboards[Black][Knight] | this->pieceBitboards[Black][Bishop] | this->pieceBitboards[Black][Rook] | this->pieceBitboards[Black][Queen] | this->pieceBitboards[Black][King];

//     this->inversionSideBitboards[White] = ~this->sideBitboards[White];
//     this->inversionSideBitboards[Black] = ~this->sideBitboards[Black];

//     this->all = this->sideBitboards[White] | this->sideBitboards[Black];
//     this->empty = ~this->all;
// }

// uint8_t Pieces::inverse(uint8_t side)
// {
//     return !side;
// }



Pieces::Pieces() = default;

Pieces::~Pieces() {}

Pieces::Pieces(const string& shortFen)
{
    uint8_t x = 0;
    uint8_t y = 7;

    uint8_t side;

    for (auto buff : shortFen)
    {
        if (buff == '/')
        {
            x = 0;
            y--;
        }
        else
            if (isdigit(buff))
            {
                x = x + buff - '0';
            }
            else
            {
                if (isupper(buff))
                {
                    buff = tolower(buff);
                    side = Pieces::White;
                }
                else
                {
                    side = Pieces::Black;
                }

                switch (buff)
                {
                case 'p':
                    setOne(this->pieceBitboards[side][Pieces::Pawn], y * 8 + x);
                    break;

                case 'n':
                    setOne(this->pieceBitboards[side][Pieces::Knight], y * 8 + x);
                    break;

                case 'b':
                    setOne(this->pieceBitboards[side][Pieces::Bishop], y * 8 + x);
                    break;

                case 'r':
                    setOne(this->pieceBitboards[side][Pieces::Rook], y * 8 + x);
                    break;

                case 'q':
                    setOne(this->pieceBitboards[side][Pieces::Queen], y * 8 + x);
                    break;

                case 'k':
                    setOne(this->pieceBitboards[side][Pieces::King], y * 8 + x);
                    break;
                }

                x++;
            }
    }

    this->updateBitboards();
}

ostream& operator<<(ostream& ostream, Pieces pieces)
{
    ostream << " ";

    for (uint8_t i = 0; i < 8; i++)
    {
        ostream << " -----";
    }

    ostream << "\n8";

    for (int8_t y = 7; y >= 0; y--)
    {
        for (uint8_t x = 0; x < 8; x++)
        {
            ostream << "|  ";

            if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Pawn], y * 8 + x))
            {
                ostream << "P";
            }
            else if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Knight], y * 8 + x))
            {
                ostream << "N";
            }
            else if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Bishop], y * 8 + x))
            {
                ostream << "S";
            }
            else if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Rook], y * 8 + x))
            {
                ostream << "L";
            }
            else if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Queen], y * 8 + x))
            {
                ostream << "F";
            }
            else if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::King], y * 8 + x))
            {
                ostream << "K";
            }
            else

                if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Pawn], y * 8 + x))
                {
                    ostream << "p";//"♙";
                }
                else if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Knight], y * 8 + x))
                {
                    ostream << "n";// "♘";
                }
                else if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Bishop], y * 8 + x))
                {
                    ostream << "s";// "♗";
                }
                else if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Rook], y * 8 + x))
                {
                    ostream << "l";// "♖";
                }
                else if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Queen], y * 8 + x))
                {
                    ostream << "f";// "♕";
                }
                else if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::King], y * 8 + x))
                {
                    ostream << "k";// "♔";
                }
                else
                {
                    ostream << " ";
                }

            ostream << "  ";
        }

        ostream << "|\n  ";

        for (uint8_t i = 0; i < 8; i++)
        {
            ostream << "----- ";
        }

        ostream << "\n";

        if (y != 0)
        {
            ostream << (int)y;
        }
    }

    ostream << "    a     b     c     d     e     f     g     h  \n\n";

    return ostream;
}

bool operator==(Pieces left, Pieces right)
{
    for (uint8_t i = 0; i < 2; i++)
    {
        for (uint8_t j = 0; j < 6; j++)
        {
            if (left.pieceBitboards[i][j] != right.pieceBitboards[i][j])
            {
                return false;
            }
        }
    }

    return true;
}

void Pieces::updateBitboards()
{
    this->sideBitboards[White] = this->pieceBitboards[White][Pawn] | this->pieceBitboards[White][Knight] | this->pieceBitboards[White][Bishop] | this->pieceBitboards[White][Rook] | this->pieceBitboards[White][Queen] | this->pieceBitboards[White][King];

    this->sideBitboards[Black] = this->pieceBitboards[Black][Pawn] | this->pieceBitboards[Black][Knight] | this->pieceBitboards[Black][Bishop] | this->pieceBitboards[Black][Rook] | this->pieceBitboards[Black][Queen] | this->pieceBitboards[Black][King];

    this->inversionSideBitboards[White] = ~this->sideBitboards[White];
    this->inversionSideBitboards[Black] = ~this->sideBitboards[Black];

    this->all = this->sideBitboards[White] | this->sideBitboards[Black];
    this->empty = ~this->all;
}

uint8_t Pieces::inverse(uint8_t side)
{
    return !side;
}
