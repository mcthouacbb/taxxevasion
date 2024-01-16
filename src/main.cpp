#include <iostream>
#include <utility>
#include <sstream>
#include "attacks.h"
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
        board.unmakeMove(move);
    }
    return nodes;
}

int main()
{
    attacks::init();

    std::string line;
    while (std::getline(std::cin, line))
    {
        if (line == "tests")
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
            continue;
        }
        std::string fen;
        int depth;
        std::stringstream ss(line);
        for (int i = 0; i < 4; i++)
        {
            std::string x;
            ss >> x;
            fen += x + " ";
        }
        ss >> depth;
        
        Board board;
        board.setToFen(fen);
        uint64_t nodes = perft<true, false>(board, depth);
        std::cout << fen << ", " << depth << ", " << nodes << ", " << std::endl;
    }
    /*for (auto& pos : positions)
    {
        Board board;
        std::cout << "fen: " << pos << std::endl;
        board.setToFen(pos);
        std::cout << "fen got: " << board.fenStr() << std::endl;
        std::string stringRep = board.stringRep();
        std::cout << stringRep << std::endl;
        std::cout << "\n\n";

        Board b = board;
        b.pieces(Color::WHITE);
    }*/
    return 0;
}