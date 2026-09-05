#ifndef FONT_HPP
#define FONT_HPP

#include <array>
#include <cstdint>

/// @brief Represents a 7 rows x 5 columns character using a standard font.
/// @note The 5 LSBs of the first array component are used.
using Glyph = std::array<uint8_t, 7>;

/// @brief Returns the bitmap for a given character.
/// @warning Unsupported characters default to a blank glyph.
Glyph to_glyph(char c);

/// @brief Helper function to print a glyph into the terminal.
void print_glyph(char c);

#endif
