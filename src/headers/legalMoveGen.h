#include "position.h"
#include "moveList.h"
#include "bitboards.h"
#include "psLegalMoveMaskGen.h"

class LegalMoveGen
{
public:
    static MoveList generate(Position position, uint8_t side, bool onlyCaptures = false);

private:
    static void pieceMaskToMoves(Pieces pieces, Bitboard mask, uint8_t attackerPos, uint8_t attackerType, uint8_t attackerSide, MoveList &moves);

    static void pawnMaskToMoves(Pieces pieces, Bitboard mask, uint8_t attackerSide, int8_t attackerIndex, bool lookForDefender, uint8_t flag, MoveList &moves);

    static bool isLegal(Pieces pieces, Move move, bool enPassantCapture);

    static void addEnPassantCaptures(Pieces pieces, uint8_t side, uint8_t enPassant, MoveList &moves);

    static void addCastlingMoves(Pieces pieces, uint8_t side, bool longCastling, bool shortCastling, MoveList &moves);
};
