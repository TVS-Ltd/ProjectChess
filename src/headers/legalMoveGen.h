#include "position.h"
#include "moveList.h"
#include "bitboards.h"
#include "psLegalMoveMaskGen.h"

class LegalMoveGen
{
public:
    static MoveList generate(const Position& position, uint8_t side, bool onlyCaptures = false);

    static void generatePawnMoves(const Position& position, uint8_t side, MoveList& moves, bool onlyCaptures = false);

    static void generateKnightMoves(const Position& position, uint8_t side, MoveList& moves, bool onlyCaptures = false);

    static void generateBishopMoves(const Position& position, uint8_t side, MoveList& moves, bool onlyCaptures = false);

    static void generateRookMoves(const Position& position, uint8_t side, MoveList& moves, bool onlyCaptures = false);

    static void generateQueenMoves(const Position& position, uint8_t side, MoveList& moves, bool onlyCaptures = false);

    static void generateKingMoves(const Position& position, uint8_t side, MoveList& moves, bool onlyCaptures = false);

    static MoveList generateCardMoves(const Position& position, uint8_t side);

    static MoveList generateMovesRoyal(const Position& position, uint8_t side, bool onlyCaptures = false);

private:
    static void generateMovesByCard(const Position& position, Bitboard freePieces, const uint8_t& side, const uint8_t& figureType, MoveList& moves);

    static void pieceMaskToMoves(const Pieces& pieces, Bitboard mask, uint8_t attackerPos, uint8_t attackerType, uint8_t attackerSide, MoveList& moves);

    static void pawnMaskToMoves(const Pieces& pieces, Bitboard mask, uint8_t attackerSide, int8_t attackerIndex, bool lookForDefender, uint8_t flag, MoveList& moves);

    static bool isLegal(Pieces pieces, Move move, bool enPassantCapture);

    static void addEnPassantCaptures(const Pieces& pieces, uint8_t side, uint8_t enPassant, MoveList& moves);

    static void addCastlingMoves(const Pieces& pieces, uint8_t side, bool longCastling, bool shortCastling, MoveList& moves);
};