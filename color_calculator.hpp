#pragma once


class ColorCalculator {
public:
    ColorCalculator(unsigned num_iter = 30);

    sf::Color get_color(const float x, const float y);
    
    static sf::Color get_averaged_color(const sf::Color c1, const sf::Color c2, const float frac);

private:
    unsigned num_iter;
};