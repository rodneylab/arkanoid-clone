#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants
{
inline constexpr unsigned int kWindowWidth{800U};
inline constexpr unsigned int kWindowHeight{600U};
inline constexpr unsigned int kFramerateLimit{60U};

inline constexpr float kBallRadius{10.F};
inline constexpr float kBallVelocity{8.F};

inline constexpr float kBlockWidth{60.F};
inline constexpr float kBlockHeight{20.F};
inline constexpr int kCountBlocksX{11};
inline constexpr int kCountBlocksY{4};
inline constexpr float kBlocksInsetX{22.F};

inline constexpr float kPaddleWidth{60.F};
inline constexpr float kPaddleHeight{20.F};
inline constexpr float kPaddleVelocity{6.F};
inline constexpr float kPaddleInsetBottom{50.F};
} // namespace constants

#endif
