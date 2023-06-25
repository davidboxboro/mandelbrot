#pragma once

#include <SFML/Graphics.hpp>
#include "color_calculator.hpp"
#include "util.hpp"
#include "constants.hpp"


class Displayer {
public:
    Displayer(
        const sf::Vector2f& init_bounds_x = INIT_BOUNDS_X,
        const unsigned num_pix_x = NUM_PIX_X,
        const float scroll_zoom_factor = SCROLL_ZOOM_FACTOR,
        const float keyboard_scroll_delta = KEYBOARD_SCROLL_DELTA
    );

    void display(const bool continuous_update = true); // main function

    void handle_events(); // event loop for window

    // main computation on each frame
    void update_image();
    void update_image_simd(); // faster version

    void update_window(); // use updated image to update frame on window

    void translate_reg(const sf::Vector2i& mouse_pos_change); // mouse click and drag
    void zoom_reg(const float mouse_scroll_delta); // mouse scroll

    // used to initialize reg member var
    sf::Vector2f compute_bounds_y(const sf::Vector2f& bounds_x);
    unsigned compute_num_pix_y(const unsigned num_pix_x);

private:
    ColorCalculator color_calculator;
    float scroll_zoom_factor;
    float keyboard_scroll_delta;

    const unsigned desktop_width;
    const unsigned desktop_height;
    CoordRegion reg;

    sf::Image image; // pixels that are dispalyed on the window
    sf::Texture texture; // texture takes in an image
    sf::Sprite sprite; // sprite takes in a texture
    sf::RenderWindow window; // windows takes in a sprite

    // mouse position variables used in event handling
    sf::Vector2i cur_mouse_pos;
    sf::Vector2i prev_mouse_pos;
    bool mouse_pressed;
};