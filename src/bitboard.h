#pragma once

#include <cstdint>
#include <iostream>
#include <bit>
#include <bitset>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

using BitBoard = uint64_t;

constexpr BitBoard VALID_SQUARES = 0x7f7f7f7f7f7f7f;

constexpr BitBoard FILE_A = 0x01010101010101ull;
constexpr BitBoard FILE_B = 0x02020202020202ull;
constexpr BitBoard FILE_C = 0x04040404040404ull;
constexpr BitBoard FILE_D = 0x08080808080808ull;
constexpr BitBoard FILE_E = 0x10101010101010ull;
constexpr BitBoard FILE_F = 0x20202020202020ull;
constexpr BitBoard FILE_G = 0x40404040404040ull;

constexpr BitBoard RANK_1 = 0x000000000000007Full;
constexpr BitBoard RANK_2 = 0x0000000000007F00ull;
constexpr BitBoard RANK_3 = 0x00000000007F0000ull;
constexpr BitBoard RANK_4 = 0x000000007F000000ull;
constexpr BitBoard RANK_5 = 0x0000007F00000000ull;
constexpr BitBoard RANK_6 = 0x00007F0000000000ull;
constexpr BitBoard RANK_7 = 0x007F000000000000ull;

constexpr BitBoard LIGHT_SQUARES = 0x55AA55AA55AA55AAull;
constexpr BitBoard DARK_SQUARES = 0xAA55AA55AA55AA55ull;


inline uint8_t reverse(uint8_t b) {
     b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
     b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
     b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
     return b;
}

inline void printBB(BitBoard bb)
{
    for (int i = 0; i < 8; i++)
    {
        uint8_t val = reverse(bb >> 56);
        std::cout << std::bitset<8>(val) << std::endl;
        bb <<= 8;
    }
    std::cout << std::endl;
}

inline BitBoard shiftNorth(BitBoard bb)
{
    return (bb << 8) & VALID_SQUARES;
}

inline BitBoard shiftSouth(BitBoard bb)
{
    return (bb >> 8) & VALID_SQUARES;
}

inline BitBoard shiftEast(BitBoard bb)
{
    return (bb << 1) & VALID_SQUARES;
}

inline BitBoard shiftWest(BitBoard bb)
{
    return (bb >> 1) & VALID_SQUARES;
}

inline BitBoard shiftNorthEast(BitBoard bb)
{
    return shiftNorth(shiftEast(bb));
}

inline BitBoard shiftNorthWest(BitBoard bb)
{
    return shiftNorth(shiftWest(bb));
}

inline BitBoard shiftSouthEast(BitBoard bb)
{
    return shiftSouth(shiftEast(bb));
}

inline BitBoard shiftSouthWest(BitBoard bb)
{
    return shiftSouth(shiftWest(bb));
}

inline uint32_t getLSB(BitBoard bb)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_ctzll(bb);
#elif defined(_MSC_VER)
    unsigned long idx;
    _BitScanForward64(&idx, bb);
    return idx;
#else
    return std::countl_zero(bb);
#endif
}

inline uint32_t getMSB(BitBoard bb)
{
#if defined(__GNUC__) || defined(__clang__)
    return 63 - __builtin_clzll(bb);
#elif defined(_MSC_VER)
    unsigned long idx;
    _BitScanReverse64(&idx, bb);
    return idx;
#else
    return 63 - std::countr_zero(bb);
#endif
}

inline BitBoard extractLSB(BitBoard bb)
{
    return bb & (0 - bb);
}

inline uint32_t popLSB(BitBoard& bb)
{
    uint32_t lsb = getLSB(bb);
    bb &= bb - 1;
    return lsb;
}

inline int getPopcnt(BitBoard bb)
{
    return std::popcount(bb);
}