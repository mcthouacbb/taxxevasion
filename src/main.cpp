#include <iostream>
#include <utility>
#include <chrono>
#include <sstream>

#include "misc.h"
#include "attacks.h"
#include "movegen.h"
#include "modes/random.hpp"

Move moveFromStr(const MoveList& legals, std::string_view moveStr)
{
    int sq1 = (moveStr[0] - 'a') + (moveStr[1] - '1') * 8;
    int sq2 = -1;
    if (moveStr.size() > 2)
        sq2 = (moveStr[2] - 'a') + (moveStr[3] - '1') * 8;

    if (sq2 == -1)
    {
        for (const Move move : legals)
        {
            if (!move.isDouble() && move.dstPos() == sq1)
                return move;
        }
        return Move();
    }
    else
    {
        for (const Move move : legals)
        {
            if (move.isDouble() && move.srcPos() == sq1 && move.dstPos() == sq2)
                return move;
        }
        return Move();
    }
}

int main()
{
    attacks::init();

    Board board;
    MoveList legalMoves;
    genMoves(board, legalMoves);
    std::string line;
    while (std::getline(std::cin, line))
    {
        std::string tok;
        std::stringstream ss(line);
        ss >> tok;
        if (tok == "position")
        {
            ss >> tok;
            if (tok == "startpos")
                board.setToFen(Board::DEFAULT_FEN);
            else if (tok == "fen")
            {
                std::string fen;
                for (int i = 0; i < 4; i++)
                {
                    ss >> tok;
                    fen += tok;
                    if (i != 3)
                        fen += " ";
                }
                board.setToFen(fen);
            }

            legalMoves.clear();
            genMoves(board, legalMoves);
        }
        else if (tok == "randmove")
        {
            std::cout << moveStr(randomMove(board, legalMoves)) << std::endl;
        }
        else if (tok == "move")
        {
            ss >> tok;
            Move move = moveFromStr(legalMoves, tok);
            if (move != Move())
            {
                board.makeMove(move);
                legalMoves.clear();
                genMoves(board, legalMoves);
            }
            else
            {
                std::cout << "Illegal move" << std::endl;
            }
        }
        else if (tok == "perft")
        {
            int depth = 1;
            ss >> depth;
            if (depth < 1)
                depth = 1;

            auto t1 = std::chrono::steady_clock::now();
            uint64_t nodes = perft<true, false>(board, depth);
            auto t2 = std::chrono::steady_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
            std::cout << "Nodes: " << nodes << std::endl;
            std::cout << "Time: " << time.count() << std::endl;
            std::cout << "nps: " << static_cast<uint64_t>(nodes / time.count()) << std::endl;
        }
        else if (tok == "undo")
        {
            board.unmakeMove();
            legalMoves.clear();
            genMoves(board, legalMoves);
        }
        else if (tok == "tests")
        {
            runPerftTests();
        }
        else if (tok == "print")
        {
            printBoard(board);
        }
    }
    return 0;
}