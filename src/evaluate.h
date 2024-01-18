#include "board.h"

int evaluate(const Board& board)
{
    return 100 * (getPopcnt(board.pieces(board.sideToMove())) - getPopcnt(board.pieces(flip(board.sideToMove()))));
}