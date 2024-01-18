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

struct Move
{
public:
    Move() = default;
    Move(int dst);
    Move(int src, int dst);

    bool operator==(const Move& other) const = default;
    bool operator!=(const Move& other) const = default;

    int srcPos() const;
    int dstPos() const;
    int fromTo() const;
    bool isDouble() const;
    bool isNull() const;
private:
    static constexpr int TYPE_MASK = 1 << 12;

    uint16_t m_Data;
};

inline Move::Move(int dst)
{
    m_Data = static_cast<uint16_t>(dst | (63 << 6));
}

inline Move::Move(int src, int dst)
{
    m_Data = static_cast<uint16_t>(dst | (src << 6));
}


inline int Move::srcPos() const
{
    return (m_Data >> 6) & 63;
}

inline int Move::dstPos() const
{
    return m_Data & 63;
}

inline int Move::fromTo() const
{
    return m_Data & 4095;
}

inline bool Move::isDouble() const
{
    return srcPos() != 63;
}

inline bool Move::isNull() const
{
    return m_Data == 0;
}

constexpr int MAX_PLY = 128;

constexpr int SCORE_MAX = 32767;
constexpr int SCORE_WIN = 32700;
constexpr int SCORE_WIN_IN_MAX = SCORE_WIN - MAX_PLY;
constexpr int SCORE_DRAW = 0;
constexpr int SCORE_NONE = -32701;

inline bool isMateScore(int score)
{
    return std::abs(score) >= SCORE_WIN_IN_MAX;
}
