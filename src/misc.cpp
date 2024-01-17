#include "board.h"
#include "movegen.h"

const std::pair<std::string, std::vector<std::uint64_t>> positions[] = {
    {"7/7/7/7/7/7/7 x 0 1", {1, 0, 0, 0, 0}},
    {"7/7/7/7/7/7/7 o 0 1", {1, 0, 0, 0, 0}},
    {"x5o/7/7/7/7/7/o5x x 0 1", {1, 16, 256, 6460, 155888, 4752668}},
    {"x5o/7/7/7/7/7/o5x o 0 1", {1, 16, 256, 6460, 155888, 4752668}},
    {"x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1", {1, 14, 196, 4184, 86528, 2266352}},
    {"x5o/7/2-1-2/7/2-1-2/7/o5x o 0 1", {1, 14, 196, 4184, 86528, 2266352}},
    {"x5o/7/2-1-2/3-3/2-1-2/7/o5x x 0 1", {1, 14, 196, 4100, 83104, 2114588}},
    {"x5o/7/2-1-2/3-3/2-1-2/7/o5x o 0 1", {1, 14, 196, 4100, 83104, 2114588}},
    {"x5o/7/3-3/2-1-2/3-3/7/o5x x 0 1", {1, 16, 256, 5948, 133264, 3639856}},
    {"x5o/7/3-3/2-1-2/3-3/7/o5x o 0 1", {1, 16, 256, 5948, 133264, 3639856}},
    {"7/7/7/7/ooooooo/ooooooo/xxxxxxx x 0 1", {1, 1, 75, 249, 14270, 452980}},
    {"7/7/7/7/ooooooo/ooooooo/xxxxxxx o 0 1", {1, 75, 249, 14270, 452980}},
    {"7/7/7/7/xxxxxxx/xxxxxxx/ooooooo x 0 1", {1, 75, 249, 14270, 452980}},
    {"7/7/7/7/xxxxxxx/xxxxxxx/ooooooo o 0 1", {1, 1, 75, 249, 14270, 452980}},
    {"7/7/7/2x1o2/7/7/7 x 0 1", {1, 23, 419, 7887, 168317, 4266992}},
    {"7/7/7/2x1o2/7/7/7 o 0 1", {1, 23, 419, 7887, 168317, 4266992}},
    {"x5o/7/7/7/7/7/o5x x 100 1", {1, 0, 0, 0, 0}},
    {"x5o/7/7/7/7/7/o5x o 100 1", {1, 0, 0, 0, 0}},
    {"7/7/7/7/-------/-------/x5o x 0 1", {1, 2, 4, 13, 30, 73, 174}},
    {"7/7/7/7/-------/-------/x5o o 0 1", {1, 2, 4, 13, 30, 73, 174}},
};

std::string squareStr(int sq)
{
    std::string result;
    result += static_cast<char>(sq % 8 + 'a');
    result += static_cast<char>(sq / 8 + '1');
    return result;
}

std::string moveStr(Move move)
{
    if (move.isDouble())
    {
        return squareStr(move.srcPos()) + squareStr(move.dstPos());
    }
    else
    {
        return squareStr(move.dstPos());
    }
}

template<bool print, bool bulk>
uint64_t perft(Board& board, int depth)
{
    if (depth == 0)
        return 1;
    MoveList moveList;
    genMoves(board, moveList);
    if (bulk && depth == 1)
        return moveList.size();
    uint64_t nodes = 0;
    for (auto move : moveList)
    {
        board.makeMove(move);
        uint64_t subNodes = perft<false, bulk>(board, depth - 1);
        if constexpr (print)
        {
            std::cout << moveStr(move) << ": " << subNodes << std::endl;
            // std::cout << board.fenStr() << std::endl;
        }
        nodes += subNodes;
        board.unmakeMove();
    }
    return nodes;
}

template uint64_t perft<true, true>(Board& board, int depth);
template uint64_t perft<false, true>(Board& board, int depth);
template uint64_t perft<true, false>(Board& board, int depth);
template uint64_t perft<false, false>(Board& board, int depth);

void runPerftTests()
{
    for (const auto& [fen, nodeCounts] : positions)
    {
        Board board;
        board.setToFen(fen);
        for (int depth = 1; depth < static_cast<int>(nodeCounts.size()); depth++)
        {
            uint64_t nodes = perft<false, false>(board, depth);
            bool correct = nodes == nodeCounts[depth];
            std::cout << fen << ", " << depth << ", " << nodes << ", " << (correct ? "pass" : "fail") << std::endl;
        }
    }
}

void printBoard(const Board& board)
{
    std::cout << board.stringRep() << std::endl;
    std::cout << "fen: " << board.fenStr() << std::endl;
    std::cout << "side to move: " << (board.sideToMove() == Color::WHITE ? "white" : "black") << std::endl;
    std::cout << "half move clock: " << board.halfMoveClock() << std::endl;
}
