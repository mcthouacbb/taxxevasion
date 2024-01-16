#include "attacks.h"

namespace attacks
{
    
std::array<BitBoard, 64> adjacentSquares;
std::array<BitBoard, 64> doubleMoves;
    
void init()
{
    for (int sq = 0; sq < 64; sq++)
    {
        adjacentSquares[sq] = singleMoves(1ull << sq);
        
        BitBoard bb = 1ull << sq;
        BitBoard nn = shiftNorth(shiftNorth(bb));
        BitBoard ss = shiftSouth(shiftSouth(bb));
        BitBoard topBottom = nn | ss;
        topBottom |= shiftWest(topBottom) | shiftWest(shiftWest(topBottom));
        topBottom |= shiftEast(topBottom) | shiftEast(shiftEast(topBottom));

        BitBoard ee = shiftEast(shiftEast(bb));
        BitBoard ww = shiftWest(shiftWest(bb));

        BitBoard leftRight = ee | ww;
        leftRight |= shiftNorth(leftRight) | shiftSouth(leftRight);
        
        doubleMoves[sq] = leftRight | topBottom;
    }
}

    
}