#include <SFML/Graphics.hpp>
#include <complex>
#include <iostream>
#include "color_calculator.hpp"
#include "constants.hpp"


ColorCalculator::ColorCalculator(unsigned num_iter)
    : num_iter{num_iter} {
}

sf::Color ColorCalculator::get_color(const float x, const float y) {
    const std::complex<float> c{x, y};
    std::complex<float> z{0, 0};
    for (int i = 0; i < num_iter; i++) {
        if (std::abs(z) > 2) {
            // escape case
            const float frac = static_cast<float>(i) / (num_iter - 1);
            return get_averaged_color(EARLY_ESCAPE_COLOR, LATE_ESCAPE_COLOR, frac);
        }
        z = z * z + c;
    }
    // no escape case
    return NO_ESCAPE_COLOR;
}

sf::Color ColorCalculator::get_averaged_color(const sf::Color c1, const sf::Color c2, const float frac) {
    const sf::Color averaged_color{
        static_cast<sf::Uint8>(c1.r + (c2.r - c1.r) * frac),
        static_cast<sf::Uint8>(c1.g + (c2.g - c1.g) * frac),
        static_cast<sf::Uint8>(c1.b + (c2.b - c1.b) * frac)
    };
    return averaged_color;
}