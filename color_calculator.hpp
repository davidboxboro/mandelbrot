#pragma once

#include "constants.hpp"
#include <array>
#include <complex>
#include <iostream>
#include <xmmintrin.h>
#include <pmmintrin.h>
#include <immintrin.h>


class ColorCalculator {
public:
    inline ColorCalculator(unsigned num_iter = NUM_ITER)
        : num_iter{num_iter}, idx_to_color(num_iter + 1) {
        for (int i = 0; i < num_iter; i++) {
            idx_to_color[i] = get_averaged_color(
                EARLY_ESCAPE_COLOR,
                LATE_ESCAPE_COLOR,
                static_cast<float>(i) / (num_iter - 1)
            );
        }
        idx_to_color[num_iter] = NO_ESCAPE_COLOR;
    }

    inline sf::Color get_color(const float x, const float y) const {
        // z = z1 + z2 * i
        // c = x + y * i
        float z1 = 0;
        float z2 = 0;
        float z1_tmp = 0;
        for (int i = 0; i < num_iter; ++i) {
            if (z1 * z1 + z2 * z2 > 4) {
                // escape case
                return idx_to_color[i];
            }
            z1_tmp = z1 * z1 - z2 * z2 + x;
            z2 = (z1 + z1) * z2 + y;
            z1 = z1_tmp;
        }
        // no escape case
        return NO_ESCAPE_COLOR;
    }

    // this uses std::complex, which is slower than raw computation with floats,
    // as used in get_color()
    inline sf::Color get_color2(const float x, const float y) const {
        // z = z1 + z2 * i
        // c = x + y * i
        const std::complex<float> c{x, y};
        std::complex<float> z{0, 0};
        for (int i = 0; i < num_iter; ++i) {
            if (z.real() * z.real() + z.imag() * z.imag() > 4) {
                // escape case
                return idx_to_color[i];
            }
            z = z * z + c;
        }
        // no escape case
        return NO_ESCAPE_COLOR;
    }

    // mostly written by chat gpt
    template<unsigned N = SIMD_MULTIPLIER>
    inline std::array<const sf::Color*, N> get_color_simd(const float* x_ptr, const float y) const {
        __m256 x_values = _mm256_loadu_ps(x_ptr);
        __m256 y_values = _mm256_set1_ps(y);

        __m256 z1 = _mm256_setzero_ps();
        __m256 z2 = _mm256_setzero_ps();

        std::array<int, N> escape_idxs;
        escape_idxs.fill(num_iter);

        for (int i = 0; i < num_iter; ++i) {
            __m256 z1_sq = _mm256_mul_ps(z1, z1);
            __m256 z2_sq = _mm256_mul_ps(z2, z2);
            __m256 escape_mask = _mm256_cmp_ps(z1_sq + z2_sq, _mm256_set1_ps(4), _CMP_GT_OQ);
            unsigned escape_mask_int = _mm256_movemask_ps(escape_mask);

            if (escape_mask_int) {
                // escape case
                for (int k = 0; k < N; ++k) {
                    if ((escape_mask_int & (1 << k)) && (escape_idxs[k] == num_iter)) {
                        escape_idxs[k] = i;
                    }
                }
            }

            z2 = _mm256_add_ps(_mm256_mul_ps(_mm256_add_ps(z1, z1), z2), y_values);
            z1 = _mm256_add_ps(_mm256_sub_ps(z1_sq, z2_sq), x_values);
        }

        // we return pointers because copying sf::Color a few million times
        // is expensive
        std::array<const sf::Color*, N> color_ptrs;
        for (int k = 0; k < N; ++k) {
            color_ptrs[k] = &idx_to_color[escape_idxs[k]];
        }
        return color_ptrs;
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
    std::vector<sf::Color> idx_to_color;
};