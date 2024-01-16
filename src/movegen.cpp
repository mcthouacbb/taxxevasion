#include "movegen.h"
#include "attacks.h"

void genMoves(const Board& board, MoveList& moveList)
{
    assert(moveList.size() == 0);

    BitBoard stmPieces = board.pieces(board.sideToMove());
    BitBoard moveMask = ~(board.pieces(Color::WHITE) | board.pieces(Color::BLACK) | board.blockers());

    BitBoard singles = attacks::singleMoves(stmPieces);
    singles &= moveMask;
    while (singles)
    {
        uint32_t single = popLSB(singles);
        moveList.push_back(Move(single));
    }

    while (stmPieces)
    {
        uint32_t piece = popLSB(stmPieces);
        BitBoard doubles = attacks::doubleMovesBB(piece);
        doubles &= moveMask;
        while (doubles)
        {
            uint32_t doubleMove = popLSB(doubles);
            moveList.push_back(Move(piece, doubleMove));
        }
    }
}
