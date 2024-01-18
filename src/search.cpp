#include "search.h"
#include "evaluate.h"

Search::Search()
    : m_ShouldStop(false), m_Limits(), m_Stack()
{
    
}

void Search::run(const Board& board, const SearchLimits& limits)
{
    m_Board = board;
    m_Limits = limits;
    iterDeep(true, true);
}

int Search::iterDeep(bool report, bool normalSearch)
{
    int maxDepth = std::min(m_Limits.maxDepth, MAX_PLY - 1);
    int score = 0;

    report = report && normalSearch;

    m_TimeMan.startSearch();
    m_Nodes = 0;
    m_ShouldStop = false;
    Move bestMove = Move();

    for (int depth = 1; depth <= maxDepth; depth++)
    {
        int searchScore = search(depth, &m_Stack[0], 0);
        if (m_ShouldStop)
            break;
        score = searchScore;
        if (report)
        {
            SearchInfo info;
            info.nodes = m_Nodes;
            info.depth = depth;
            info.time = m_TimeMan.elapsed();
            info.pv = {m_Stack[0].pv.begin(), m_Stack[0].pv.begin() + m_Stack[0].pvLength};
            info.score = searchScore;
            printSearchInfo(info);
        }
        bestMove = m_Stack[0].pv[0];
    }

    printBestMove(bestMove);

    return score;
}

int Search::search(int depth, SearchStack* stack, int ply)
{
    if (m_Nodes % 1024 == 0 && m_TimeMan.stopHard(m_Limits))
    {
        m_ShouldStop = true;
        return SCORE_NONE;
    }

    stack->pvLength = 0;
    
    if (m_Board.gameOver())
    {
        if (m_Board.halfMoveClock() >= 100)
            return SCORE_DRAW;
        // implicitly handles 0 case
        int us = getPopcnt(m_Board.pieces(m_Board.sideToMove()));
        int them = getPopcnt(m_Board.pieces(flip(m_Board.sideToMove())));
        if (us > them)
            return SCORE_WIN - ply;
        else if (us == them)
            return SCORE_DRAW;
        else if (us != them)
            return -SCORE_WIN + ply;
    }

    if (depth <= 0)
        return evaluate(m_Board);

    MoveList moves;
    genMoves(m_Board, moves);

    int bestScore = -SCORE_MAX;

    for (auto& move : moves)
    {
        m_Nodes++;
        m_Board.makeMove(move);
        int score = -search(depth - 1, stack + 1, ply + 1);
        m_Board.unmakeMove();

        if (m_ShouldStop)
            return SCORE_NONE;

        if (score > bestScore)
        {
            bestScore = score;
            stack->pv[0] = move;
            stack->pvLength = stack[1].pvLength + 1;
            for (int i = 0; i < stack[1].pvLength; i++)
                stack->pv[i + 1] = stack[1].pv[i];
        }
    }

    return bestScore;
}