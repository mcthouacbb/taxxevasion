#include "defs.h"
#include "board.h"

std::string squareStr(int sq);
std::string moveStr(Move move);

template<bool print, bool bulk>
uint64_t perft(Board& board, int depth);

void runPerftTests();

void printBoard(const Board& board);
