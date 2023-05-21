#include <bit>
#include <array>
#include <cstdint>

#pragma once

using namespace std;

// ——————————————————  Bitboards  ——————————————————

/**
 * Type for storing board in the form of bitboards.
*/
typedef uint64_t Bitboard;

/**
 * Set a bit in a bitboard to 1.
 *
 * @param bitBoard Reference to the bitboard you wanna change.
 * @param square Bitboard's square to set to 1.
 *
 * @exception This function does not throw exceptions.
*/
static constexpr void setOne(Bitboard &bitBoard, uint8_t square)
{
    bitBoard = bitBoard | (1ull << square);
}

/**
 * Set a bit in a bitboard to 0.
 *
 * @param bitBoard Reference to the bitboard you wanna change.
 * @param square Bitboard's square to set to 0.
 *
 * @exception This function does not throw exceptions.
*/
static constexpr void setZero(Bitboard &bitBoard, uint8_t square)
{
    bitBoard = bitBoard & (~(1ull << square));
}

/**
 * Get the value of a bit in a bitboard.
 *
 * @param bitBoard Bitboard you wanna get info about.
 * @param square Bitboard's square you wanna get value for.
 * 
 * @return Value (0/1) of the requested square.
 *
 * @exception This function does not throw exceptions.
*/
static constexpr bool getBit(Bitboard bitBoard, uint8_t square)
{
    return (bitBoard & (1ull << square));
}

/**
 * Count the amount of ones in a bitboard.
 *
 * @param bitBoard Bitboard you wanna get info about.
 * 
 * @return Amount of squares with value 1.
 *
 * @exception This function does not throw exceptions.
*/
static constexpr uint8_t countOnes(Bitboard bitBoard)
{
    return popcount(bitBoard);
}

// https://www.chessprogramming.org/BitScan
static constexpr array<uint8_t, 64> BitScanTable = {
    0, 47, 1, 56, 48, 27, 2, 60,
    57, 49, 41, 37, 28, 16, 3, 61,
    54, 58, 35, 52, 50, 42, 21, 44,
    38, 32, 29, 23, 17, 11, 4, 62,
    46, 55, 26, 59, 40, 36, 15, 53,
    34, 51, 20, 43, 31, 22, 10, 45,
    25, 39, 14, 33, 19, 30, 9, 24,
    13, 18, 8, 12, 7, 6, 5, 63};

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

// Bitboard rows
namespace BitboardRows
{
    static consteval array<Bitboard, 8> calculateRows()
    {
        array<Bitboard, 8> rows{};

        for (uint8_t y = 0; y < 8; y++)
        {
            for (uint8_t x = 0; x < 8; x++)
            {
                setOne(rows[y], y * 8 + x);
            }
        }

        return rows;
    }

    static consteval array<Bitboard, 2> calculateSideRows()
    {
        array<Bitboard, 2> rows{};

        for (uint8_t y = 0; y < 4; y++)
        {
            for (uint8_t x = 0; x < 8; x++)
            {
                setOne(rows[0], y * 8 + x);
            }
        }

        for (uint8_t y = 4; y < 8; y++)
        {
            for (uint8_t x = 0; x < 8; x++)
            {
                setOne(rows[1], y * 8 + x);
            }
        }

        return rows;
    }

    static constexpr array<Bitboard, 8> Rows = calculateRows();
    static constexpr array<Bitboard, 2> SideRows = calculateSideRows();

    static consteval array<Bitboard, 8> calculateInversionRows()
    {
        array<Bitboard, 8> inversionRows{};

        for (uint8_t i = 0; i < 8; i++)
        {
            inversionRows[i] = ~Rows[i];
        }

        return inversionRows;
    }

    static constexpr array<Bitboard, 8> InversionRows = BitboardRows::calculateInversionRows();
}

// Bitboard columns
namespace BitboardColumns
{
    static consteval array<Bitboard, 8> calculateColumns()
    {
        array<Bitboard, 8> columns{};

        for (uint8_t x = 0; x < 8; x++)
        {
            for (uint8_t y = 0; y < 8; y++)
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

        for (uint8_t i = 0; i < 8; i++)
        {
            inversionColumns[i] = ~Columns[i];
        }

        return inversionColumns;
    }

    static constexpr array<Bitboard, 8> InversionColumns = calculateInversionColumns();
}