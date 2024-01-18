#pragma once

#include "bitboard.h"
#include <array>

namespace attacks
{

extern std::array<BitBoard, 64> adjacentSquares;
extern std::array<BitBoard, 64> doubleMoves;

void init();

inline BitBoard singleMoves(BitBoard bb)
{
    return
        ((bb << 1) |
        (bb >> 1) |
        (bb << 7) |
        (bb >> 7) |
        (bb << 8) |
        (bb >> 8) |
        (bb << 9) |
        (bb >> 9)) & VALID_SQUARES;
}
    
inline BitBoard adjSquaresBB(int square)
{
    return adjacentSquares[square];
}

inline BitBoard doubleMovesBB(int square)
{
    return doubleMoves[square];
}
    
}