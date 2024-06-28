#ifndef SRC_CONSTANTS_H
#define SRC_CONSTANTS_H

/*! \file src/constants.h
 * \brief Declarations for game constants used throughout the project
 */

#include <string>

namespace constants
{
inline constexpr int kWindowWidth{800U};
inline constexpr int kWindowHeight{450U};
inline const std::string kWindowTitle{"Arkanoid Clone"};

inline constexpr int kTargetFramerate{240};

inline constexpr float kBallRadius{10.F};
inline constexpr float kBallVelocity{0.8F};

inline constexpr float kBrickWidth{60.F};
inline constexpr float kBrickHeight{20.F};
inline constexpr int kBricksInsetX{22};
inline constexpr int kBrickRows{4};
inline constexpr int kBrickColumns{11};

inline constexpr int kPaddleWidth{60};
inline constexpr int kPaddleHeight{20};
inline constexpr float kPaddleVelocity{0.6F};
inline constexpr float kPaddleInsetBottom{30.F};

} // namespace constants

#endif
