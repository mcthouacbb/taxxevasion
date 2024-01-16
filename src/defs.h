#pragma once

#include <cstdint>
#include <iostream>
#include <bitset>
#include <bit>
#include <cassert>
#include <cstdint>

enum class Color
{
    WHITE,
    BLACK
};

enum class Piece
{
    NONE,
    WHITE,
    BLACK,
    BLOCKER
};

inline Color flip(Color c)
{
    return static_cast<Color>(static_cast<int>(c) ^ 1);
}

template<Color c>
inline constexpr Color flip()
{
    return static_cast<Color>(static_cast<int>(c) ^ 1);
}

enum class MoveType
{
    SINGLE,
    DOUBLE = 1 << 12
};

struct Move
{
public:
    Move() = default;
    Move(int src, int dst, MoveType type);

    bool operator==(const Move& other) const = default;
    bool operator!=(const Move& other) const = default;

    int srcPos() const;
    int dstPos() const;
    int fromTo() const;
    MoveType type() const;
private:
    static constexpr int TYPE_MASK = 1 << 12;

    uint16_t m_Data;
};

inline Move::Move(int src, int dst, MoveType type)
    : m_Data(0)
{
    assert(src >= 0 && src < 49 && "Src pos is out of range");
    assert(dst >= 0 && dst < 49 && "Dst pos is out of range");
    m_Data = static_cast<uint16_t>(src | (dst << 6) | static_cast<int>(type));
}


inline int Move::srcPos() const
{
    return m_Data & 63;
}

inline int Move::dstPos() const
{
    return (m_Data >> 6) & 63;
}

inline int Move::fromTo() const
{
    return m_Data & 4095;
}

inline MoveType Move::type() const
{
    return static_cast<MoveType>(m_Data & TYPE_MASK);
}

constexpr int MAX_PLY = 128;

constexpr int SCORE_MAX = 32767;
constexpr int SCORE_MATE = 32700;
constexpr int SCORE_MATE_IN_MAX = SCORE_MATE - MAX_PLY;
constexpr int SCORE_WIN = 31000;
constexpr int SCORE_DRAW = 0;
constexpr int SCORE_NONE = -32701;

inline bool isMateScore(int score)
{
    return std::abs(score) >= SCORE_MATE_IN_MAX;
}
