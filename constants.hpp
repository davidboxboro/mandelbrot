#pragma once

#include <SFML/Graphics.hpp>


const float SCROLL_ZOOM_FACTOR = 1.015; // how much to zoom in on each mouse scroll
const float KEYBOARD_SCROLL_DELTA = 2; // how much to zoom in and out when zoom controlled by key press

// num pixels in y direction computed to match screen size
const unsigned NUM_PIX_X = 1920;

// bounds in y direction computed to match screen size
const float INIT_MIN_X = -2.5; // x-coordinate of left edge of leftmost pixel
const float INIT_MAX_X = 1.6;
const sf::Vector2f INIT_BOUNDS_X{INIT_MIN_X, INIT_MAX_X};

const sf::Color EARLY_ESCAPE_COLOR{30, 30, 200};
const sf::Color LATE_ESCAPE_COLOR{sf::Color::White};
const sf::Color NO_ESCAPE_COLOR{sf::Color::Black};

const unsigned SIMD_MULTIPLIER = 8; // number of floats that fit into one register

const unsigned NUM_ITER = 200;
static_assert(NUM_ITER % SIMD_MULTIPLIER == 0); // because of SIMD optimization