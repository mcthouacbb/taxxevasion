#pragma once

#include <array>
#include <vector>
#include <string>
#include "defs.h"
#include "bitboard.h"

struct BoardState
{
    int halfMoveClock;
    std::array<BitBoard, 2> pieces;
};

class Board
{
public:
    static constexpr const char* DEFAULT_FEN = "x5o/7/7/7/7/7/o5x x 0 1";
    Board();
    
    Board(const Board&) = default;
    Board& operator=(const Board&) = default;
    
    Board(Board&&) = default;
    Board& operator=(Board&&) = default;
    
    void setToFen(const std::string_view& fen);

    std::string stringRep() const;
    std::string fenStr() const;

    Color sideToMove() const;
    Piece pieceAt(int sq) const;
    BitBoard pieces(Color color) const;
    BitBoard blockers() const;

    void makeMove(Move move);
    void unmakeMove(Move move);
private:
    BoardState& state();
    const BoardState& state() const;

    BitBoard m_Blockers;
    Color m_SideToMove;
    std::vector<BoardState> m_States;
};

inline Color Board::sideToMove() const
{
    return m_SideToMove;
}

inline Piece Board::pieceAt(int sq) const
{
    if (pieces(Color::WHITE) & (1ull << sq))
        return Piece::WHITE;
    if (pieces(Color::BLACK) & (1ull << sq))
        return Piece::BLACK;
    if (m_Blockers & (1ull << sq))
        return Piece::BLOCKER;
    return Piece::NONE;
}

inline BitBoard Board::pieces(Color color) const
{
    return state().pieces[static_cast<int>(color)];
}

inline BitBoard Board::blockers() const
{
    return m_Blockers;
}

inline BoardState& Board::state()
{
    return m_States.back();
}

inline const BoardState& Board::state() const
{
    return m_States.back();
}
