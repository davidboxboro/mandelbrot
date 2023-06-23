#pragma once

#include "constants.hpp"

class ColorCalculator {
public:
    ColorCalculator(unsigned num_iter = NUM_ITER);

    sf::Color get_color(const float x, const float y);
    
    static sf::Color get_averaged_color(const sf::Color c1, const sf::Color c2, const float frac);

private:
    unsigned num_iter;
};