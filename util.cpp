#include "util.hpp"


CoordRegion::CoordRegion(
    sf::Vector2f bounds_x, sf::Vector2f bounds_y,
    unsigned num_pix_x, unsigned num_pix_y
)
    : bounds_x{bounds_x},
      bounds_y{bounds_y},
      num_pix_x{num_pix_x},
      num_pix_y{num_pix_y} {
}

std::vector<float> CoordRegion::get_pix_coords_y() {
    return _get_pix_coords(bounds_y, num_pix_y);
}

std::vector<float> CoordRegion::get_pix_coords_x() {
    return _get_pix_coords(bounds_x, num_pix_x);
}

std::vector<float> CoordRegion::_get_pix_coords(sf::Vector2f bounds, unsigned num_pix) {
    std::vector<float> coords(num_pix_x);
    const float min = bounds.x;
    const float max = bounds.y;
    const float step = (max - min) / num_pix;
    for (int i = 0; i < num_pix; i++) {
        coords[i] = min + step * i + step / 2.;
    }
    return coords;
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
