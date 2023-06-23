#pragma once

#include <SFML/Graphics.hpp>


const unsigned NUM_PIX_X = 1200;
const unsigned NUM_PIX_Y = 800;

const float SCROLL_ZOOM_FACTOR = 1.01; // how much to zoom in on each mouse scroll

const int INIT_MIN_X = -2; // x-coordinate of left edge of leftmost pixel
const int INIT_MAX_X = 1;
const int INIT_MIN_Y = -1;
const int INIT_MAX_Y = 1;

const sf::Vector2f INIT_BOUNDS_X{INIT_MIN_X, INIT_MAX_X};
const sf::Vector2f INIT_BOUNDS_Y{INIT_MIN_Y, INIT_MAX_Y};

const sf::Color EARLY_ESCAPE_COLOR{30, 30, 200};
const sf::Color LATE_ESCAPE_COLOR{sf::Color::White};
const sf::Color NO_ESCAPE_COLOR{sf::Color::Black};

const unsigned NUM_ITER = 30;