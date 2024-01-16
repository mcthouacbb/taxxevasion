#include <iostream>
#include "attacks.h"
#include "board.h"

const std::string positions[] = {
    {"7/7/7/7/7/7/7 x 0 1"},
    {"7/7/7/7/7/7/7 o 0 1"},
    {"x5o/7/7/7/7/7/o5x x 0 1"},
    {"x5o/7/7/7/7/7/o5x o 0 1"},
    {"x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1"},
    {"x5o/7/2-1-2/7/2-1-2/7/o5x o 0 1"},
    {"x5o/7/2-1-2/3-3/2-1-2/7/o5x x 0 1"},
    {"x5o/7/2-1-2/3-3/2-1-2/7/o5x o 0 1"},
    {"x5o/7/3-3/2-1-2/3-3/7/o5x x 0 1"},
    {"x5o/7/3-3/2-1-2/3-3/7/o5x o 0 1"},
    {"7/7/7/7/ooooooo/ooooooo/xxxxxxx x 0 1"},
    {"7/7/7/7/ooooooo/ooooooo/xxxxxxx o 0 1"},
    {"7/7/7/7/xxxxxxx/xxxxxxx/ooooooo x 0 1"},
    {"7/7/7/7/xxxxxxx/xxxxxxx/ooooooo o 0 1"},
    {"7/7/7/2x1o2/7/7/7 x 0 1"},
    {"7/7/7/2x1o2/7/7/7 o 0 1"},
    {"x5o/7/7/7/7/7/o5x x 100 1"},
    {"x5o/7/7/7/7/7/o5x o 100 1"},
    {"7/7/7/7/-------/-------/x5o x 0 1"},
    {"7/7/7/7/-------/-------/x5o o 0 1"},
};

int main()
{
    attacks::init();
    for (int sq = 0; sq < 64; sq++)
    {
        printBB(attacks::doubleMovesBB(sq));
    }
    for (auto& pos : positions)
    {
        Board board;
        std::cout << "fen: " << pos << std::endl;
        board.setToFen(pos);
        std::cout << "fen got: " << board.fenStr() << std::endl;
        std::string stringRep = board.stringRep();
        std::cout << stringRep << std::endl;
        std::cout << "\n\n";
    }
    return 0;
}