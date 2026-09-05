#include "text.hpp"

#include <algorithm>

using namespace std;

/// @brief Used as a vector basis to write text along several directions.
struct Basis {
    Vector2D horizontal, vertical;
};

Basis basis_for(Direction d) {
    switch (d) {
        case Direction::Right: return {{ 1, 0}, { 0, 1}};
        case Direction::Down:  return {{ 0, 1}, {-1, 0}};
        case Direction::Left:  return {{-1, 0}, { 0,-1}};
        case Direction::Up:    return {{ 0,-1}, { 1, 0}};
        default:
            return {{1, 0}, {0, 1}};
    }
}

void draw_glyph(
    gfx::Window& win, 
    Vector2D origin,
    const Glyph& glyph, 
    const Basis& basis,
    int pixel_size, 
    gfx::Color color
) {
    for (int row = 0; row < LETTER_HEIGHT; ++row) {
        uint8_t bits = glyph[row];

        for (int col = 0; col < LETTER_WIDTH; ++col) {            
            if (bits & (1 << (4 - col))) {
                Vector2D p = origin
                    + basis.horizontal * float(col * pixel_size)
                    + basis.vertical  * float(row * pixel_size);

                win.fillRect(
                    int(p.x()), int(p.y()), 
                    pixel_size, pixel_size, 
                    color
                );
            }
        }
    }
}

void write(
    gfx::Window& win,
    Vector2D position,
    Direction direction,
    const std::string& message,
    int pixel_size,
    gfx::Color color
) {
    if (pixel_size <= 0) 
        pixel_size = 1;

    Basis basis = basis_for(direction);
    Vector2D cursor = position;

    for (char c : message) {
        draw_glyph(win, cursor, to_glyph(c), basis, pixel_size, color);
        cursor += basis.horizontal * float((LETTER_WIDTH + SPACE_SIZE) * pixel_size);
    }
}

TextArea measure_text(const string& message, int pixel_size) {
    return TextArea{
        float(message.size()) * (LETTER_WIDTH + SPACE_SIZE) * pixel_size - pixel_size,
        float(LETTER_HEIGHT * pixel_size)
    };
}

void write_centered(
    gfx::Window& win, 
    Vector2D center, 
    Direction direction,
    const string& message, 
    int pixel_size, 
    gfx::Color color
) {
    TextArea area = measure_text(message, pixel_size);
    Basis basis = basis_for(direction);

    // fillRect() always grows +x / +y from a cell's logical position, so any
    // basis component pointing in the negative direction leaves the glyph
    // block shifted by one pixel_size along that axis. 
    Vector2D correction(
        float(pixel_size) * (
            min(0.f, basis.horizontal.x()) + min(0.f, basis.vertical.x())
        ),
        float(pixel_size) * (
            min(0.f, basis.horizontal.y()) + min(0.f, basis.vertical.y())
        )
    );

    Vector2D text_position = center
        - basis.horizontal * (area.length / 2.f)
        - basis.vertical   * (area.thickness / 2.f)
        + correction;

    write(
        win,
        text_position,
        direction,
        message,
        pixel_size,
        color
    );
}