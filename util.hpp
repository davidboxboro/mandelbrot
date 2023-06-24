#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>


class CoordRegion {
public:
    CoordRegion(
        const sf::Vector2f& bounds_x, const sf::Vector2f& bounds_y,
        const unsigned num_pix_x, const unsigned num_pix_y
    );

    const std::vector<float>& get_pix_coords_y(); // return a reference to avoid copying
    const std::vector<float>& get_pix_coords_x();
    void _update_pix_coords(
        std::vector<float>& pix_coords,
        const sf::Vector2f& bounds,
        const unsigned num_pix
    );

    inline sf::Vector2f get_bounds_x() const { return bounds_x; }
    inline sf::Vector2f get_bounds_y() const { return bounds_y; }
    inline void set_bounds_x(const sf::Vector2f& new_bounds_x) {
        bounds_x = new_bounds_x;
    }
    inline void set_bounds_y(const sf::Vector2f& new_bounds_y) {
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

    std::vector<float> pix_coords_x;
    std::vector<float> pix_coords_y;
};


void print_vec(const auto& v);

void print_color(const sf::Color& c);