#pragma once

#include "../movegen.h"
#include "random.hpp"
#include "../attacks.h"
#include <random>

Move mostCaptures(const Board& board, const MoveList& moves)
{
    int mostCaptures = 0;
    for (auto& move : moves)
    {
        int captures = !move.isDouble() + getPopcnt(attacks::adjSquaresBB(move.dstPos()) & board.pieces(flip(board.sideToMove())));
        if (captures > mostCaptures)
            mostCaptures = captures;
    }

    MoveList candidates;
    for (auto& move : moves)
    {
        int captures = !move.isDouble() + getPopcnt(attacks::adjSquaresBB(move.dstPos()) & board.pieces(flip(board.sideToMove())));
        if (captures == mostCaptures)
            candidates.push_back(move);
    }

    return randomMove(candidates);
}
