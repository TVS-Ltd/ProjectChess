#include <iostream>
#include <bit>
#include <array>

using namespace std;

typedef uint64_t Bitboard;

static constexpr void setOne(Bitboard &bitBoard, uint8_t square)
{
    bitBoard = bitBoard | (1ull << square);
}

static constexpr void setZero(Bitboard &bitBoard, uint8_t square)
{
    bitBoard = bitBoard & (~(1ull << square));
}

static constexpr bool getBit(Bitboard bitBoard, uint8_t square)
{
    return (bitBoard & (1ull << square));
}

static constexpr uint8_t countOnes(Bitboard bitBoard)
{
    return popcount(bitBoard);
}

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

int main()
{


    return 0;
}