#include "position.h"

using namespace std;


Position::Position() = default;
Position::Position(const string &shortFen, uint8_t enPassant, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling, float moveCtr)
{
    //this->time_storage = shortFen;

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

// string Position::getFen()
// {
//    //return time_storage;
// }

ostream &operator<<(ostream &ostream, Position position)
{
    ostream << position.pieces;

    #if DEBUG
            ostream << "En passant: " << (uint32_t)position.EnPassant << "\n";
            ostream << "White long castling: " << position.WhiteLongCastling << "\n";
            ostream << "White short castling: " << position.WhiteShortCastling << "\n";
            ostream << "Black long castling: " << position.BlackLongCastling << "\n";
            ostream << "Black short castling: " << position.BlackShortCastling << "\n";
            ostream << "Move counter: " << position.MoveCtr << "\n";
            ostream << "Zobrist hash: " << hex << "0x" << position.hash.hash << "\n"
                    << dec;
            ostream << "Fifty moves counter: " << position.fiftyMovesCtr << "\n";
            ostream << "Threefold repetition counter: " << (uint32_t)position.repetitionHistory.getRepetionNumber(position.hash) << "\n";
    #endif

    return ostream;
}

void Position::move(Move move)
{
    this->removePiece(move.From, move.AttackerType, move.AttackerSide);
    this->addPiece(move.To, move.AttackerType, move.AttackerSide);//

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
            } else
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
        this->hash.invertPiece(square, type, side);
    }
}

void Position::removePiece(uint8_t square, uint8_t type, uint8_t side)
{
    if (getBit(this->pieces.pieceBitboards[side][type], square))
    {
        setZero(this->pieces.pieceBitboards[side][type], square);
        this->hash.invertPiece(square, type, side);
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

void Position::updateFiftyMovesCtr(bool breakEvent)
{
    if (breakEvent)
    {
        this->fiftyMovesCtr = 0;
    } else
    {
        this->fiftyMovesCtr = this->fiftyMovesCtr + 0.5f;
    }
};