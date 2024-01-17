#include "board.h"
#include "attacks.h"

#include <charconv>

Board::Board()
{
    m_States.reserve(4096);
    setToFen(DEFAULT_FEN);
}

void Board::setToFen(const std::string_view& fen)
{
    m_States.resize(1);
    state() = {};
    m_Blockers = {};

    int i = 0;
    int sq = 48;
    for (; fen[i] != ' '; i++)
    {
        switch (fen[i])
        {
            case 'x':
            case 'X':
            case 'b':
            case 'B':
                    state().pieces[static_cast<int>(Color::BLACK)] |= 1ull << sq++;
                break;
            case 'o':
            case 'O':
            case 'w':
            case 'W':
                state().pieces[static_cast<int>(Color::WHITE)] |= 1ull << sq++;
                break;
            case '-':
                m_Blockers |= 1ull << sq++;
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                sq += fen[i] - '0';
                break;
            case '/':
                sq -= 15;
                break;
            default:
                // shouldn't happen
                break;
        }
    }

    i++;
    switch (fen[i])
    {
        case 'o':
        case 'O':
        case 'w':
        case 'W':
        default:
            m_SideToMove = Color::WHITE;
            break;
        case 'x':
        case 'X':
        case 'b':
        case 'B':
            m_SideToMove = Color::BLACK;
            break;
    }

    i += 2;
    std::from_chars(&fen[i], fen.data() + fen.size(), state().halfMoveClock);
}

constexpr std::array<char, 4> pieceChars = {' ', 'o', 'x', '-'};

std::string Board::stringRep() const
{
    std::string result;
    const char* between = "+---+---+---+---+---+---+---+\n";
    for (int j = 48; j >= 0; j -= 8)
    {
        result += between;
        for (int i = j; i < j + 7; i++)
        {
            if (pieceAt(i) == Piece::BLOCKER)
                result += "|...";
            else
            {
                result += "| ";
                result += pieceChars[static_cast<int>(pieceAt(i))];
                result += " ";
            }
        }
        result += "|\n";
    }
    result += between;
    return result;
}

std::string Board::fenStr() const
{
    std::string fen = "";
    int lastFile;
    for (int j = 48; j >= 0; j -= 8)
    {
        lastFile = -1;
        for (int i = j; i < j + 7; i++)
        {
            Piece pce = pieceAt(i);
            if (pce != Piece::NONE)
            {
                int diff = i - j - lastFile;
                if (diff > 1)
                    fen += static_cast<char>((diff - 1) + '0');
                fen += pieceChars[static_cast<int>(pce)];
                lastFile = i - j;
            }
        }
        int diff = 7 - lastFile;
        if (diff > 1)
            fen += static_cast<char>((diff - 1) + '0');
        if (j != 0)
            fen += '/';
    }

    fen += ' ';
    fen += m_SideToMove == Color::WHITE ? "o " : "x ";
    fen += std::to_string(state().halfMoveClock);
    fen += " 1";

    return fen;
}

void Board::makeMove(Move move)
{
    if (move.isNull())
    {
        makeNullMove();
        return;
    }
    m_States.push_back(state());
    state().halfMoveClock++;

    if (move.isDouble())
        state().pieces[static_cast<int>(m_SideToMove)] ^= (1ull << move.srcPos());
    else
        state().halfMoveClock = 0;

    state().pieces[static_cast<int>(m_SideToMove)] |= (1ull << move.dstPos());

    BitBoard adjOpps = state().pieces[static_cast<int>(flip(m_SideToMove))] & attacks::adjSquaresBB(move.dstPos());
    state().pieces[static_cast<int>(m_SideToMove)] |= adjOpps;
    state().pieces[static_cast<int>(flip(m_SideToMove))] ^= adjOpps;

    m_SideToMove = flip(m_SideToMove);
}

void Board::unmakeMove()
{
    m_SideToMove = flip(m_SideToMove);
    m_States.pop_back();
}

void Board::makeNullMove()
{
    m_States.push_back(state());
    m_SideToMove = flip(m_SideToMove);
}

bool Board::gameOver() const
{
    if (pieces(Color::WHITE) == 0)
        return true;
    if (pieces(Color::BLACK) == 0)
        return true;
    if (halfMoveClock() >= 100)
        return true;
    BitBoard allMoves = (empty() & attacks::singleMoves(attacks::singleMoves(pieces(Color::WHITE) | pieces(Color::BLACK))));
    if (allMoves == 0)
        return true;
    return false;
}
