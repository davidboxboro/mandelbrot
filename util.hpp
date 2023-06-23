#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>


class CoordRegion {
public:
    CoordRegion(
        sf::Vector2f bounds_x, sf::Vector2f bounds_y,
        unsigned num_pix_x, unsigned num_pix_y
    );

    std::vector<float> get_pix_coords_y();
    std::vector<float> get_pix_coords_x();
    std::vector<float> _get_pix_coords(sf::Vector2f bounds, unsigned num_pix);

    inline sf::Vector2f get_bounds_x() { return bounds_x; }
    inline sf::Vector2f get_bounds_y() { return bounds_y; }
    inline void set_bounds_x(sf::Vector2f new_bounds_x) {
        bounds_x = new_bounds_x;
    }
    inline void set_bounds_y(sf::Vector2f new_bounds_y) {
        bounds_y = new_bounds_y;
    }

    inline unsigned get_num_pix_x() { return num_pix_x; }
    inline unsigned get_num_pix_y() { return num_pix_y; }

    inline float get_pix_width() {
        return (bounds_x.y - bounds_x.x) / num_pix_x;
    }
    inline float get_pix_height() {
        return (bounds_y.y - bounds_y.x) / num_pix_y;
    }
    inline float get_width() {
        return bounds_x.y - bounds_x.x;
    }
    inline float get_height() {
        return bounds_y.y - bounds_y.x;
    }

private:
    sf::Vector2f bounds_x;
    sf::Vector2f bounds_y;

    const unsigned num_pix_x;
    const unsigned num_pix_y;
};


void print_vec(const auto& v);

void print_color(const sf::Color& c);