#pragma once

#include "constants.hpp"
#include <complex>

class ColorCalculator {
public:
    inline ColorCalculator(unsigned num_iter = NUM_ITER)
        : num_iter{num_iter} {
    }

    inline sf::Color get_color(const float x, const float y) const {
        const std::complex<float> c{x, y};
        std::complex<float> z{0, 0};
        for (int i = 0; i < num_iter; i++) {
            if (z.real()*z.real() + z.imag()*z.imag() > 4) {
                // escape case
                const float frac = static_cast<float>(i) / (num_iter - 1);
                return get_averaged_color(EARLY_ESCAPE_COLOR, LATE_ESCAPE_COLOR, frac);
            }
            z = z * z + c;
        }
        // no escape case
        return NO_ESCAPE_COLOR;
    }
    
    static inline sf::Color get_averaged_color(const sf::Color& c1, const sf::Color& c2, const float frac) {
        return {
            static_cast<sf::Uint8>(c1.r + (c2.r - c1.r) * frac),
            static_cast<sf::Uint8>(c1.g + (c2.g - c1.g) * frac),
            static_cast<sf::Uint8>(c1.b + (c2.b - c1.b) * frac)
        };
    }

private:
    const unsigned num_iter;
};