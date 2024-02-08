#ifndef SRC_CONSTANTS_H
#define SRC_CONSTANTS_H

namespace constants
{
inline constexpr unsigned int kWindowWidth{800U};
inline constexpr unsigned int kWindowHeight{600U};
inline constexpr unsigned int kFramerateLimit{240U};
inline constexpr float kFrameTimestep{1.F};
inline constexpr float kFrameTimeSlice{1.F};

inline constexpr float kBallRadius{10.F};
inline constexpr float kBallVelocity{0.8F};

inline constexpr float kBlockWidth{60.F};
inline constexpr float kBlockHeight{20.F};
inline constexpr int kCountBlocksX{11};
inline constexpr int kCountBlocksY{4};
inline constexpr float kBlocksInsetX{22.F};

inline constexpr float kPaddleWidth{60.F};
inline constexpr float kPaddleHeight{20.F};
inline constexpr float kPaddleVelocity{0.6F};
inline constexpr float kPaddleInsetBottom{50.F};
} // namespace constants

#endif
