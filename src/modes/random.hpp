#include "../movegen.h"
#include <random>

Move randomMove(const Board& board, const MoveList& moves)
{
    static std::random_device rd;
    static const unsigned int seed = rd();
    static std::mt19937 rng(seed);

    std::uniform_int_distribution<int> dist(0, moves.size() - 1);
    return moves[dist(rng)];
}
