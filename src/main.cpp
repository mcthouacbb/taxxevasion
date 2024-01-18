#include <iostream>
#include <utility>
#include <chrono>
#include <sstream>

#include "misc.h"
#include "attacks.h"
#include "movegen.h"
#include "modes/random.hpp"
#include "modes/most_captures.hpp"
#include "search.h"

void printSearchInfo(const SearchInfo& info)
{
    std::cout << "info depth " << info.depth;
    int time = info.time.count();
    std::cout << " nodes " << info.nodes << " time " << time << " nps " << info.nodes * 1000 / std::max(time, 1);
    std::cout << " score cp " << info.score;
    std::cout << " pv ";
    for (auto& pvMove : info.pv)
    {
        std::cout << moveStr(pvMove) << " ";
    }
    std::cout << std::endl;
}

void printBestMove(Move move)
{
    std::cout << "bestmove " << moveStr(move) << std::endl;
}

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
    Search search;
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
            std::cout << moveStr(randomMove(legalMoves)) << std::endl;
        }
        else if (tok == "mostcaptures")
        {
            std::cout << moveStr(mostCaptures(board, legalMoves)) << std::endl;
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
        else if (tok == "go")
        {
            SearchLimits limits = {};
            limits.maxDepth = MAX_PLY - 1;
            while (ss)
            {
                ss >> tok;
                if (tok == "depth")
                    ss >> limits.maxDepth;
                else if (tok == "movetime")
                {
                    int time;
                    ss >> time;
                    limits.maxTime = Duration(time);
                }
            }
            search.run(board, limits);
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