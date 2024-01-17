#include "board.h"
#include "utils/static_vector.h"

using MoveList = StaticVector<Move, 256>;

void genMoves(const Board& board, MoveList& moveList);
