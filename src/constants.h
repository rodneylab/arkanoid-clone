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

inline constexpr float kBallRadius{8.F};
inline constexpr float kBallVelocity{0.8F};

inline constexpr int kWallWidth{24};

inline constexpr float kBrickWidth{42.F};
inline constexpr float kBrickHeight{16.F};
inline constexpr float kBrickPadding{3.F};
inline constexpr int kBricksInsetX{kWallWidth};
inline constexpr int kBricksInsetY{80};
inline constexpr int kBrickColumns{11};
inline constexpr float kBricksWidth{
    constants::kBrickColumns *
    (constants::kBrickWidth + 1.F * constants::kBrickPadding)};
inline constexpr int kBrickDestructionPoints{10};

inline constexpr int kHudFontSize{18};
inline constexpr float kHudFontSpacing{1.F};

inline constexpr int kPaddleWidth{72};
inline constexpr int kPaddleHeight{16};
inline constexpr float kPaddleVelocity{0.6F};
inline constexpr float kPaddleInsetBottom{30.F};

inline constexpr int kDefaultHighScore{50'000};

} // namespace constants

#endif
