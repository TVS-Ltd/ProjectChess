#include <bit>
#include <array>
#include <vector>
#include <cstdint>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <iostream>
#include <fstream>
#include <future>
#include <unistd.h>
#include <set>

#define TEST_LEGAL_MOVE_GENERATION false
#define TEST_AI false
#define PLAY true
#define nsecs std::chrono::high_resolution_clock::now().time_since_epoch().count()
#define OpeningBookFile "OpeningBook.txt"


//Terminal colors
const char* END = "\033[0m";
const char* RED = "\033[91m";
const char* GREEN = "\033[92m";
const char* YELLOW = "\033[93m";


using namespace std;

typedef uint64_t Bitboard;

//Set a bit in a bitboard to 1
static constexpr void setOne(Bitboard &bitBoard, uint8_t square)
{
    bitBoard = bitBoard | (1ull << square);
}

//Set a bit in a bitboard to 0
static constexpr void setZero(Bitboard &bitBoard, uint8_t square)
{
    bitBoard = bitBoard & (~(1ull << square));
}

//Get the value of a bit in a bitboard
static constexpr bool getBit(Bitboard bitBoard, uint8_t square)
{
    return (bitBoard & (1ull << square));
}

//Count the number of 1s in a bitboard
static constexpr uint8_t countOnes(Bitboard bitBoard)
{
    return popcount(bitBoard);
}

//https://www.chessprogramming.org/BitScan
static constexpr array<uint8_t, 64> BitScanTable = {
    0, 47,  1, 56, 48, 27,  2, 60,
    57, 49, 41, 37, 28, 16,  3, 61,
    54, 58, 35, 52, 50, 42, 21, 44,
    38, 32, 29, 23, 17, 11,  4, 62,
    46, 55, 26, 59, 40, 36, 15, 53,
    34, 51, 20, 43, 31, 22, 10, 45,
    25, 39, 14, 33, 19, 30,  9, 24,
    13, 18,  8, 12,  7,  6,  5, 63
};

static constexpr uint8_t bsf(Bitboard bitBoard) 
{
    return BitScanTable[((bitBoard ^ (bitBoard - 1)) * 0x03f79d71b4cb0a89) >> 58];
}

static constexpr uint8_t bsrFunc(Bitboard bitBoard) 
{
    bitBoard = bitBoard | (bitBoard >> 1);
    bitBoard = bitBoard | (bitBoard >> 2);
    bitBoard = bitBoard | (bitBoard >> 4);
    bitBoard = bitBoard | (bitBoard >> 8);
    bitBoard = bitBoard | (bitBoard >> 16);
    bitBoard = bitBoard | (bitBoard >> 32);

    return BitScanTable[(bitBoard * 0x03f79d71b4cb0a89) >> 58];
}

//Bitboard rows
namespace BitboardRows 
{
    static consteval array<Bitboard, 8> calculateRows() 
    {
        array<Bitboard, 8> rows{};

        for (uint8_t y = 0; y < 8; y = y + 1) 
        {
            for (uint8_t x = 0; x < 8; x = x + 1)
            {
                setOne(rows[y], y * 8 + x);
            }
        }

        return rows;
    }


    static constexpr array<Bitboard, 8> Rows = calculateRows();


    static consteval array<Bitboard, 8> calculateInversionRows() 
    {
        array<Bitboard, 8> inversionRows{};

        for (uint8_t i = 0; i < 8; i = i + 1)
        {
            inversionRows[i] = ~Rows[i];
        }

        return inversionRows;
    }


    static constexpr std::array<Bitboard, 8> InversionRows = BitboardRows::calculateInversionRows();
}

//Bitboard columns
namespace BitboardColumns 
{
    static consteval array<Bitboard, 8> calculateColumns() 
    {
        array<Bitboard, 8> columns{};

        for (uint8_t x = 0; x < 8; x = x + 1) 
        {
            for (uint8_t y = 0; y < 8; y = y + 1)
            {
                setOne(columns[x], y * 8 + x);
            }
        }

        return columns;
    }


    static constexpr array<Bitboard, 8> Columns = calculateColumns();


    static consteval array<Bitboard, 8> calculateInversionColumns() 
    {
        array<Bitboard, 8> inversionColumns{};

        for (uint8_t i = 0; i < 8; i = i + 1)
        {
            inversionColumns[i] = ~Columns[i];
        }

        return inversionColumns;
    }


    static constexpr array<Bitboard, 8> InversionColumns = calculateInversionColumns();
}

class Pieces 
{
public:
    Pieces() = default;
    Pieces(const string& shortFen)
    {
        uint8_t x = 0;
        uint8_t y = 7;

        uint8_t side;

        for (auto buff : shortFen) 
        {
            if (buff == '/') 
            {
                x = 0;
                y = y - 1;
            } else 
            if (isdigit(buff)) 
            {
                x = x + buff - '0';
            } else 
            {
                if (isupper(buff)) 
                {
                    buff = tolower(buff);
                    side = Pieces::White;
                } else
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

                x = x + 1;
            }
        }

        this->updateBitboards();
    }

    array<array<Bitboard, 6>, 2> pieceBitboards{};
    array<Bitboard, 2> sideBitboards{};
    array<Bitboard, 2> inversionSideBitboards{};
    Bitboard all;
    Bitboard empty;

    static constexpr uint8_t Pawn = 0;
    static constexpr uint8_t Knight = 1;
    static constexpr uint8_t Bishop = 2;
    static constexpr uint8_t Rook = 3;
    static constexpr uint8_t Queen = 4;
    static constexpr uint8_t King = 5;

    static constexpr uint8_t White = 0;
    static constexpr uint8_t Black = 1;

    friend ostream& operator <<(ostream& ostream, Pieces pieces)
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
                    ostream << "♟";
                } else
                if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Knight], y * 8 + x))
                {
                    ostream << "♞";
                } else
                if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Bishop], y * 8 + x))
                {
                    ostream << "♝";
                } else
                if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Rook], y * 8 + x))
                {
                    ostream << "♜";
                } else
                if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Queen], y * 8 + x))
                {
                    ostream << "♛";
                } else
                if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::King], y * 8 + x))
                {
                    ostream << "♚";
                } else

                if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Pawn], y * 8 + x))
                {
                    ostream << "♙";
                } else
                if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Knight], y * 8 + x))
                {
                    ostream << "♘";
                } else
                if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Bishop], y * 8 + x))
                {
                    ostream << "♗";
                } else
                if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Rook], y * 8 + x))
                {
                    ostream << "♖";
                } else
                if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Queen], y * 8 + x))
                {
                    ostream << "♕";
                } else
                if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::King], y * 8 + x))
                {
                    ostream << "♔";
                } else

                ostream << " ";

                ostream << "  ";
            }

            ostream << "|\n  " ;

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

        ostream << "    a     b     c     d     e     f     g     h  \n";

        return ostream;
    }

    friend bool operator ==(Pieces left, Pieces right)
    {
        for (uint8_t i = 0; i < 2; i = i + 1) 
        {
            for (uint8_t j = 0; j < 6; j = j + 1) 
            {
                if (left.pieceBitboards[i][j] != right.pieceBitboards[i][j])
                {
                    return false;
                }
            }
        }

        return true;
    }

    void updateBitboards() 
    {
        this->sideBitboards[White] = this->pieceBitboards[White][Pawn] |
                                               this->pieceBitboards[White][Knight] |
                                               this->pieceBitboards[White][Bishop] |
                                               this->pieceBitboards[White][Rook] |
                                               this->pieceBitboards[White][Queen] |
                                               this->pieceBitboards[White][King];

        this->sideBitboards[Black] = this->pieceBitboards[Black][Pawn] |
                                               this->pieceBitboards[Black][Knight] |
                                               this->pieceBitboards[Black][Bishop] |
                                               this->pieceBitboards[Black][Rook] |
                                               this->pieceBitboards[Black][Queen] |
                                               this->pieceBitboards[Black][King];

        this->inversionSideBitboards[White] = ~this->sideBitboards[White];
        this->inversionSideBitboards[Black] = ~this->sideBitboards[Black];

        this->all = this->sideBitboards[White] | this->sideBitboards[Black];
        this->empty = ~this->all;
    }

    static uint8_t inverse(uint8_t side) 
    {
        return !side;
    }
};

namespace ZobristHashConsteval 
{
    namespace randomKeyGenerator 
    {
        static constexpr uint64_t Seed = 0x98f107;
        static constexpr uint64_t Multiplier = 0x71abc9;
        static constexpr uint64_t Summand = 0xff1b3f;
    }


    static consteval uint64_t nextRandom(uint64_t previous) 
    {
        return ZobristHashConsteval::randomKeyGenerator::Multiplier * previous + ZobristHashConsteval::randomKeyGenerator::Summand;
    }

    static consteval array<array<array<uint64_t, 6>, 2>, 64> calculateConstants() 
    {
        array<array<array<uint64_t, 6>, 2>, 64> constants{};

        uint64_t previous = ZobristHashConsteval::randomKeyGenerator::Seed;

        for (uint8_t square = 0; square < 64; square = square + 1) 
        {
            for (uint8_t side = 0; side < 2; side = side + 1)
            {
                for (uint8_t type = 0; type < 6; type = type + 1) 
                {
                    previous = ZobristHashConsteval::nextRandom(previous);
                    constants[square][side][type] = previous;
                }
            }
        }

        return constants;
    }


    static constexpr array<array<array<uint64_t, 6>, 2>, 64> Constants = calculateConstants();
    static constexpr uint64_t BlackMove = ZobristHashConsteval::nextRandom(ZobristHashConsteval::Constants[63][1][5]);
    static constexpr uint64_t WhiteLongCastling = ZobristHashConsteval::nextRandom(ZobristHashConsteval::BlackMove);
    static constexpr uint64_t WhiteShortCastling = ZobristHashConsteval::nextRandom(ZobristHashConsteval::WhiteLongCastling);
    static constexpr uint64_t BlackLongCastling = ZobristHashConsteval::nextRandom(ZobristHashConsteval::WhiteShortCastling);
    static constexpr uint64_t BlackShortCastling = ZobristHashConsteval::nextRandom(ZobristHashConsteval::BlackLongCastling);
}

class ZobristHash 
{
public:
    ZobristHash() = default;

    ZobristHash(Pieces pieces, bool blackMove, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling)
    {
        this->hash = 0;

        if (blackMove)
        {
            this->invertMove();
        } 

        if (whiteLongCastling)
        {
            this->invertWhiteLongCastling();
        } 

        if (whiteShortCastling)
        {
            this->invertWhiteShortCastling();
        } 

        if (blackLongCastling)
        {
            this->invertBlackLongCastling();
        } 

        if (blackShortCastling)
        {
            this->invertBlackShortCastling();
        } 

        uint8_t side;

        for (uint8_t square = 0; square < 64; square = square + 1) 
        {
            if (getBit(pieces.sideBitboards[Pieces::White], square))
            {
                side = Pieces::White;
            } else 
            if (getBit(pieces.sideBitboards[Pieces::Black], square))
            {
                side = Pieces::Black;
            } else 
            {
                continue;
            }

            for (uint8_t type = 0; type < 6; type = type + 1) 
            {
                if (getBit(pieces.pieceBitboards[side][type], square)) 
                {
                    this->invertPiece(square, type, side);
                    break;
                }
            }
        }
    }

    friend bool operator ==(ZobristHash left, ZobristHash right)
    {
        return (left.hash == right.hash);
    }

    friend bool operator <(ZobristHash left, ZobristHash right)
    {
        return (left.hash < right.hash);
    }

    void invertPiece(uint8_t square, uint8_t type, uint8_t side)
    {
        this->hash = this->hash ^ ZobristHashConsteval::Constants[square][side][type];
    }

    void invertMove()
    {
        this->hash = this->hash ^ ZobristHashConsteval::BlackMove;
    }

    void invertWhiteLongCastling()
    {
        this->hash = this->hash ^ ZobristHashConsteval::WhiteLongCastling;
    }

    void invertWhiteShortCastling()
    {
        this->hash = this->hash ^ ZobristHashConsteval::WhiteShortCastling;
    }

    void invertBlackLongCastling()
    {
        this->hash = this->hash ^ ZobristHashConsteval::BlackLongCastling;
    }

    void invertBlackShortCastling()
    {
        this->hash = this->hash ^ ZobristHashConsteval::BlackShortCastling;
    }

    uint64_t hash;
};

class RepetitionHistory 
{
public:
    RepetitionHistory() = default;

    void addPosition(ZobristHash hash)
    {
        this->hashes.push_back(hash);
    }

    void clear()
    {
        this->hashes.clear();
    }

    uint8_t getRepetionNumber(ZobristHash hash)
    {
        uint8_t ctr = 0;

        for (auto hash1 : this->hashes)
        {
            if (hash == hash1)
            {
                ctr = ctr + 1;
            }
        }

        return ctr;
    }
private:
    vector<ZobristHash> hashes;
};

class Move 
{
public:
    Move() = default;
    Move(uint8_t from, uint8_t to, uint8_t attackerType, uint8_t attackerSide, uint8_t defenderType, uint8_t defenderSide, uint8_t flag = 0)
    {
        this->From = from;
        this->To = to;

        this->AttackerType = attackerType;
        this->AttackerSide = attackerSide;

        this->DefenderType = defenderType;
        this->DefenderSide = defenderSide;

        this->Flag = flag;
    }

    friend bool operator ==(Move left, Move right)
    {
        return (left.From == right.From and left.To == right.To and left.AttackerType == right.AttackerType and left.AttackerSide == right.AttackerSide and left.DefenderType == right.DefenderType and left.DefenderSide == right.DefenderSide and left.Flag == right.Flag);
    }

    uint8_t From;
    uint8_t To;

    uint8_t AttackerType;
    uint8_t AttackerSide;

    uint8_t DefenderType;
    uint8_t DefenderSide;

    uint8_t Flag;

    struct Flag 
    {
        static constexpr uint8_t Default = 0;

        static constexpr uint8_t PawnLongMove = 1;
        static constexpr uint8_t EnPassantCapture = 2;

        static constexpr uint8_t WhiteLongCastling = 3;
        static constexpr uint8_t WhiteShortCastling = 4;
        static constexpr uint8_t BlackLongCastling = 5;
        static constexpr uint8_t BlackShortCastling = 6;

        static constexpr uint8_t PromoteToKnight = 7;
        static constexpr uint8_t PromoteToBishop = 8;
        static constexpr uint8_t PromoteToRook = 9;
        static constexpr uint8_t PromoteToQueen = 10;
    };
};

class Position 
{
public:

    Pieces pieces;
    uint8_t EnPassant;

    bool WhiteLongCastling;
    bool WhiteShortCastling;
    bool BlackLongCastling;
    bool BlackShortCastling;

    bool whiteCastlingHappened;
    bool blackCastlingHappened;

    float MoveCtr;
    ZobristHash hash;
    float fiftyMovesCtr;
    RepetitionHistory repetitionHistory;

    Position() = default;
    Position(const string& shortFen, uint8_t enPassant, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling, float moveCtr)
    {   
        this->pieces = {shortFen};
        this->EnPassant = enPassant;

        this->WhiteLongCastling = whiteLongCastling;
        this->WhiteShortCastling = whiteShortCastling;
        this->BlackLongCastling = blackLongCastling;
        this->BlackShortCastling = blackShortCastling;

        // These variables are used by an AI.
        this->whiteCastlingHappened = false;
        this->blackCastlingHappened = false;

        this->MoveCtr = moveCtr;
        this->hash = {this->pieces, (this->MoveCtr - floor(this->MoveCtr) > 1e-4), this->WhiteLongCastling, this->WhiteShortCastling, this->BlackLongCastling, this->BlackShortCastling};
        this->repetitionHistory.addPosition(this->hash);
        this->fiftyMovesCtr = 0;
    }

    friend ostream& operator <<(ostream& ostream, Position position)
    {
        ostream << position.pieces;

        ostream << "En passant: " << (uint32_t)position.EnPassant << "\n";
        ostream << "White long castling: " << position.WhiteLongCastling << "\n";
        ostream << "White short castling: " << position.WhiteShortCastling << "\n";
        ostream << "Black long castling: " << position.BlackLongCastling << "\n";
        ostream << "Black short castling: " << position.BlackShortCastling << "\n";
        ostream << "Move counter: " << position.MoveCtr << "\n";
        ostream << "Zobrist hash: " << hex << "0x" << position.hash.hash << "\n" << dec;
        ostream << "Fifty moves counter: " << position.fiftyMovesCtr << "\n";
        ostream << "Threefold repetition counter: " << (uint32_t)position.repetitionHistory.getRepetionNumber(position.hash) << "\n";

        return ostream;
    }

    void move(Move move)
    {
        this->removePiece(move.From, move.AttackerType, move.AttackerSide);
        this->addPiece(move.To, move.AttackerType, move.AttackerSide);

        if (move.DefenderType != 255)
        {
            this->removePiece(move.To, move.DefenderType, move.DefenderSide);
        }

        switch (move.Flag) 
        {
            case Move::Flag::Default:
            break;

            case Move::Flag::PawnLongMove:
                this->changeEnPassant((move.From + move.To) / 2);
            break;

            case Move::Flag::EnPassantCapture:
                if (move.AttackerSide == Pieces::White)
                {
                    this->removePiece(move.To - 8, Pieces::Pawn, Pieces::Black);
                }
                else
                {
                    this->removePiece(move.To + 8, Pieces::Pawn, Pieces::White);
                }
            break;

            case Move::Flag::WhiteLongCastling:
                this->removePiece(0, Pieces::Rook, Pieces::White);
                this->addPiece(3, Pieces::Rook, Pieces::White);
                this->whiteCastlingHappened = true;
            break;

            case Move::Flag::WhiteShortCastling:
                this->removePiece(7, Pieces::Rook, Pieces::White);
                this->addPiece(5, Pieces::Rook, Pieces::White);
                this->whiteCastlingHappened = true;
            break;

            case Move::Flag::BlackLongCastling:
                this->removePiece(56, Pieces::Rook, Pieces::Black);
                this->addPiece(59, Pieces::Rook, Pieces::Black);
                this->blackCastlingHappened = true;
            break;

            case Move::Flag::BlackShortCastling:
                this->removePiece(63, Pieces::Rook, Pieces::Black);
                this->addPiece(61, Pieces::Rook, Pieces::Black);
                this->blackCastlingHappened = true;
            break;

            case Move::Flag::PromoteToKnight:
                this->removePiece(move.To, Pieces::Pawn, move.AttackerSide);
                this->addPiece(move.To, Pieces::Knight, move.AttackerSide);
            break;

            case Move::Flag::PromoteToBishop:
                this->removePiece(move.To, Pieces::Pawn, move.AttackerSide);
                this->addPiece(move.To, Pieces::Bishop, move.AttackerSide);
            break;

            case Move::Flag::PromoteToRook:
                this->removePiece(move.To, Pieces::Pawn, move.AttackerSide);
                this->addPiece(move.To, Pieces::Rook, move.AttackerSide);
            break;

            case Move::Flag::PromoteToQueen:
                this->removePiece(move.To, Pieces::Pawn, move.AttackerSide);
                this->addPiece(move.To, Pieces::Queen, move.AttackerSide);
            break;
        }

        this->pieces.updateBitboards();

        if (move.Flag != Move::Flag::PawnLongMove)
        {
            this->changeEnPassant(255);
        }

        switch (move.From) 
        {
            case 0:
                this->removeWhiteLongCastling();
            break;

            case 4:
                this->removeWhiteLongCastling();
                this->removeWhiteShortCastling();
            break;

            case 7:
                this->removeWhiteShortCastling();
            break;

            case 56:
                this->removeBlackLongCastling();
            break;

            case 60:
                this->removeBlackLongCastling();
                this->removeBlackShortCastling();
            break;

            case 63:
                this->removeBlackShortCastling();
            break;
        }

        this->updateMoveCtr();

        this->updateFiftyMovesCtr(move.AttackerType == Pieces::Pawn or move.DefenderType != 255);

        if (move.AttackerType == Pieces::Pawn or move.DefenderType != 255)
        {
            this->repetitionHistory.clear();
        }

        this->repetitionHistory.addPosition(this->hash);
    }

    
private:
    void addPiece(uint8_t square, uint8_t type, uint8_t side)
    {
        if (!getBit(this->pieces.pieceBitboards[side][type], square)) 
        {
            setOne(this->pieces.pieceBitboards[side][type], square);
            this->hash.invertPiece(square, type, side);
        }
    }
    void removePiece(uint8_t square, uint8_t type, uint8_t side)
    {
        if (getBit(this->pieces.pieceBitboards[side][type], square)) 
        {
            setZero(this->pieces.pieceBitboards[side][type], square);
            this->hash.invertPiece(square, type, side);
        }
    }
    void changeEnPassant(uint8_t enPassant)
    {
        this->EnPassant = enPassant;
    }

    void removeWhiteLongCastling()
    {
        if (this->WhiteLongCastling) 
        {
            this->WhiteLongCastling = false;
            this->hash.invertWhiteLongCastling();
        }
    }
    void removeWhiteShortCastling()
    {
        if (this->WhiteShortCastling) 
        {
            this->WhiteShortCastling = false;
            this->hash.invertWhiteShortCastling();
        }
    }
    void removeBlackLongCastling()
    {
        if (this->BlackLongCastling) 
        {
            this->BlackLongCastling = false;
            this->hash.invertBlackLongCastling();
        }
    }
    void removeBlackShortCastling()
    {
        if (this->BlackShortCastling) 
        {
            this->BlackShortCastling = false;
            this->hash.invertBlackShortCastling();
        }
    }

    void updateMoveCtr()
    {
        this->MoveCtr = this->MoveCtr + 0.5f;
        this->hash.invertMove();
    }
    void updateFiftyMovesCtr(bool breakEvent)
    {
        if (breakEvent)
        {
            this->fiftyMovesCtr = 0;
        } else
        {
            this->fiftyMovesCtr = this->fiftyMovesCtr + 0.5f;
        }
    };
};

namespace KingMasks 
{
    static consteval uint8_t absSubtract(uint8_t left, uint8_t right) 
    {
        if (left >= right)
        {
            return left - right;
        }

        return right - left;
    }

    static consteval array<Bitboard, 64> calculateMasks() 
    {
        array<Bitboard, 64> masks{};

        uint8_t dx;
        uint8_t dy;

        for (uint8_t x0 = 0; x0 < 8; x0 = x0 + 1) 
        {
            for (uint8_t y0 = 0; y0 < 8; y0 = y0 + 1) 
            {

                for (uint8_t x1 = 0; x1 < 8; x1 = x1 + 1) 
                {
                    for (uint8_t y1 = 0; y1 < 8; y1 = y1 + 1) 
                    {

                        dx = KingMasks::absSubtract(x0, x1);
                        dy = KingMasks::absSubtract(y0, y1);

                        if (dx <= 1 and dy <= 1)
                        {
                            setOne(masks[y0 * 8 + x0], y1 * 8 + x1);
                        } 
                    }
                }
            }
        }

        return masks;
    }


    static constexpr array<Bitboard, 64> Masks = KingMasks::calculateMasks();
}

namespace KnightMasks 
{
    static consteval uint8_t absSubtract(uint8_t left, uint8_t right) 
    {
        if (left >= right)
        {
            return left - right;
        }
        return right - left;
    }

    static consteval array<Bitboard, 64> calculateMasks() 
    {
        array<Bitboard, 64> masks{};

        uint8_t dx;
        uint8_t dy;

        for (uint8_t x0 = 0; x0 < 8; x0 = x0 + 1) 
        {
            for (uint8_t y0 = 0; y0 < 8; y0 = y0 + 1) 
            {

                for (uint8_t x1 = 0; x1 < 8; x1 = x1 + 1) 
                {
                    for (uint8_t y1 = 0; y1 < 8; y1 = y1 + 1) 
                    {

                        dx = KnightMasks::absSubtract(x0, x1);
                        dy = KnightMasks::absSubtract(y0, y1);

                        if ((dx == 2 and dy == 1) or (dx == 1 and dy == 2))
                        {
                            setOne(masks[y0 * 8 + x0], y1 * 8 + x1);
                        } 
                    }
                }
            }
        }

        return masks;
    }


    static constexpr array<Bitboard, 64> Masks = KnightMasks::calculateMasks();
}

namespace SlidersMasks 
{
    struct Direction 
    {
        static constexpr int8_t North = 0;
        static constexpr int8_t South = 1;
        static constexpr int8_t West = 2;
        static constexpr int8_t East = 3;

        static constexpr int8_t NorthWest = 4;
        static constexpr int8_t NorthEast = 5;
        static constexpr int8_t SouthWest = 6;
        static constexpr int8_t SouthEast = 7;
    };


    static consteval Bitboard calculateMask(uint8_t p, int8_t direction) 
    {
        Bitboard mask = 0;

        int8_t x = p % 8;
        int8_t y = p / 8;

        for (; ;) 
        {
            switch (direction) 
            {
                case SlidersMasks::Direction::North: 
                    y = y + 1; 
                break;

                case SlidersMasks::Direction::South: 
                    y = y - 1; 
                break;

                case SlidersMasks::Direction::West: 
                    x = x - 1; 
                break;

                case SlidersMasks::Direction::East: 
                    x = x + 1; 
                break;

                case SlidersMasks::Direction::NorthWest:
                    y = y + 1;  
                    x = x - 1; 
                break;

                case SlidersMasks::Direction::NorthEast: 
                    y = y + 1; 
                    x = x + 1; 
                break;

                case SlidersMasks::Direction::SouthWest: 
                    y = y - 1; 
                    x = x - 1; 
                break;

                case SlidersMasks::Direction::SouthEast: 
                    y = y - 1; 
                    x = x + 1; 
                break;
            }

            if (x > 7 or x < 0 or y > 7 or y < 0)
            {
                break;
            }

            setOne(mask, y * 8 + x);
        }

        return mask;
    }


    static consteval array<array<Bitboard, 8>, 64> calculateMasks() 
    {
        array<array<Bitboard, 8>, 64> masks{};

        for (uint8_t i = 0; i < 64; i = i + 1) 
        {
            for (uint8_t j = 0; j < 8; j = j + 1)
            {
                masks[i][j] = SlidersMasks::calculateMask(i, j);
            }
        }

        return masks;
    }


    static constexpr std::array<std::array<Bitboard, 8>, 64> Masks = SlidersMasks::calculateMasks();
};

class PsLegalMoveMaskGen 
{
public:
    static Bitboard generatePawnDafaultMask(Pieces pieces, uint8_t side)
    {
        if (side == Pieces::White) 
        {
            return (pieces.pieceBitboards[Pieces::White][Pieces::Pawn] << 8) & pieces.empty;
        }

        return (pieces.pieceBitboards[Pieces::Black][Pieces::Pawn] >> 8) & pieces.empty;
    }
    static Bitboard generatePawnLongMask(Pieces pieces, uint8_t side)
    {
        Bitboard defaultMask = PsLegalMoveMaskGen::generatePawnDafaultMask(pieces, side);

        if (side == Pieces::White) 
        {
            return ((defaultMask & BitboardRows::Rows[2]) << 8) & pieces.empty;
        }

        return ((defaultMask & BitboardRows::Rows[5]) >> 8) & pieces.empty;
    }
    static Bitboard generatePawnLeftCapturesMask(Pieces pieces, uint8_t side, bool includeAllPossibleCaptures)
    {
        if (side == Pieces::White) 
        {
            Bitboard mask = (pieces.pieceBitboards[Pieces::White][Pieces::Pawn] << 7) & BitboardColumns::InversionColumns[7];

            if (!includeAllPossibleCaptures)
            {
                mask = mask & pieces.sideBitboards[Pieces::Black];
            }

            return mask;
        }

        Bitboard mask = (pieces.pieceBitboards[Pieces::Black][Pieces::Pawn] >> 9) & BitboardColumns::InversionColumns[7];

        if (!includeAllPossibleCaptures)
        {
            mask = mask & pieces.sideBitboards[Pieces::White];
        } 

        return mask;
    }
    static Bitboard generatePawnRightCapturesMask(Pieces pieces, uint8_t side, bool includeAllPossibleCaptures)
    {
        if (side == Pieces::White) 
        {
            Bitboard mask = (pieces.pieceBitboards[Pieces::White][Pieces::Pawn] << 9) & BitboardColumns::InversionColumns[0];

            if (!includeAllPossibleCaptures)
            {
                mask = mask & pieces.sideBitboards[Pieces::Black];
            }

            return mask;
        }

        Bitboard mask = (pieces.pieceBitboards[Pieces::Black][Pieces::Pawn] >> 7) & BitboardColumns::InversionColumns[0];

        if (!includeAllPossibleCaptures)
        {
            mask = mask & pieces.sideBitboards[Pieces::White];
        }

        return mask;
    }

    static Bitboard generateKnightMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures)
    {
        if (onlyCaptures) 
        {
            return KnightMasks::Masks[p] & pieces.sideBitboards[Pieces::inverse(side)];
        }

        return KnightMasks::Masks[p] & pieces.inversionSideBitboards[side];
    }
    static Bitboard generateBishopMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures)
    {
        Bitboard nw = PsLegalMoveMaskGen::calculateRay(pieces, p, side, onlyCaptures, SlidersMasks::Direction::NorthWest, false);
        Bitboard ne = PsLegalMoveMaskGen::calculateRay(pieces, p, side, onlyCaptures, SlidersMasks::Direction::NorthEast, false);
        Bitboard sw = PsLegalMoveMaskGen::calculateRay(pieces, p, side, onlyCaptures, SlidersMasks::Direction::SouthWest, true);
        Bitboard se = PsLegalMoveMaskGen::calculateRay(pieces, p, side, onlyCaptures, SlidersMasks::Direction::SouthEast, true);

        return nw | ne | sw | se;
    }
    static Bitboard generateRookMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures)
    {
        Bitboard n = PsLegalMoveMaskGen::calculateRay(pieces, p, side, onlyCaptures, SlidersMasks::Direction::North, false);
        Bitboard s = PsLegalMoveMaskGen::calculateRay(pieces, p, side, onlyCaptures, SlidersMasks::Direction::South, true);
        Bitboard w = PsLegalMoveMaskGen::calculateRay(pieces, p, side, onlyCaptures, SlidersMasks::Direction::West, true);
        Bitboard e = PsLegalMoveMaskGen::calculateRay(pieces, p, side, onlyCaptures, SlidersMasks::Direction::East, false);

        return n | s | w | e;
    }
    static Bitboard generateQueenMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures)
    {
        Bitboard bishopMask = PsLegalMoveMaskGen::generateBishopMask(pieces, p, side, onlyCaptures);
        Bitboard rookMask = PsLegalMoveMaskGen::generateRookMask(pieces, p, side, onlyCaptures);

        return bishopMask | rookMask;
    }
    static Bitboard generateKingMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures)
    {
        if (onlyCaptures) 
        {
            return KingMasks::Masks[p] & pieces.sideBitboards[Pieces::inverse(side)];
        }

        return KingMasks::Masks[p] & pieces.inversionSideBitboards[side];
    }

    static bool inDanger(Pieces pieces, uint8_t p, uint8_t side)
    {
        Bitboard opposite_pawns_left_captures = PsLegalMoveMaskGen::generatePawnLeftCapturesMask(pieces, Pieces::inverse(side), true);
        Bitboard opposite_pawns_right_captures = PsLegalMoveMaskGen::generatePawnRightCapturesMask(pieces, Pieces::inverse(side), true);
        Bitboard opposite_pawns_captures = opposite_pawns_left_captures | opposite_pawns_right_captures;

        if (getBit(opposite_pawns_captures, p))
        {
            return true;
        }

        if (PsLegalMoveMaskGen::generateKnightMask(pieces, p, side, true) & pieces.pieceBitboards[Pieces::inverse(side)][Pieces::Knight]) return true;
        if (PsLegalMoveMaskGen::generateBishopMask(pieces, p, side, true) & pieces.pieceBitboards[Pieces::inverse(side)][Pieces::Bishop]) return true;
        if (PsLegalMoveMaskGen::generateRookMask(pieces, p, side, true) & pieces.pieceBitboards[Pieces::inverse(side)][Pieces::Rook]) return true;
        if (PsLegalMoveMaskGen::generateQueenMask(pieces, p, side, true) & pieces.pieceBitboards[Pieces::inverse(side)][Pieces::Queen]) return true;
        if (PsLegalMoveMaskGen::generateKingMask(pieces, p, side, true) & pieces.pieceBitboards[Pieces::inverse(side)][Pieces::King]) return true;

        return false;
    }
private:
    static Bitboard calculateRay(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures, uint8_t direction, bool bsr)
    {
        Bitboard blockers = SlidersMasks::Masks[p][direction] & pieces.all;

        if (blockers == 0) {
            if (onlyCaptures) return 0;
            return SlidersMasks::Masks[p][direction];
        }

        uint8_t blockingSquare;

        if (bsr) blockingSquare = bsrFunc(blockers);
        else blockingSquare = bsf(blockers);

        Bitboard moves;

        if (onlyCaptures) moves = 0;
        else moves = SlidersMasks::Masks[p][direction] ^ SlidersMasks::Masks[blockingSquare][direction];

        if (getBit(pieces.sideBitboards[side], blockingSquare)) setZero(moves, blockingSquare);
        else setOne(moves, blockingSquare);

        return moves;
    }
};

class MoveList 
{
private:
    array<Move, 218> moves{};
    uint8_t Size;
    
public:
    MoveList()
    {
        this->Size = 0;
    }

    Move &operator[](uint8_t index)
    {
        return this->moves[index];
    }
    Move operator[](uint8_t index) const
    {
        return this->moves[index];
    }

    void push_back(Move move)
    {
        this->moves[this->Size] = move;
        this->Size = this->Size + 1;
    }
    [[nodiscard]] uint8_t size() const
    {
        return this->Size;
    }

};

class LegalMoveGen 
{
public:
    static MoveList generate(Position position, uint8_t side, bool only_captures = false)
    {

        MoveList moves;

        Bitboard pawnLeftCaptureMask = PsLegalMoveMaskGen::generatePawnLeftCapturesMask(position.pieces, side, false);
        Bitboard pawnRightCapturesMask = PsLegalMoveMaskGen::generatePawnRightCapturesMask(position.pieces, side, false);

        int8_t pawnLeftCapture;
        int8_t pawnRightCapture;

        if (side == Pieces::White) 
        {
            pawnLeftCapture = -7;
            pawnRightCapture = -9;
        } else 
        {
            pawnLeftCapture = 9;
            pawnRightCapture = 7;
        }

        LegalMoveGen::pawnMaskToMoves(position.pieces, pawnLeftCaptureMask, side, pawnLeftCapture, true,Move::Flag::Default, moves);
        LegalMoveGen::pawnMaskToMoves(position.pieces, pawnRightCapturesMask, side, pawnRightCapture, true,Move::Flag::Default, moves);

        if (!only_captures) {
            Bitboard pawnDafaultMask = PsLegalMoveMaskGen::generatePawnDafaultMask(position.pieces, side);
            Bitboard pawnLongMask = PsLegalMoveMaskGen::generatePawnLongMask(position.pieces, side);

            int8_t pawnDefaultMove;
            int8_t pawnLongMove;

            if (side == Pieces::White) 
            {
                pawnDefaultMove = -8;
                pawnLongMove = -16;
            } else 
            {
                pawnDefaultMove = 8;
                pawnLongMove = 16;
            }

            LegalMoveGen::pawnMaskToMoves(position.pieces, pawnDafaultMask, side, pawnDefaultMove, false,Move::Flag::Default, moves);
            LegalMoveGen::pawnMaskToMoves(position.pieces, pawnLongMask, side, pawnLongMove, false,Move::Flag::PawnLongMove, moves);
        }

        Bitboard allKnights = position.pieces.pieceBitboards[side][Pieces::Knight];
        Bitboard allBishops = position.pieces.pieceBitboards[side][Pieces::Bishop];
        Bitboard allRooks = position.pieces.pieceBitboards[side][Pieces::Rook];
        Bitboard allQueens = position.pieces.pieceBitboards[side][Pieces::Queen];

        uint8_t attackerPos;
        Bitboard mask;

        while (allKnights) 
        {
            attackerPos = bsf(allKnights);
            setZero(allKnights, attackerPos);
            mask = PsLegalMoveMaskGen::generateKnightMask(position.pieces, attackerPos, side, only_captures);
            LegalMoveGen::pieceMaskToMoves(position.pieces, mask, attackerPos, Pieces::Knight, side, moves);
        }

        while (allBishops) 
        {
            attackerPos = bsf(allBishops);
            setZero(allBishops, attackerPos);
            mask = PsLegalMoveMaskGen::generateBishopMask(position.pieces, attackerPos, side, only_captures);
            LegalMoveGen::pieceMaskToMoves(position.pieces, mask, attackerPos, Pieces::Bishop, side, moves);
        }

        while (allRooks) 
        {
            attackerPos = bsf(allRooks);
            setZero(allRooks, attackerPos);
            mask = PsLegalMoveMaskGen::generateRookMask(position.pieces, attackerPos, side, only_captures);
            LegalMoveGen::pieceMaskToMoves(position.pieces, mask, attackerPos, Pieces::Rook, side, moves);
        }

        while (allQueens) 
        {
            attackerPos = bsf(allQueens);
            setZero(allQueens, attackerPos);
            mask = PsLegalMoveMaskGen::generateQueenMask(position.pieces, attackerPos, side, only_captures);
            LegalMoveGen::pieceMaskToMoves(position.pieces, mask, attackerPos, Pieces::Queen, side, moves);
        }

        attackerPos = bsf(position.pieces.pieceBitboards[side][Pieces::King]);
        mask = PsLegalMoveMaskGen::generateKingMask(position.pieces, attackerPos, side, only_captures);
        LegalMoveGen::pieceMaskToMoves(position.pieces, mask, attackerPos, Pieces::King, side, moves);

        LegalMoveGen::addEnPassantCaptures(position.pieces, side, position.EnPassant, moves);

        if (!only_captures) 
        {
            if (side == Pieces::White)
            {
                LegalMoveGen::addCastlingMoves(position.pieces, Pieces::White, position.WhiteLongCastling,position.WhiteShortCastling, moves);
            } else 
            {
                LegalMoveGen::addCastlingMoves(position.pieces, Pieces::Black, position.BlackLongCastling, position.BlackShortCastling, moves);
            } 
        }

        return moves;
    }
private:
    static void pieceMaskToMoves(Pieces pieces, Bitboard mask, uint8_t attackerPos, uint8_t attackerType, uint8_t attackerSide, MoveList &moves)
    {
        uint8_t defenderPos;
        uint8_t defenderType;

        Move move;

        while (mask) {
            defenderPos = bsf(mask);
            setZero(mask, defenderPos);

            defenderType = 255;

            for (uint8_t i = 0; i < 6; i = i + 1) 
            {
                if (getBit(pieces.pieceBitboards[Pieces::inverse(attackerSide)][i], defenderPos)) 
                {
                    defenderType = i;
                    break;
                }
            }

            move = {attackerPos, defenderPos, attackerType, attackerSide, defenderType, Pieces::inverse(attackerSide)};

            if (LegalMoveGen::isLegal(pieces, move, false))
            {
                moves.push_back(move);
            }
        }
    }
    static void pawnMaskToMoves(Pieces pieces, Bitboard mask, uint8_t attackerSide, int8_t attackerIndex, bool lookForDefender, uint8_t flag, MoveList &moves)
    {
        uint8_t defenderPos;
        uint8_t defenderType = 255;

        Move move;

        while (mask) 
        {
            defenderPos = bsf(mask);
            setZero(mask, defenderPos);

            if (lookForDefender) 
            {
                defenderType = 255;

                for (uint8_t i = 0; i < 6; i = i + 1) 
                {
                    if (getBit(pieces.pieceBitboards[Pieces::inverse(attackerSide)][i], defenderPos)) 
                    {
                        defenderType = i;
                        break;
                    }
                }
            }

            move = {(uint8_t)(defenderPos + attackerIndex), defenderPos, Pieces::Pawn, attackerSide, defenderType, Pieces::inverse(attackerSide), flag};

            if (LegalMoveGen::isLegal(pieces, move, false)) 
            {
                if (defenderPos < 8 or defenderPos > 55) 
                {
                    moves.push_back({(uint8_t)(defenderPos + attackerIndex), defenderPos, 0, attackerSide, defenderType, Pieces::inverse(attackerSide), Move::Flag::PromoteToKnight});
                    moves.push_back({(uint8_t)(defenderPos + attackerIndex), defenderPos, 0, attackerSide, defenderType, Pieces::inverse(attackerSide), Move::Flag::PromoteToBishop});
                    moves.push_back({(uint8_t)(defenderPos + attackerIndex), defenderPos, 0, attackerSide, defenderType, Pieces::inverse(attackerSide), Move::Flag::PromoteToRook});
                    moves.push_back({(uint8_t)(defenderPos + attackerIndex), defenderPos, 0, attackerSide, defenderType, Pieces::inverse(attackerSide), Move::Flag::PromoteToQueen});
                } else
                {
                    moves.push_back(move);
                }
            }
        }
    }

    static bool isLegal(Pieces pieces, Move move, bool enPassantCapture)
    {
        setZero(pieces.pieceBitboards[move.AttackerSide][move.AttackerType], move.From);
        setOne(pieces.pieceBitboards[move.AttackerSide][move.AttackerType], move.To);

        if (move.DefenderType != 255)
        {
            setZero(pieces.pieceBitboards[move.DefenderSide][move.DefenderType], move.To);
        } 

        if (enPassantCapture) 
        {
            if (move.AttackerSide == Pieces::White)
            {
                setZero(pieces.pieceBitboards[Pieces::Black][Pieces::Pawn], move.To - 8);
            } 

            setZero(pieces.pieceBitboards[Pieces::White][Pieces::Pawn], move.To + 8);
        }

        pieces.updateBitboards();

        if (PsLegalMoveMaskGen::inDanger(pieces, bsf(pieces.pieceBitboards[move.AttackerSide][Pieces::King]), move.AttackerSide))
        {
            return false;
        } 

        return true;
    }

    static void addEnPassantCaptures(Pieces pieces, uint8_t side, uint8_t enPassant, MoveList &moves)
    {
        if (enPassant == 255) return;

        Move move;

        if (side == Pieces::White) 
        {
            if (enPassant % 8 != 7 and getBit(pieces.pieceBitboards[Pieces::White][Pieces::Pawn], enPassant - 7)) 
            {
                move = {(uint8_t)(enPassant - 7), enPassant, Pieces::Pawn, Pieces::White, 255, 255, Move::Flag::EnPassantCapture};

                if (LegalMoveGen::isLegal(pieces, move, true))
                {
                    moves.push_back(move);
                }
            }

            if (enPassant % 8 != 0 and getBit(pieces.pieceBitboards[Pieces::White][Pieces::Pawn], enPassant - 9)) 
            {
                move = {(uint8_t)(enPassant - 9), enPassant, Pieces::Pawn, Pieces::White, 255, 255, Move::Flag::EnPassantCapture};

                if (LegalMoveGen::isLegal(pieces, move, true))
                {
                    moves.push_back(move);
                }
            }
        } else 
        {
            if (enPassant % 8 != 0 and getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Pawn], enPassant + 7)) 
            {
                move = {(uint8_t)(enPassant + 7), enPassant, Pieces::Pawn, Pieces::Black, 255, 255, Move::Flag::EnPassantCapture};

                if (LegalMoveGen::isLegal(pieces, move, true))
                {
                    moves.push_back(move);
                }
            }

            if (enPassant % 8 != 7 and getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Pawn], enPassant + 9)) 
            {
                move = {(uint8_t)(enPassant + 9), enPassant, Pieces::Pawn, Pieces::Black, 255, 255, Move::Flag::EnPassantCapture};

                if (LegalMoveGen::isLegal(pieces, move, true))
                {
                    moves.push_back(move);
                }
            }
        }
    }
    static void addCastlingMoves(Pieces pieces, uint8_t side, bool longCastling, bool shortCastling, MoveList &moves)
    {
        uint8_t index;
        uint8_t long_castling_flag;
        uint8_t short_castling_flag;

        if (side == Pieces::White) 
        {
            index = 0;
            long_castling_flag = Move::Flag::WhiteLongCastling;
            short_castling_flag = Move::Flag::WhiteShortCastling;
        } else 
        {
            index = 56;
            long_castling_flag = Move::Flag::BlackLongCastling;
            short_castling_flag = Move::Flag::BlackShortCastling;
        }

        if (longCastling and getBit(pieces.pieceBitboards[side][Pieces::Rook], 0 + index) and getBit(pieces.empty, 1 + index) and getBit(pieces.empty, 2 + index) and getBit(pieces.empty, 3 + index)) 
        {
            if (!PsLegalMoveMaskGen::inDanger(pieces, bsf(pieces.pieceBitboards[side][Pieces::King]), side) and !PsLegalMoveMaskGen::inDanger(pieces, 2 + index, side) and !PsLegalMoveMaskGen::inDanger(pieces, 3 + index, side)) moves.push_back({(uint8_t)(4 + index), (uint8_t)(2 + index), Pieces::King, side, 255, 255, long_castling_flag});
        }

        if (shortCastling and getBit(pieces.pieceBitboards[side][Pieces::Rook], 7 + index) and getBit(pieces.empty, 5 + index) and getBit(pieces.empty, 6 + index)) 
        {
            if (!PsLegalMoveMaskGen::inDanger(pieces, bsf(pieces.pieceBitboards[side][Pieces::King]), side) and !PsLegalMoveMaskGen::inDanger(pieces, 5 + index, side) and !PsLegalMoveMaskGen::inDanger(pieces, 6 + index, side)) moves.push_back({(uint8_t)(4 + index), (uint8_t)(6 + index), Pieces::King, side, 255, 255, short_castling_flag});
        }
    }
};

class LegalMoveGenTester 
{
public:
    static void test()
    {
        Position position = {(std::string)LegalMoveGenTester::Fen, LegalMoveGenTester::EnPassant, LegalMoveGenTester::WLCastling, LegalMoveGenTester::WSCastling, LegalMoveGenTester::BLCastling, LegalMoveGenTester::BSCastling, 1};

        uint64_t correct;
        uint64_t got;

        uint64_t timeStart;
        float speed;

        for (uint32_t i = 0; i < 6; i = i + 1) 
        {
            timeStart = nsecs;

            correct = LegalMoveGenTester::Nodes[i];
            got = LegalMoveGenTester::getNodesNumber(position, Pieces::White, i);

            speed = (float)got / ((float)(nsecs - timeStart) / (float)1e+9) / (float)1e+6;

            if (correct == got) std::cout << "Depth " << setw(4) << i << ". Correct: " << setw(18) << correct << ". Got: " << setw(18) << got << ". Speed: " << setw(10) << speed << " MNPS. OK." << endl;
            else std::cout  << "Depth " << std::setw(4) << i << ". Correct: " << setw(18) << correct << ". Got: " << setw(18) << got << ". Speed: " << setw(10) << speed << " MNPS. Error." << endl;
        }
    }
private:
    static uint64_t getNodesNumber(const Position& position, uint8_t side, uint32_t depth)
    {
        if (depth == 0) return 1;

        uint64_t ctr = 0;

        Position copy = position;

        MoveList moves = LegalMoveGen::generate(copy, side);
        Move move;

        for (uint8_t i = 0; i < moves.size(); i = i + 1) 
        {
            move = moves[i];

            copy = position;
            copy.move(move);
            ctr = ctr + LegalMoveGenTester::getNodesNumber(copy, Pieces::inverse(side), depth - 1);
        }

        return ctr;
    }

    static constexpr string_view Fen = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R";
    static constexpr uint8_t EnPassant = 255;
    static constexpr bool WLCastling = true;
    static constexpr bool WSCastling = true;
    static constexpr bool BLCastling = false;
    static constexpr bool BSCastling = false;

    static constexpr array<uint64_t, 6> Nodes = {1, 44, 1486, 62379, 2103487, 89941194};
};



//AI

static atomic<bool> stopSearch;


static int64_t evaluated;
static int32_t maxDepth;
static int32_t cutOffs;

class OpeningBook 
{
public:
    OpeningBook() = default;
    OpeningBook(const string& path)
    {
        ifstream file(path);

        if (!file.is_open()) 
        {
            cout  << "Could not find the opening book." << endl;
            exit(255);
        }

        string game;
        stringstream game_thread;

        string string_move;
        uint8_t from;
        uint8_t to;

        MoveList possibleMoves;
        bool move_found;

        Position buff;

        while (getline(file, game)) 
        {
            game_thread = std::stringstream(game);
            this->moves.resize(this->moves.size() + 1);

            buff = {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 255, true, true, true, true, 1};

            while (game_thread >> string_move and game_thread.good()) 
            {
                from = (string_move[1] - '1') * 8 + string_move[0] - 'a';
                to = (string_move[3] - '1') * 8 + string_move[2] - 'a';

                possibleMoves = LegalMoveGen::generate(buff, buff.MoveCtr - std::floor(buff.MoveCtr) > 1e-7);
                move_found = false;

                for (uint8_t i = 0; i < possibleMoves.size(); i = i + 1) 
                {
                    if (possibleMoves[i].From == from and possibleMoves[i].To == to) 
                    {
                        this->moves.back().push_back(possibleMoves[i]);

                        buff.move(possibleMoves[i]);
                        move_found = true;

                        break;
                    }
                }

                if (!move_found) 
                {
                    cout << "Error in the opening book." << std::endl;
                    exit(255);
                }
            }
        }

        file.close();
    }

    tuple<Move, int32_t> tryToFindMove(const Position& position)
    {
        Position buff;

        vector<Move> possibleMoves;
        bool moveExist;

        for (int32_t game = 0; game < this->moves.size(); game = game + 1) 
        {
            buff = {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 255, true, true, true, true, 1};

            if (buff.pieces == position.pieces) 
            {
                moveExist = false;

                for (auto added_move : possibleMoves) 
                {
                    if (added_move == this->moves[game][0]) 
                    {
                        moveExist = true;
                        break;
                    }
                }

                if (!moveExist) possibleMoves.push_back(this->moves[game][0]);
                continue;
            }

            for (int32_t move = 0; move < this->moves[game].size() - 1; move = move + 1) 
            {
                buff.move(this->moves[game][move]);

                if (buff.pieces == position.pieces) 
                {
                    moveExist = false;
                    for (auto addedMove : possibleMoves) 
                    {
                        if (addedMove == this->moves[game][move + 1]) 
                        {
                            moveExist = true;
                            break;
                        }
                    }

                    if (!moveExist) possibleMoves.push_back(this->moves[game][move + 1]);
                }
            }
        }

        if (possibleMoves.empty()) 
        {
            return make_tuple(Move(), 0);
        }

        return make_tuple(possibleMoves[time(nullptr) % possibleMoves.size()], possibleMoves.size());
    }

private:
    vector<vector<Move>> moves;
};

namespace PawnShieldMasks 
{
    static consteval array<Bitboard, 64> calculateWhitePawnShieldMasks() 
    {
        array<Bitboard, 64> whitePawnShieldMasks{};

        for (uint8_t x = 0; x < 8; x = x + 1) 
        {
            for (uint8_t y = 0; y < 7; y = y + 1) 
            {
                if (x != 0) setOne(whitePawnShieldMasks[y * 8 + x], (y + 1) * 8 + x - 1);
                if (x != 7) setOne(whitePawnShieldMasks[y * 8 + x], (y + 1) * 8 + x + 1);
                setOne(whitePawnShieldMasks[y * 8 + x], (y + 1) * 8 + x);

                if (y != 6) 
                {
                    if (x != 0) setOne(whitePawnShieldMasks[y * 8 + x], (y + 2) * 8 + x - 1);
                    if (x != 7) setOne(whitePawnShieldMasks[y * 8 + x], (y + 2) * 8 + x + 1);
                    setOne(whitePawnShieldMasks[y * 8 + x], (y + 2) * 8 + x);
                }
            }
        }

        return whitePawnShieldMasks;
    }


    static consteval array<Bitboard, 64> calculatePawnShieldMasks() 
    {
        array<Bitboard, 64> blackPawnShieldMasks{};

        for (uint8_t x = 0; x < 8; x = x + 1) 
        {
            for (uint8_t y = 1; y < 8; y = y + 1) 
            {
                if (x != 0) setOne(blackPawnShieldMasks[y * 8 + x], (y - 1) * 8 + x - 1);
                if (x != 7) setOne(blackPawnShieldMasks[y * 8 + x], (y - 1) * 8 + x + 1);
                setOne(blackPawnShieldMasks[y * 8 + x], (y - 1) * 8 + x);

                if (y != 1) 
                {
                    if (x != 0) setOne(blackPawnShieldMasks[y * 8 + x], (y - 2) * 8 + x - 1);
                    if (x != 7) setOne(blackPawnShieldMasks[y * 8 + x], (y - 2) * 8 + x + 1);
                    setOne(blackPawnShieldMasks[y * 8 + x], (y - 2) * 8 + x);
                }
            }
        }

        return blackPawnShieldMasks;
    }


    static constexpr array<Bitboard, 64> WhitePawnShieldMasks = PawnShieldMasks::calculateWhitePawnShieldMasks();
    static constexpr array<Bitboard, 64> BlackPawnShieldMasks = PawnShieldMasks::calculatePawnShieldMasks();
}

namespace PassedPawnMasks 
{
    static consteval array<Bitboard, 64> calculateWhitePassedPawnMasks() 
    {
        array<Bitboard, 64> masks{};

        for (uint8_t x = 0; x < 8; x = x + 1) 
        {
            for (uint8_t y = 0; y < 8; y = y + 1) 
            {

                for (uint8_t y1 = y + 1; y1 < 8; y1 = y1 + 1) 
                {
                    if (x != 0) setOne(masks[y * 8 + x], y1 * 8 + x - 1);
                    if (x != 7) setOne(masks[y * 8 + x], y1 * 8 + x + 1);
                    setOne(masks[y * 8 + x], y1 * 8 + x);
                }
            }
        }

        return masks;
    }


    static consteval array<Bitboard, 64> calculateBlackPassedPawnMasks() 
    {
        array<Bitboard, 64> masks{};

        for (uint8_t x = 0; x < 8; x = x + 1) 
        {
            for (uint8_t y = 0; y < 8; y = y + 1) 
            {

                for (int8_t y1 = y - 1; y1 >= 0; y1 = y1 - 1) 
                {
                    if (x != 0) setOne(masks[y * 8 + x], y1 * 8 + x - 1);
                    if (x != 7) setOne(masks[y * 8 + x], y1 * 8 + x + 1);
                    setOne(masks[y * 8 + x], y1 * 8 + x);
                }
            }
        }

        return masks;
    }


    static constexpr array<Bitboard, 64> WhitePassedPawnMasks = PassedPawnMasks::calculateWhitePassedPawnMasks();
    static constexpr array<Bitboard, 64> BlackPassedPawnMasks = PassedPawnMasks::calculateBlackPassedPawnMasks();
}

class StaticEvaluator 
{
public:
    static int32_t evaluate(Pieces pieces, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling, bool whiteCastlingHappened, bool blackCastlingHappened, bool showDebugInfo = false)
    {
        int32_t evaluation = 0;

        int32_t material = StaticEvaluator::material(pieces);
        int32_t mobility = StaticEvaluator::mobility(pieces);
        int32_t double_pawn = StaticEvaluator::pawnStructureDoublePawn(pieces);
        int32_t connected_pawn = StaticEvaluator::pawnStructureConnectedPawn(pieces);
        int32_t pawn_promotion = StaticEvaluator::pawnStructurePawnPromotion(pieces);
        int32_t crashed_castling = StaticEvaluator::kingSafetyCrashedCastling(whiteLongCastling, whiteShortCastling, blackLongCastling, blackShortCastling, whiteCastlingHappened, blackCastlingHappened);
        int32_t pawn_shield = StaticEvaluator::kingSafetyPawnShield(pieces, whiteCastlingHappened, blackCastlingHappened);
        int32_t two_bishops = StaticEvaluator::twoBishops(pieces);
        int32_t endgame = StaticEvaluator::endgame(pieces, material >= 0);

        evaluation = evaluation + material;
        evaluation = evaluation + mobility;
        evaluation = evaluation + double_pawn;
        evaluation = evaluation + connected_pawn;
        evaluation = evaluation + pawn_promotion;
        evaluation = evaluation + crashed_castling;
        evaluation = evaluation + pawn_shield;
        evaluation = evaluation + two_bishops;
        evaluation = evaluation + endgame;

        if (showDebugInfo) 
        {
            cout << "Details of static evaluation of current position." << endl;
            cout << "Material: " << (float)material / 100.0f << " pawns." << endl;
            cout << "Mobility: " << (float)mobility / 100.0f << " pawns." << endl;
            cout << "Double pawn: " << (float)double_pawn / 100.0f << " pawns." << endl;
            cout << "Connected pawn: " << (float)connected_pawn / 100.0f << " pawns." << endl;
            cout << "Pawn promotion: " << (float)pawn_promotion / 100.0f << " pawns." << endl;
            cout << "Crashed castling: " << (float)crashed_castling / 100.0f << " pawns." << endl;
            cout << "Pawn shield: " << (float)pawn_shield / 100.0f << " pawns." << endl;
            cout << "Two bishops: " << (float)two_bishops / 100.0f << " pawns." << endl;
            cout << "Endgame: " << (float)endgame / 100.0f << " pawns." << endl;
            cout << "Sum: " << (float)evaluation / 100.0f << " pawns." << endl;
        }

        return evaluation;
    }

private:
    static int32_t material(Pieces pieces)
    {
        int32_t material = 0;
    
        material = material + StaticEvaluator::Material::Pawn * (countOnes(pieces.pieceBitboards[Pieces::White][Pieces::Pawn]) - countOnes(pieces.pieceBitboards[Pieces::Black][Pieces::Pawn]));
        material = material + StaticEvaluator::Material::Knight * (countOnes(pieces.pieceBitboards[Pieces::White][Pieces::Knight]) - countOnes(pieces.pieceBitboards[Pieces::Black][Pieces::Knight]));
        material = material + StaticEvaluator::Material::Bishop * (countOnes(pieces.pieceBitboards[Pieces::White][Pieces::Bishop]) - countOnes(pieces.pieceBitboards[Pieces::Black][Pieces::Bishop]));
        material = material + StaticEvaluator::Material::Rook * (countOnes(pieces.pieceBitboards[Pieces::White][Pieces::Rook]) - countOnes(pieces.pieceBitboards[Pieces::Black][Pieces::Rook]));
        material = material + StaticEvaluator::Material::Queen * (countOnes(pieces.pieceBitboards[Pieces::White][Pieces::Queen]) - countOnes(pieces.pieceBitboards[Pieces::Black][Pieces::Queen]));
    
        return material;
    }

    static int32_t mobility(Pieces pieces)
    {
        int32_t mobility = 0;

        array<array<Bitboard, 6>, 2> iteration_masks = pieces.pieceBitboards;
        uint8_t index;

        int32_t knight_moves = 0;
        int32_t bishop_moves = 0;
        int32_t rook_moves = 0;
        int32_t queen_moves = 0;

        while (iteration_masks[Pieces::White][Pieces::Knight]) 
        {
            index = bsf(iteration_masks[Pieces::White][Pieces::Knight]);
            setZero(iteration_masks[Pieces::White][Pieces::Knight], index);
            knight_moves = knight_moves + countOnes(PsLegalMoveMaskGen::generateKnightMask(pieces, index, Pieces::White, false));
        }

        while (iteration_masks[Pieces::White][Pieces::Bishop]) 
        {
            index = bsf(iteration_masks[Pieces::White][Pieces::Bishop]);
            setZero(iteration_masks[Pieces::White][Pieces::Bishop], index);
            bishop_moves = bishop_moves + countOnes(PsLegalMoveMaskGen::generateBishopMask(pieces, index, Pieces::White, false));
        }

        while (iteration_masks[Pieces::White][Pieces::Rook]) 
        {
            index = bsf(iteration_masks[Pieces::White][Pieces::Rook]);
            setZero(iteration_masks[Pieces::White][Pieces::Rook], index);
            rook_moves = rook_moves + countOnes(PsLegalMoveMaskGen::generateRookMask(pieces, index, Pieces::White, false));
        }

        while (iteration_masks[Pieces::White][Pieces::Queen]) 
        {
            index = bsf(iteration_masks[Pieces::White][Pieces::Queen]);
            setZero(iteration_masks[Pieces::White][Pieces::Queen], index);
            queen_moves = queen_moves + countOnes(PsLegalMoveMaskGen::generateQueenMask(pieces, index, Pieces::White, false));
        }

        while (iteration_masks[Pieces::Black][Pieces::Knight]) 
        {
            index = bsf(iteration_masks[Pieces::Black][Pieces::Knight]);
            setZero(iteration_masks[Pieces::Black][Pieces::Knight], index);
            knight_moves = knight_moves - countOnes(PsLegalMoveMaskGen::generateKnightMask(pieces, index, Pieces::Black, false));
        }

        while (iteration_masks[Pieces::Black][Pieces::Bishop]) 
        {
            index = bsf(iteration_masks[Pieces::Black][Pieces::Bishop]);
            setZero(iteration_masks[Pieces::Black][Pieces::Bishop], index);
            bishop_moves = bishop_moves - countOnes(PsLegalMoveMaskGen::generateBishopMask(pieces, index, Pieces::Black, false));
        }

        while (iteration_masks[Pieces::Black][Pieces::Rook]) 
        {
            index = bsf(iteration_masks[Pieces::Black][Pieces::Rook]);
            setZero(iteration_masks[Pieces::Black][Pieces::Rook], index);
            rook_moves = rook_moves - countOnes(PsLegalMoveMaskGen::generateRookMask(pieces, index, Pieces::Black, false));
        }

        while (iteration_masks[Pieces::Black][Pieces::Queen])
        {
            index = bsf(iteration_masks[Pieces::Black][Pieces::Queen]);
            setZero(iteration_masks[Pieces::Black][Pieces::Queen], index);
            queen_moves = queen_moves - countOnes(PsLegalMoveMaskGen::generateQueenMask(pieces, index, Pieces::Black, false));
        }

        mobility = mobility + StaticEvaluator::Mobility::Knight * knight_moves;
        mobility = mobility + StaticEvaluator::Mobility::Bishop * bishop_moves;
        mobility = mobility + StaticEvaluator::Mobility::Rook * rook_moves;
        mobility = mobility + StaticEvaluator::Mobility::Queen * queen_moves;

        return mobility;
    }
    static int32_t pawnStructureDoublePawn(Pieces pieces)
    {
        int32_t double_pawn = 0;

        int32_t double_pawn_ctr = 0;

        uint8_t white_pawns;
        uint8_t black_pawns;

        for (uint8_t x = 0; x < 8; x = x + 1) 
        {
            white_pawns = countOnes(pieces.pieceBitboards[Pieces::White][Pieces::Pawn] & BitboardColumns::Columns[x]);
            black_pawns = countOnes(pieces.pieceBitboards[Pieces::Black][Pieces::Pawn] & BitboardColumns::Columns[x]);

            double_pawn_ctr = double_pawn_ctr + std::max(0, white_pawns - 1);
            double_pawn_ctr = double_pawn_ctr - std::max(0, black_pawns - 1);
        }

        double_pawn = double_pawn + StaticEvaluator::PawnStructure::DoublePawn * double_pawn_ctr;

        return double_pawn;
    }
    static int32_t pawnStructureConnectedPawn(Pieces pieces)
    {
        int32_t connected_pawn = 0;
    
        int32_t connected_pawn_ctr = 0;
    
        Bitboard white_captures = PsLegalMoveMaskGen::generatePawnLeftCapturesMask(pieces, Pieces::White, true) | PsLegalMoveMaskGen::generatePawnRightCapturesMask(pieces, Pieces::White, true);
        Bitboard black_captures = PsLegalMoveMaskGen::generatePawnLeftCapturesMask(pieces, Pieces::Black, true) | PsLegalMoveMaskGen::generatePawnRightCapturesMask(pieces, Pieces::Black, true);
    
        connected_pawn_ctr = connected_pawn_ctr + countOnes(white_captures & pieces.pieceBitboards[Pieces::White][Pieces::Pawn]);
        connected_pawn_ctr = connected_pawn_ctr - countOnes(black_captures & pieces.pieceBitboards[Pieces::Black][Pieces::Pawn]);
    
        connected_pawn = connected_pawn + StaticEvaluator::PawnStructure::ConnectedPawn * connected_pawn_ctr;
    
        return connected_pawn;
    }
    static int32_t pawnStructurePawnPromotion(Pieces pieces)
    {
        int32_t pawn_promotion = 0;

        Bitboard white_pawns = pieces.pieceBitboards[Pieces::White][Pieces::Pawn];
        Bitboard black_pawns = pieces.pieceBitboards[Pieces::Black][Pieces::Pawn];

        uint8_t index;

        while (white_pawns) 
        {
            index = bsf(white_pawns);
            setZero(white_pawns, index);

            if (PassedPawnMasks::WhitePassedPawnMasks[index] & pieces.pieceBitboards[Pieces::Black][Pieces::Pawn]) pawn_promotion = pawn_promotion + StaticEvaluator::PawnStructure::DefaultPawnPromotion[index / 8];
            else pawn_promotion = pawn_promotion + StaticEvaluator::PawnStructure::PassedPawnPromotion[index / 8];
        }

        while (black_pawns) 
        {
            index = bsf(black_pawns);
            setZero(black_pawns, index);

            if (PassedPawnMasks::BlackPassedPawnMasks[index] & pieces.pieceBitboards[Pieces::White][Pieces::Pawn]) pawn_promotion = pawn_promotion - StaticEvaluator::PawnStructure::DefaultPawnPromotion[7 - index / 8];
            else pawn_promotion = pawn_promotion - StaticEvaluator::PawnStructure::PassedPawnPromotion[7 - index / 8];
        }

        return pawn_promotion;
    }
    static int32_t kingSafetyCrashedCastling(bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling, bool whiteCastlingHappened, bool blackCastlingHappened)
    {
        int32_t crashed_castling = 0;

        if (!whiteCastlingHappened) 
        {
            if (!whiteLongCastling) crashed_castling = crashed_castling + StaticEvaluator::KingSafety::CrashedCastling;
            if (!whiteShortCastling) crashed_castling = crashed_castling + StaticEvaluator::KingSafety::CrashedCastling;
        }

        if (!blackCastlingHappened) 
        {
            if (!blackLongCastling) crashed_castling = crashed_castling - StaticEvaluator::KingSafety::CrashedCastling;
            if (!blackShortCastling) crashed_castling = crashed_castling - StaticEvaluator::KingSafety::CrashedCastling;
        }

        return crashed_castling;
    }
    static int32_t kingSafetyPawnShield(Pieces pieces, bool whiteCastlingHappened, bool blackCastlingHappened)
    {
        int32_t pawn_shield = 0;

        int32_t pawn_shield_ctr = 0;

        if (whiteCastlingHappened) 
        {
            Bitboard white_pawns = pieces.pieceBitboards[Pieces::White][Pieces::Pawn];
            Bitboard white_pawn_shield = PawnShieldMasks::WhitePawnShieldMasks[bsf(pieces.pieceBitboards[Pieces::White][Pieces::King])];
            pawn_shield_ctr = pawn_shield_ctr + countOnes(white_pawns & white_pawn_shield);
        }

        if (blackCastlingHappened) 
        {
            Bitboard black_pawns = pieces.pieceBitboards[Pieces::Black][Pieces::Pawn];
            Bitboard black_pawn_shield = PawnShieldMasks::BlackPawnShieldMasks[bsf(pieces.pieceBitboards[Pieces::Black][Pieces::King])];
            pawn_shield_ctr = pawn_shield_ctr - countOnes(black_pawns & black_pawn_shield);
        }

        pawn_shield = pawn_shield + StaticEvaluator::KingSafety::PawnShield * pawn_shield_ctr;

        return pawn_shield;
    }

    static int32_t twoBishops(Pieces pieces)
    {
        int32_t two_bishops = 0;
    
        if (countOnes(pieces.pieceBitboards[Pieces::White][Pieces::Bishop]) >= 2) two_bishops = two_bishops + StaticEvaluator::TwoBishops;
        if (countOnes(pieces.pieceBitboards[Pieces::Black][Pieces::Bishop]) >= 2) two_bishops = two_bishops - StaticEvaluator::TwoBishops;
    
        return two_bishops;
    }
    static int32_t endgame(Pieces pieces, bool whiteLeading)
    {
        int32_t endgame = 0;

        if (countOnes(pieces.all) > StaticEvaluator::Endgame::MaximumPiecesForEndgame) return endgame;

        uint8_t attacker_side;
        uint8_t defender_side;

        if (whiteLeading) {
            attacker_side = Pieces::White;
            defender_side = Pieces::Black;
        }
        else {
            attacker_side = Pieces::Black;
            defender_side = Pieces::White;
        }

        uint8_t attacker_king_p = bsf(pieces.pieceBitboards[attacker_side][Pieces::King]);
        uint8_t attacker_king_x = attacker_king_p % 8;
        uint8_t attacker_king_y = attacker_king_p / 8;

        uint8_t defender_king_p = bsf(pieces.pieceBitboards[defender_side][Pieces::King]);
        uint8_t defender_king_x = defender_king_p % 8;
        uint8_t defender_king_y = defender_king_p / 8;

        endgame = endgame + StaticEvaluator::Endgame::AttackerKingProximityToDefenderKing * (16 - std::abs(attacker_king_x - defender_king_x) - std::abs(attacker_king_y - defender_king_y));
        endgame = endgame + StaticEvaluator::Endgame::DistanceBetweenDefenderKingAndMiddle * (std::abs(defender_king_x - 3) + std::abs(defender_king_y - 4));

        if (!whiteLeading) endgame = -endgame;

        return endgame;
    }

    struct Material 
    {
        static constexpr int32_t Pawn = 100;
        static constexpr int32_t Knight = 305;
        static constexpr int32_t Bishop = 333;
        static constexpr int32_t Rook = 563;
        static constexpr int32_t Queen = 950;
    };

    struct Mobility 
    {
        static constexpr int32_t Knight = 9;
        static constexpr int32_t Bishop = 4;
        static constexpr int32_t Rook = 3;
        static constexpr int32_t Queen = 3;
    };

    struct PawnStructure 
    {
        static constexpr int32_t DoublePawn = -25;
        static constexpr int32_t ConnectedPawn = 12;
        static constexpr std::array<int32_t, 8> DefaultPawnPromotion = {0, 0, 0, 0, 10, 20, 30, 0};
        static constexpr std::array<int32_t, 8> PassedPawnPromotion = {0, 50, 50, 50, 70, 90, 110, 0};
    };

    struct KingSafety 
    {
        static constexpr int32_t CrashedCastling = -50;
        static constexpr int32_t PawnShield = 33;
    };

    static constexpr int32_t TwoBishops = 50;

    struct Endgame 
    {
        static constexpr int32_t MaximumPiecesForEndgame = 8;
        static constexpr int32_t AttackerKingProximityToDefenderKing = 10;
        static constexpr int32_t DistanceBetweenDefenderKingAndMiddle = 10;
    };

    friend class MoveSorter;
};

class MoveSorter 
{
public:
    static MoveList sort(Pieces pieces, MoveList moves)
    {
        for (uint8_t i = 0; i < moves.size() - 1; i = i + 1) 
        {
            for (uint8_t j = 0; j < moves.size() - i - 1; j = j + 1) 
            {
                if (MoveSorter::evaluateMove(pieces, moves[j]) < MoveSorter::evaluateMove(pieces, moves[j + 1])) std::swap(moves[j], moves[j + 1]);
            }
        }

        return moves;
    }
private:
    static int32_t evaluateMove(Pieces pieces, Move move)
    {
        int32_t evaluation = 0;
    
        if (move.AttackerType != Pieces::Pawn) 
        {
            Bitboard opponent_pawn_attacks = PsLegalMoveMaskGen::generatePawnLeftCapturesMask(pieces, Pieces::inverse(move.AttackerSide), true) | PsLegalMoveMaskGen::generatePawnRightCapturesMask(pieces, Pieces::inverse(move.AttackerSide), true);
            
            if (getBit(opponent_pawn_attacks, move.To)) 
            {
                switch (move.AttackerType) 
                {
                    case Pieces::Knight: evaluation = evaluation - StaticEvaluator::Material::Knight; break;
                    case Pieces::Bishop: evaluation = evaluation - StaticEvaluator::Material::Bishop; break;
                    case Pieces::Rook: evaluation = evaluation - StaticEvaluator::Material::Rook; break;
                    case Pieces::Queen: evaluation = evaluation - StaticEvaluator::Material::Queen; break;
                }
            }
        }
    
        if (move.DefenderType != 255) 
        {
            switch (move.DefenderType) 
            {
                case Pieces::Pawn: evaluation = evaluation + 1000 * StaticEvaluator::Material::Pawn; break;
                case Pieces::Knight: evaluation = evaluation + 1000 * StaticEvaluator::Material::Knight; break;
                case Pieces::Bishop: evaluation = evaluation + 1000 * StaticEvaluator::Material::Bishop; break;
                case Pieces::Rook: evaluation = evaluation + 1000 * StaticEvaluator::Material::Rook; break;
                case Pieces::Queen: evaluation = evaluation + 1000 * StaticEvaluator::Material::Queen; break;
            }
            
            switch (move.DefenderType)
            {
                case Pieces::Pawn: evaluation = evaluation - StaticEvaluator::Material::Pawn; break;
                case Pieces::Knight: evaluation = evaluation - StaticEvaluator::Material::Knight; break;
                case Pieces::Bishop: evaluation = evaluation - StaticEvaluator::Material::Bishop; break;
                case Pieces::Rook: evaluation = evaluation - StaticEvaluator::Material::Rook; break;
                case Pieces::Queen: evaluation = evaluation - StaticEvaluator::Material::Queen; break;
            }
        }
    
        return evaluation;
    }
};

class Entry 
{
public:
    Entry() = default;
    Entry(ZobristHash hash, int32_t depth, uint8_t bestMoveIndex)
    {
        this->Hash = hash;
        this->Depth = depth;
        this->BestMoveIndex = bestMoveIndex;
    }

    friend bool operator <(Entry left, Entry right)
    {
        return (left.Hash < right.Hash);
    }

    ZobristHash Hash;
    int32_t Depth;
    uint8_t BestMoveIndex;
};

class TranspositionTable 
{
public:
    TranspositionTable() = default;

    void addEntry(Entry entry)
    {
        auto hash_copy = this->Set.find(entry);
        if (hash_copy == this->Set.end() or hash_copy->Depth < entry.Depth) this->Set.insert(entry);
    }

    uint8_t tryToFindBestMoveIndex(ZobristHash hash)
    {
        auto entry = this->Set.find({hash, 0, 0});

        if (entry == this->Set.end()) return 255;
        return entry->BestMoveIndex;
    }

private:
    set<Entry> Set;
};

class AI 
{
public:
    AI() = default;
    AI(const string& openingBookPath)
    {
        this->openingBook = {openingBookPath};
    }  

    Move bestMove(const Position& position, uint8_t side, int32_t minMs, int32_t maxMs)
    {
        cout << endl;

        StaticEvaluator::evaluate(position.pieces, position.WhiteLongCastling, position.WhiteShortCastling, position.BlackLongCastling, position.BlackShortCastling, position.whiteCastlingHappened, position.blackCastlingHappened, true);

        int64_t timeStart = nsecs;
        stopSearch = false;
        TranspositionTable tt;

        tuple<Move, int32_t> openingBookResult = this->openingBook.tryToFindMove(position);
        cout << "Number of available moves in the opening book: " << get<1>(openingBookResult) << "." << std::endl;

        if (get<1>(openingBookResult)) 
        {
            usleep(max((int64_t)0, (minMs - (nsecs - timeStart) / (int64_t)1e+6) * (int64_t)1e+3));

            return get<0>(openingBookResult);
        }

        cout << "Search started." << endl;

        int32_t bestMoveEvaluation;
        Move bestMove;

        future<tuple<int32_t, Move>> bestMoveThread;

        bool updateBestMove;

        for (int32_t i = 1; i < 1e+3; i = i + 1) 
        {
            evaluated = 0;
            maxDepth = 0;
            cutOffs = 0;

            bestMoveThread = async(AI::BestMove, position, side, i, ref(tt));

            updateBestMove = true;

            while (bestMoveThread.wait_for(chrono::seconds(0)) != future_status::ready) 
            {
                if ((nsecs - timeStart) / (int32_t)1e+6 >= maxMs) 
                {
                    updateBestMove = false;
                    break;
                }

                usleep(20000);
            }

            if (updateBestMove or i == 1) tie(bestMoveEvaluation, bestMove) = bestMoveThread.get();
            else {
                stopSearch = true;
                break;
            }

            cout << "Base depth: " << setw(4) << i << ". Maximal depth: " << setw(4) << maxDepth << ". Evaluation: " << setw(6) << (float)bestMoveEvaluation / 100.0f << " pawns. Evaluated (this iteration): " << std::setw(10) << evaluated << ". TT cutoffs (this iteration): " << std::setw(10) << cutOffs << ". Time (full search): " << std::setw(10) << (nsecs - timeStart) / (int32_t)1e+6 << " ms." << std::endl;

            if (bestMoveEvaluation > AI::Infinity::Positive - 2000 or bestMoveEvaluation < AI::Infinity::Negative + 2000) break;
        }

        usleep(max((int64_t)0, (minMs - (nsecs - timeStart) / (int64_t)1e+6) * (int64_t)1e+3));

        cout << "Search finished." << endl;

        return bestMove;
    }

private:
    OpeningBook openingBook;

    static tuple<int32_t, Move> BestMove(const Position& position, uint8_t side, int32_t depth, TranspositionTable &tt)
    {
        if (side == Pieces::White) return AI::alphaBetaMax(position, AI::Infinity::Negative, AI::Infinity::Positive, depth, 0, tt);

        return AI::alphaBetaMin(position, AI::Infinity::Negative, AI::Infinity::Positive, depth, 0, tt);
    }

    static tuple<int32_t, Move> alphaBetaMin(Position position, int32_t alpha, int32_t beta, int32_t depth_left, int32_t currentDepth, TranspositionTable &tt)
    {
        if (stopSearch) return make_tuple(0, Move());
        if (currentDepth > maxDepth) maxDepth = currentDepth;

        if (depth_left == 0) return make_tuple(AI::alphaBetaMinOnlyCaptures(position, alpha, beta, currentDepth), Move());

        if (position.fiftyMovesCtr >= 50 or position.repetitionHistory.getRepetionNumber(position.hash) >= 3) return make_tuple(0, Move());

        MoveList moves = LegalMoveGen::generate(position, Pieces::Black);
        moves = MoveSorter::sort(position.pieces, moves);

        Move move;
        Move bestMove;
        uint8_t bestMoveIndex;

        bool in_check = PsLegalMoveMaskGen::inDanger(position.pieces, bsf(position.pieces.pieceBitboards[Pieces::Black][Pieces::King]), Pieces::Black);

        if (moves.size() == 0) 
        {
            if (in_check) return make_tuple(AI::Infinity::Positive - currentDepth, Move());
            return std::make_tuple(0, Move());
        }

        int32_t evaluation;

        Position copy;

        uint8_t tt_result = tt.tryToFindBestMoveIndex(position.hash);

        for (uint8_t i = 0; i < moves.size(); i = i + 1) 
        {
            if (tt_result >= moves.size()) move = moves[i];
            else 
            {
                if (i == 0) move = moves[tt_result];
                else 
                {
                    if (i == tt_result) move = moves[0];
                    else move = moves[i];
                }
            }

            copy = position;
            copy.move(move);
            evaluation = get<0>(AI::alphaBetaMax(copy, alpha, beta, depth_left - !in_check, currentDepth + 1, tt));

            if (evaluation <= alpha) 
            {
                if (tt_result >= moves.size() or i != 0) tt.addEntry({position.hash, depth_left, bestMoveIndex});
                else cutOffs = cutOffs + 1;
                return make_tuple(alpha, bestMove);
            }

            if (evaluation < beta) 
            {
                bestMove = move;
                bestMoveIndex = i;
                beta = evaluation;
            }
        }

        tt.addEntry({position.hash, depth_left, bestMoveIndex});
        return std::make_tuple(beta, bestMove);
    }
    static tuple<int32_t, Move> alphaBetaMax(Position position, int32_t alpha, int32_t beta, int32_t depth_left, int32_t currentDepth, TranspositionTable &tt)
    {
        if (stopSearch) return std::make_tuple(0, Move());
        if (currentDepth > maxDepth) maxDepth = currentDepth;

        if (depth_left == 0) return std::make_tuple(AI::alphaBetaMaxOnlyCaptures(position, alpha, beta, currentDepth), Move());

        if (position.fiftyMovesCtr >= 50 or position.repetitionHistory.getRepetionNumber(position.hash) >= 3) return std::make_tuple(0, Move());

        MoveList moves = LegalMoveGen::generate(position, Pieces::White);
        moves = MoveSorter::sort(position.pieces, moves);
        Move move;
        Move bestMove;
        uint8_t bestMoveIndex;

        bool in_check = PsLegalMoveMaskGen::inDanger(position.pieces, bsf(position.pieces.pieceBitboards[Pieces::White][Pieces::King]), Pieces::White);

        if (moves.size() == 0) 
        {
            if (in_check) return std::make_tuple(AI::Infinity::Negative + currentDepth, Move());
            return std::make_tuple(0, Move());
        }

        int32_t evaluation;

        Position copy;

        uint8_t tt_result = tt.tryToFindBestMoveIndex(position.hash);

        for (uint8_t i = 0; i < moves.size(); i = i + 1) 
        {
            if (tt_result >= moves.size()) move = moves[i];
            else 
            {
                if (i == 0) move = moves[tt_result];
                else 
                {
                    if (i == tt_result) move = moves[0];
                    else move = moves[i];
                }
            }

            copy = position;
            copy.move(move);
            evaluation = get<0>(AI::alphaBetaMin(copy, alpha, beta, depth_left - !in_check, currentDepth + 1, tt));

            if (evaluation >= beta) 
            {
                if (tt_result >= moves.size() or i != 0) tt.addEntry({position.hash, depth_left, bestMoveIndex});
                else cutOffs = cutOffs + 1;
                return make_tuple(beta, bestMove);
            }

            if (evaluation > alpha) 
            {
                bestMove = move;
                bestMoveIndex = i;
                alpha = evaluation;
            }
        }

        tt.addEntry({position.hash, depth_left, bestMoveIndex});
        return make_tuple(alpha, bestMove);
    }

    static int32_t alphaBetaMinOnlyCaptures(const Position& position, int32_t alpha, int32_t beta, int32_t currentDepth)
    {
        if (stopSearch) return 0;
        if (currentDepth > maxDepth) maxDepth = currentDepth;

        int32_t evaluation = StaticEvaluator::evaluate(position.pieces, position.WhiteLongCastling, position.WhiteShortCastling, position.BlackLongCastling, position.BlackShortCastling, position.whiteCastlingHappened, position.blackCastlingHappened);
        evaluated = evaluated + 1;

        if (evaluation <= alpha) return alpha;
        if (evaluation < beta) beta = evaluation;

        MoveList moves = LegalMoveGen::generate(position, Pieces::Black, true);
        moves = MoveSorter::sort(position.pieces, moves);
        Move move;

        Position copy;

        for (uint8_t i = 0; i < moves.size(); i = i + 1) 
        {
            move = moves[i];

            copy = position;
            copy.move(move);
            evaluation = AI::alphaBetaMaxOnlyCaptures(copy, alpha, beta, currentDepth + 1);

            if (evaluation <= alpha) return alpha;
            if (evaluation < beta) beta = evaluation;
        }

        return beta;
    }

    static int32_t alphaBetaMaxOnlyCaptures(const Position& position, int32_t alpha, int32_t beta, int32_t currentDepth)
    {
        if (stopSearch) return 0;
        if (currentDepth > maxDepth) maxDepth = currentDepth;

        int32_t evaluation = StaticEvaluator::evaluate(position.pieces, position.WhiteLongCastling, position.WhiteShortCastling, position.BlackLongCastling, position.BlackShortCastling, position.whiteCastlingHappened, position.blackCastlingHappened);
        evaluated = evaluated + 1;

        if (evaluation >= beta) return beta;
        if (evaluation > alpha) alpha = evaluation;

        MoveList moves = LegalMoveGen::generate(position, Pieces::White, true);
        moves = MoveSorter::sort(position.pieces, moves);
        Move move;

        Position copy;

        for (uint8_t i = 0; i < moves.size(); i = i + 1) 
        {
            move = moves[i];

            copy = position;
            copy.move(move);
            evaluation = AI::alphaBetaMinOnlyCaptures(copy, alpha, beta, currentDepth + 1);

            if (evaluation >= beta) return beta;
            if (evaluation > alpha) alpha = evaluation;
        }

        return alpha;
    }

    struct Infinity 
    {
        static constexpr int32_t Negative = -1e+9;
        static constexpr int32_t Positive = 1e+9;
    };
};


class Game
{
public:
    void start()
    {
        this->position = {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 255, true, true, true, true, 1};

        cout << "Welcome to the chess engine!" << endl;

        sideChoose();

        if (playerSide == Pieces::White) 
        {
            while (true) 
            {

                cout << position << endl;


                moves = LegalMoveGen::generate(this->position, playerSide);

                this->playerMove = getMove();

                bool moveFound = false;

                for (uint8_t i = 0; i < moves.size(); i++)
                {
                    if (moves[i].From == playerMove.From && moves[i].To == playerMove.To && (moves[i].Flag != Move::Flag::PromoteToKnight && moves[i].Flag != Move::Flag::PromoteToBishop && moves[i].Flag != Move::Flag::PromoteToRook && moves[i].Flag != Move::Flag::PromoteToQueen))
                    {
                        move = moves[i];
                        moveFound = true;

                        break;
                    } else
                    if (moves[i].From == playerMove.From && moves[i].To == playerMove.To)
                    {
                        //Choose promotion piece
                        cout << "\nChoose promotion piece: " << endl;
                        cout << "1. Knight" << endl;
                        cout << "2. Bishop" << endl;
                        cout << "3. Rook" << endl;
                        cout << "4. Queen" << endl;

                        int promotionPiece = 0;
                        cin >> promotionPiece;

                        switch (promotionPiece)
                        {
                            case 1:
                                move = moves[i];
                                move.Flag = Move::Flag::PromoteToKnight;
                                moveFound = true;
                            break;

                            case 2:
                                move = moves[i];
                                move.Flag = Move::Flag::PromoteToBishop;
                                moveFound = true;
                            break;

                            case 3:
                                move = moves[i];
                                move.Flag = Move::Flag::PromoteToRook;
                                moveFound = true;
                            break;

                            case 4:
                                move = moves[i];
                                move.Flag = Move::Flag::PromoteToQueen;
                                moveFound = true;
                            break;

                            default:
                                cout << RED << "\n[ERROR] Illegal move!" << END << endl;
                                continue;
                            break;
                        }

                        break;
                    }

                }

                if (!moveFound)
                {
                    cout << RED << "\n[ERROR] Illegal move!" << END << endl;
                    continue;
                }

                this->position.move(move);

                if (move.DefenderType != 255)
                {
                    cout << "\n";

                    switch (move.DefenderType)
                    {
                        case 0:
                            cout << YELLOW << "Pawn has been captured!" << END << endl;
                        break;

                        case 1:
                            cout << YELLOW << "Knight has been captured!" << END << endl;
                        break;

                        case 2:
                            cout << YELLOW << "Bishop has been captured!" << END << endl;
                        break;

                        case 3:
                            cout << YELLOW << "Rook has been captured!" << END << endl;
                        break;

                        case 4:
                            cout << YELLOW << "Queen has been captured!" << END << endl;
                        break;

                        default:
                            cout << RED << "[ERROR] Unknown piece type!" << END << endl;
                        break;
                    }
                }


                cout << "\nMove has been made!" << endl;

                cout << position << endl;

                //Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;
                    break;
                }

                //AI move
                move = ai.bestMove(position, aiSide, 0, 10000);
                position.move(move);

                //Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;
                    break;
                }
            }
        } else 
        {
            cout << position << endl;

            while (true) 
            {

                //AI move
                move = ai.bestMove(position, aiSide, 0, 10000);
                position.move(move);

                cout << position << endl;
                
                //Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;
                    break;
                }

                //Player move
                bool moveFound = false;

                do
                {
                    moves = LegalMoveGen::generate(this->position, playerSide);
    
                    this->playerMove = getMove();
    
                    for (uint8_t i = 0; i < moves.size(); i++)
                    {
                        if (moves[i].From == playerMove.From && moves[i].To == playerMove.To && (moves[i].Flag != Move::Flag::PromoteToKnight && moves[i].Flag != Move::Flag::PromoteToBishop && moves[i].Flag != Move::Flag::PromoteToRook && moves[i].Flag != Move::Flag::PromoteToQueen))
                        {
                            move = moves[i];
                            moveFound = true;

                            break;
                        } else
                        if (moves[i].From == playerMove.From && moves[i].To == playerMove.To)
                        {
                            //Choose promotion piece
                            cout << "\nChoose promotion piece: " << endl;
                            cout << "1. Knight" << endl;
                            cout << "2. Bishop" << endl;
                            cout << "3. Rook" << endl;
                            cout << "4. Queen" << endl;

                            int promotionPiece = 0;
                            cin >> promotionPiece;

                            switch (promotionPiece)
                            {
                                case 1:
                                    move = moves[i];
                                    move.Flag = Move::Flag::PromoteToKnight;
                                    moveFound = true;
                                break;

                                case 2:
                                    move = moves[i];
                                    move.Flag = Move::Flag::PromoteToBishop;
                                    moveFound = true;
                                break;

                                case 3:
                                    move = moves[i];
                                    move.Flag = Move::Flag::PromoteToRook;
                                    moveFound = true;
                                break;

                                case 4:
                                    move = moves[i];
                                    move.Flag = Move::Flag::PromoteToQueen;
                                    moveFound = true;
                                break;

                                default:
                                    cout << RED << "\n[ERROR] Illegal move!" << END << endl;
                                    continue;
                                break;
                            }

                            break;
                        }

                    }

                    if (!moveFound)
                    {
                        cout << RED << "\n[ERROR] Illegal move!" << END << endl;
                        continue;
                    }

                    this->position.move(move);

                    if (move.DefenderType != 255)
                    {
                        cout << "\n";

                        switch (move.DefenderType)
                        {
                            case 0:
                                cout << YELLOW << "Pawn has been captured!" << END << endl;
                            break;

                            case 1:
                                cout << YELLOW << "Knight has been captured!" << END << endl;
                            break;

                            case 2:
                                cout << YELLOW << "Bishop has been captured!" << END << endl;
                            break;

                            case 3:
                                cout << YELLOW << "Rook has been captured!" << END << endl;
                            break;

                            case 4:
                                cout << YELLOW << "Queen has been captured!" << END << endl;
                            break;

                            default:
                                cout << RED << "[ERROR] Unknown piece type!" << END << endl;
                            break;
                        }
                    }

                    cout << "\nMove has been made!" << endl;

                    cout << position << endl;
                } while (!moveFound);
                
                
    
                //Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;
                    break;
                }
            }
        }
    }

private:
    AI ai = {OpeningBookFile};
    Move move;
    MoveList moves;
    Position position;
    uint8_t playerSide;
    uint8_t aiSide;
    Move playerMove;

    bool whiteVictory() 
    {
        bool blackHaventGotMoves = (LegalMoveGen::generate(this->position, Pieces::Black).size() == 0);
        bool blackInCheck = PsLegalMoveMaskGen::inDanger(this->position.pieces, bsf(this->position.pieces.pieceBitboards[Pieces::Black][Pieces::King]), Pieces::Black);

        if (playerSide == Pieces::White && blackHaventGotMoves && blackInCheck)
        {
            cout << GREEN << endl;
            cout << "White win!" << END << endl;
        } else
        if (blackHaventGotMoves && blackInCheck)
        {
            cout << RED << endl;
            cout << "White win!" << END << endl;
        }

        return (blackHaventGotMoves && blackInCheck);
    }

    bool blackVictory() 
    {
        bool whiteHaventGotMoves = (LegalMoveGen::generate(this->position, Pieces::White).size() == 0);
        bool whiteInCheck = PsLegalMoveMaskGen::inDanger(this->position.pieces, bsf(this->position.pieces.pieceBitboards[Pieces::White][Pieces::King]), Pieces::White);

        if (playerSide == Pieces::Black && whiteHaventGotMoves && whiteInCheck)
        {
            cout << GREEN << endl;
            cout << "Black win!" << END << endl;
        } else
        if (whiteHaventGotMoves && whiteInCheck)
        {
            cout << RED << endl;
            cout << "Black win!" << END << endl;
        }
        
        return (whiteHaventGotMoves && whiteInCheck);
    }

    bool draw() 
    {
        bool whiteHaventGotMoves = (LegalMoveGen::generate(this->position, Pieces::White).size() == 0);
        bool whiteInCheck = PsLegalMoveMaskGen::inDanger(this->position.pieces, bsf(this->position.pieces.pieceBitboards[Pieces::White][Pieces::King]), Pieces::White);

        bool blackHaventGotMoves = (LegalMoveGen::generate(this->position, Pieces::Black).size() == 0);
        bool blackInCheck = PsLegalMoveMaskGen::inDanger(this->position.pieces, bsf(this->position.pieces.pieceBitboards[Pieces::Black][Pieces::King]), Pieces::Black);

        bool fiftyMovesRule = (this->position.fiftyMovesCtr >= 50);
        bool threeMovesRule = (this->position.repetitionHistory.getRepetionNumber(this->position.hash) >= 3);

        bool whiteMove = this->isWhiteMove();
        bool blackMove = !whiteMove;

        if (whiteHaventGotMoves && !whiteInCheck && whiteMove)
        {
            cout << YELLOW << "[DRAW] White haven't got moves." << END << endl;
            return true;
        }

        if (blackHaventGotMoves && !blackInCheck && blackMove)
        {
            cout << YELLOW << "[DRAW] Black haven't got moves." << END << endl;
            return true;
        }

        if (fiftyMovesRule)
        {
            cout << YELLOW << "[DRAW] Fifty moves rule." << END << endl;
            return true;
        }

        if (threeMovesRule)
        {
            cout << YELLOW << "[DRAW] Three moves rule." << END << endl;
            return true;
        }

        return false;
    }

    bool gameFinished() 
    {
        return (this->whiteVictory() || this->blackVictory() || this->draw());
    }

    bool isWhiteMove() const 
    {
        return (this->position.MoveCtr - floor(this->position.MoveCtr) < 1e-7);
    }

    bool isBlackMove() 
    {
        return !isWhiteMove();
    }

    Move getMove() 
    {
        Move PlayerMove;
        string from, to;

        cout << "\nEnter move (Example: e2 e4): ";
        cin >> from >> to;

        PlayerMove.From = (from[0] - 'a') + (from[1] - '1') * 8;
        PlayerMove.To = (to[0] - 'a') + (to[1] - '1') * 8;
        
        return PlayerMove;
    }

    void sideChoose()
    {
        cout << "Choose your side:" << endl;
        cout << "1. White" << endl;
        cout << "2. Black" << endl;

        int choice;

        do
        {
            cin >> choice;

            if (cin.fail() || choice < 1 || choice > 2)
            {
                cin.clear();
                cin.ignore(1000, '\n');

                cout << RED << "[ERROR] Invalid input. Please try again." << END << endl;
            }
        } while (cin.fail() || choice < 1 || choice > 2);
        
        if (choice == 1)
        {
            cout << "You are playing as white." << endl;
            playerSide = Pieces::White;
            aiSide = Pieces::Black;
        } else
        {
            cout << "You are playing as black." << endl;
            playerSide = Pieces::Black;
            aiSide = Pieces::White;
        }
        
    }
};

int main()
{
    #if TEST_LEGAL_MOVE_GENERATION
        LegalMoveGenTester::test();
    #endif


    #if TEST_AI
        Position position = {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 255, true, true, true, true, 1};
        AI ai;
        ai.bestMove(position, Pieces::White, 0, 60000);
    #endif


    #if PLAY
        Game game;
        game.start();
    #endif

    return 0;
}