#include <iostream>
#include <bit>
#include <array>
#include <vector>
#include <cstdint>
#include <cmath>

using namespace std;

typedef uint64_t Bitboard;

struct Pieces {
    Pieces();
    Pieces(const string& shortFen);

    friend ostream& operator <<(ostream& ostream, Pieces pieces);
    friend bool operator ==(Pieces left, Pieces right);

    void updateBitboards();

    static uint8_t inverse(uint8_t side);

    array<array<Bitboard, 6>, 2> pieceBitboards{};
    array<Bitboard, 2> sideBitboards{};
    array<Bitboard, 2> inversionSideBitboards{};
    Bitboard _all;
    Bitboard _empty;

    static constexpr uint8_t Pawn = 0;
    static constexpr uint8_t Knight = 1;
    static constexpr uint8_t Bishop = 2;
    static constexpr uint8_t Rook = 3;
    static constexpr uint8_t Queen = 4;
    static constexpr uint8_t King = 5;

    static constexpr uint8_t White = 0;
    static constexpr uint8_t Black = 1;
};

bool operator ==(Pieces left, Pieces right) 
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

uint8_t Pieces::inverse(uint8_t side) 
{
    return !side;
}

void Pieces::updateBitboards() 
{
    this->sideBitboards[Pieces::White] = this->pieceBitboards[Pieces::White][Pieces::Pawn] |
                                           this->pieceBitboards[Pieces::White][Pieces::Knight] |
                                           this->pieceBitboards[Pieces::White][Pieces::Bishop] |
                                           this->pieceBitboards[Pieces::White][Pieces::Rook] |
                                           this->pieceBitboards[Pieces::White][Pieces::Queen] |
                                           this->pieceBitboards[Pieces::White][Pieces::King];

    this->sideBitboards[Pieces::Black] = this->pieceBitboards[Pieces::Black][Pieces::Pawn] |
                                           this->pieceBitboards[Pieces::Black][Pieces::Knight] |
                                           this->pieceBitboards[Pieces::Black][Pieces::Bishop] |
                                           this->pieceBitboards[Pieces::Black][Pieces::Rook] |
                                           this->pieceBitboards[Pieces::Black][Pieces::Queen] |
                                           this->pieceBitboards[Pieces::Black][Pieces::King];

    this->inversionSideBitboards[Pieces::White] = ~this->sideBitboards[Pieces::White];
    this->inversionSideBitboards[Pieces::Black] = ~this->sideBitboards[Pieces::Black];

    this->_all = this->sideBitboards[Pieces::White] | this->sideBitboards[Pieces::Black];
    this->_empty = ~this->_all;
}


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


struct ZobristHash 
{
    ZobristHash();
    ZobristHash(Pieces pieces, bool blackMove, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling);

    friend bool operator ==(ZobristHash left, ZobristHash right);
    friend bool operator <(ZobristHash left, ZobristHash right);

    void invert_piece(uint8_t square, uint8_t type, uint8_t side);
    void invertMove();
    void invertWhiteLongCastling();
    void invertWhiteShortCastling();
    void invertBlackLongCastling();
    void invertBlackShortCastling();

    uint64_t hash;
};


ZobristHash::ZobristHash(Pieces pieces, bool blackMove, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling) 
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
                this->invert_piece(square, type, side);
                break;
            }
        }
    }
}

bool operator ==(ZobristHash left, ZobristHash right) 
{
    return (left.hash == right.hash);
}

bool operator <(ZobristHash left, ZobristHash right) 
{
    return (left.hash < right.hash);
}

void ZobristHash::invert_piece(uint8_t square, uint8_t type, uint8_t side) 
{
    this->hash = this->hash ^ ZobristHashConsteval::Constants[square][side][type];
}

void ZobristHash::invertMove() 
{
    this->hash = this->hash ^ ZobristHashConsteval::BlackMove;
}

void ZobristHash::invertWhiteLongCastling() 
{
    this->hash = this->hash ^ ZobristHashConsteval::WhiteLongCastling;
}

void ZobristHash::invertWhiteShortCastling() 
{
    this->hash = this->hash ^ ZobristHashConsteval::WhiteShortCastling;
}

void ZobristHash::invertBlackLongCastling() 
{
    this->hash = this->hash ^ ZobristHashConsteval::BlackLongCastling;
}

void ZobristHash::invertBlackShortCastling() 
{
    this->hash = this->hash ^ ZobristHashConsteval::BlackShortCastling;
}


class RepetitionHistory 
{
public:
    RepetitionHistory();

    void addPosition(ZobristHash hash);

    void clear();

    uint8_t getRepetionNumber(ZobristHash hash);
private:
    vector<ZobristHash> hashes;
};

void RepetitionHistory::addPosition(ZobristHash hash) 
{
    this->hashes.push_back(hash);
}

void RepetitionHistory::clear() 
{
    this->hashes.clear();
}

uint8_t RepetitionHistory::getRepetionNumber(ZobristHash hash) 
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

struct Move 
{
    Move();
    Move(uint8_t from, uint8_t to, uint8_t attackerType, uint8_t attackerSide, uint8_t defenderType, uint8_t defenderSide, uint8_t flag = 0);

    friend bool operator ==(Move left, Move right);

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

bool operator ==(Move left, Move right) 
{
    return (left.From == right.From and left.To == right.To and left.AttackerType == right.AttackerType and left.AttackerSide == right.AttackerSide and left.DefenderType == right.DefenderType and left.DefenderSide == right.DefenderSide and left.Flag == right.Flag);
}

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

static constexpr uint8_t bsr(Bitboard bitBoard) 
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

ostream &operator<<(std::ostream &ostream, Pieces pieces) 
{
    for (int8_t y = 7; y >= 0; y = y - 1) 
    {
        for (uint8_t x = 0; x < 8; x = x + 1) 
        {
            ostream << "|  ";

            if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Pawn], y * 8 + x)) ostream << "♙";
            else if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Knight], y * 8 + x)) ostream << "♘";
            else if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Bishop], y * 8 + x)) ostream << "♗";
            else if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Rook], y * 8 + x)) ostream << "♖";
            else if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::Queen], y * 8 + x)) ostream << "♕";
            else if (getBit(pieces.pieceBitboards[Pieces::White][Pieces::King], y * 8 + x)) ostream << "♔";

            else if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Pawn], y * 8 + x)) ostream << "♟";
            else if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Knight], y * 8 + x)) ostream << "♞";
            else if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Bishop], y * 8 + x)) ostream << "♝";
            else if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Rook], y * 8 + x)) ostream << "♜";
            else if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::Queen], y * 8 + x)) ostream << "♛";
            else if (getBit(pieces.pieceBitboards[Pieces::Black][Pieces::King], y * 8 + x)) ostream << "♚";

            else ostream << " ";

            ostream << "  ";
        }

        ostream << "|\n";
    }

    return ostream;
}

class Position 
{
public:
    Position();
    Position(const string& shortFen, uint8_t enPassant, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling, float moveCtr);

    friend ostream& operator <<(ostream& ostream, Position position);

    void move(Move move);

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
private:
    void addPiece(uint8_t square, uint8_t type, uint8_t side);
    void removePiece(uint8_t square, uint8_t type, uint8_t side);
    void changeEnPassant(uint8_t en_passant);

    void removeWhiteLongCastling();
    void removeWhiteShortCastling();
    void removeBlackLongCastling();
    void removeBlackShortCastling();

    void updateMoveCtr();
    void updateFiftyMovesCtr(bool breakEvent);
};

Position::Position(const string& shortFen, uint8_t enPassant, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling, float moveCtr) 
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

ostream &operator<<(ostream &ostream, Position position) 
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

void Position::move(Move move) 
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

void Position::addPiece(uint8_t square, uint8_t type, uint8_t side) 
{
    if (!getBit(this->pieces.pieceBitboards[side][type], square)) 
    {
        setOne(this->pieces.pieceBitboards[side][type], square);
        this->hash.invert_piece(square, type, side);
    }
}

void Position::removePiece(uint8_t square, uint8_t type, uint8_t side) 
{
    if (getBit(this->pieces.pieceBitboards[side][type], square)) 
    {
        setZero(this->pieces.pieceBitboards[side][type], square);
        this->hash.invert_piece(square, type, side);
    }
}

void Position::changeEnPassant(uint8_t enPassant) 
{
    this->EnPassant = enPassant;
}

void Position::removeWhiteLongCastling() 
{
    if (this->WhiteLongCastling) 
    {
        this->WhiteLongCastling = false;
        this->hash.invertWhiteLongCastling();
    }
}

void Position::removeWhiteShortCastling() 
{
    if (this->WhiteShortCastling) 
    {
        this->WhiteShortCastling = false;
        this->hash.invertWhiteShortCastling();
    }
}

void Position::removeBlackLongCastling() 
{
    if (this->BlackLongCastling) 
    {
        this->BlackLongCastling = false;
        this->hash.invertBlackLongCastling();
    }
}

void Position::removeBlackShortCastling() 
{
    if (this->BlackShortCastling) 
    {
        this->BlackShortCastling = false;
        this->hash.invertBlackShortCastling();
    }
}

void Position::updateMoveCtr() 
{
    this->MoveCtr = this->MoveCtr + 0.5f;
    this->hash.invertMove();
}

void Position::updateFiftyMovesCtr(bool break_event) 
{
    if (break_event)
    {
        this->fiftyMovesCtr = 0;
    }
    else
    {
        this->fiftyMovesCtr = this->fiftyMovesCtr + 0.5f;
    }
}

int main()
{


    return 0;
}