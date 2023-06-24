#include "util.hpp"


CoordRegion::CoordRegion(
    const sf::Vector2f& bounds_x, const sf::Vector2f& bounds_y,
    const unsigned num_pix_x, const unsigned num_pix_y
)
    : bounds_x{bounds_x},
      bounds_y{bounds_y},
      num_pix_x{num_pix_x},
      num_pix_y{num_pix_y},
      pix_coords_x(num_pix_x),
      pix_coords_y(num_pix_y) {
}

const std::vector<float>& CoordRegion::get_pix_coords_y() {
    _update_pix_coords(pix_coords_y, bounds_y, num_pix_y);
    return pix_coords_y;
}

const std::vector<float>& CoordRegion::get_pix_coords_x() {
    _update_pix_coords(pix_coords_x, bounds_x, num_pix_x);
    return pix_coords_x;
}

void CoordRegion::_update_pix_coords(
    std::vector<float>& pix_coords,
    const sf::Vector2f& bounds,
    const unsigned num_pix
) {
    const float min = bounds.x;
    const float max = bounds.y;
    const float step = (max - min) / num_pix;
    for (int i = 0; i < num_pix; i++) {
        pix_coords[i] = min + step * i + step / 2.;
    }
}


void print_vec(const auto& v) {
    for (const auto& e: v) {
        std::cout << e << ' ';
    }
    std::cout << '\n';
}

void print_color(const sf::Color& c) {
    for (const float f: {c.r, c.g, c.b}) {
        std::cout << f << ' ';
    }
    std::cout << '\n';
}