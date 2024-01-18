#pragma once

#include "time_man.h"
#include "board.h"
#include "movegen.h"
#include <vector>

struct SearchStack
{
    std::array<Move, MAX_PLY + 1> pv;
    int pvLength;
};

struct SearchInfo
{
    int depth;
    uint64_t nodes;
    Duration time;
    std::vector<Move> pv;
    int score;
};

void printSearchInfo(const SearchInfo& searchInfo);
void printBestMove(Move move);

class Search
{
public:
    Search();

    void run(const Board& board, const SearchLimits& limits);
private:
    int iterDeep(bool report, bool normalSearch);
    int search(int depth, SearchStack* stack, int ply);

    Board m_Board;
    TimeManager m_TimeMan;
    bool m_ShouldStop;

    uint64_t m_Nodes;
    SearchLimits m_Limits;
    std::array<SearchStack, MAX_PLY + 1> m_Stack;
};