#pragma once

#include <SFML/Graphics.hpp>
#include "color_calculator.hpp"
#include "util.hpp"
#include "constants.hpp"


class Displayer {
public:
    Displayer(
        const sf::Vector2f& init_bounds_x = INIT_BOUNDS_X,
        const sf::Vector2f& init_bounds_y = INIT_BOUNDS_Y,
        const unsigned num_pix_x = NUM_PIX_X,
        const unsigned num_pix_y = NUM_PIX_Y,
        const float scroll_zoom_factor = SCROLL_ZOOM_FACTOR
    );

    void display(const bool continuous_update = true); // main function

    void handle_events(); // event loop for window

    void update_image(); // update image
    void update_window(); // use updated image to update frame on window

    void translate_reg(const sf::Vector2i& mouse_pos_change); // mouse click and drag
    void zoom_reg(const float mouse_scroll_delta); // mouse scroll

private:
    CoordRegion reg;
    ColorCalculator color_calculator;
    float scroll_zoom_factor;

    // display related variables
    sf::Image image; // pixels that are dispalyed on the window
    sf::Texture texture; // texture takes in an image
    sf::Sprite sprite; // sprite takes in a texture
    sf::RenderWindow window; // windows takes in a sprite

    // mouse position variables used in event handling
    sf::Vector2i cur_mouse_pos;
    sf::Vector2i prev_mouse_pos;
    bool mouse_pressed;
};