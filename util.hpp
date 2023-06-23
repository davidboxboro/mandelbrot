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

    std::vector<float> get_pix_coords_y() const;
    std::vector<float> get_pix_coords_x() const;
    std::vector<float> _get_pix_coords(sf::Vector2f bounds, unsigned num_pix) const;

    inline sf::Vector2f get_bounds_x() const { return bounds_x; }
    inline sf::Vector2f get_bounds_y() const { return bounds_y; }
    inline void set_bounds_x(sf::Vector2f new_bounds_x) {
        bounds_x = new_bounds_x;
    }
    inline void set_bounds_y(sf::Vector2f new_bounds_y) {
        bounds_y = new_bounds_y;
    }

    inline unsigned get_num_pix_x() const { return num_pix_x; }
    inline unsigned get_num_pix_y() const { return num_pix_y; }

    inline float get_pix_width() const {
        return (bounds_x.y - bounds_x.x) / num_pix_x;
    }
    inline float get_pix_height() const {
        return (bounds_y.y - bounds_y.x) / num_pix_y;
    }
    inline float get_width() const {
        return bounds_x.y - bounds_x.x;
    }
    inline float get_height() const {
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