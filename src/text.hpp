#ifndef TEXT_HPP
#define TEXT_HPP

#include "Window.hpp"
#include "geometry.hpp"
#include "font.hpp"
#include <string>

static constexpr int SPACE_SIZE = 1;  // Pixels between letters.
static constexpr int LETTER_WIDTH = 5;
static constexpr int LETTER_HEIGHT = 7; 

struct TextArea {
    float length;     // Along the reading direction.
    float thickness;  // Across reading direction (7 * pixel_size)
};

enum class Direction { Right, Left, Down, Up };

/// @brief This function writes to the window a paramterized custom text.
/// @param win The window to draw to.
/// @param position The text starting position (upper left).
/// @param direction In which direction to write the text.
/// @param message The text content.
/// @param pixel_size The number of window pixels taken by a pixel in the font.
/// @param color The text color.
void write(
    gfx::Window& win,
    Vector2D position,
    Direction direction,
    const std::string& message,
    int pixel_size,
    gfx::Color color
);

/// @brief Measures the area taken by a given text message written with a given pixel size.
/// @return a TextArea representing the number of pixels taken by the text.
TextArea measure_text(const std::string& message, int pixel_size);


/// @brief This function writes to the window a custom text, centered.
/// @param win The window to draw to.
/// @param position The center position of the text.
/// @param direction In which direction to write the text.
/// @param message The text content.
/// @param pixel_size The number of window pixels taken by a pixel in the font.
/// @param color The text color.
void write_centered(
    gfx::Window& win, 
    Vector2D center, 
    Direction direction,
    const std::string& message, 
    int pixel_size, 
    gfx::Color color
);

#endif
