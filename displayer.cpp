#include <cmath>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "displayer.hpp"


Displayer::Displayer(
    sf::Vector2f init_bounds_x, sf::Vector2f init_bounds_y,
    unsigned num_pix_x, unsigned num_pix_y,
    float scroll_zoom_factor
)
    : reg{init_bounds_x, init_bounds_y, num_pix_x, num_pix_y},
      scroll_zoom_factor{scroll_zoom_factor},
      color_calculator{},
      window{sf::VideoMode{num_pix_x, num_pix_y}, "Mandelbrot Set", sf::Style::Fullscreen},
      mouse_pressed{false} {

    // enables full-screen to work correctly (not sure exactly why)
    sf::View view{
        sf::FloatRect{
            0, 0, static_cast<float>(num_pix_x), static_cast<float>(num_pix_y),
        }
    };
    window.setView(view);

    // image is updated as pixels change
    // these changes propogate into texture and then sprite
    image.create(reg.get_num_pix_x(), reg.get_num_pix_y());
}

void Displayer::display(bool continuous_update) {
    // main loop
    int i = 0;
    while (window.isOpen()) {
        handle_events();

        // update displayed pixels
        if (i == 0 || continuous_update) {
            update_window();
            std::cout << "Updated window, iteration " << i << "\n";
        }
        i += 1;
    }
}

void Displayer::handle_events() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed ||
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            prev_mouse_pos = sf::Mouse::getPosition(window);
            mouse_pressed = true;
        }
        else if (mouse_pressed && event.type == sf::Event::MouseMoved) {
            cur_mouse_pos = sf::Mouse::getPosition(window);
            const sf::Vector2i mouse_pos_change = cur_mouse_pos - prev_mouse_pos;
            translate_reg(mouse_pos_change);
            prev_mouse_pos = cur_mouse_pos;
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            mouse_pressed = false;
        }
        else if (event.type == sf::Event::MouseWheelScrolled) {
            const float mouse_scroll_delta = event.mouseWheelScroll.delta;
            const sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
            zoom_reg(mouse_scroll_delta);
        }
    }
}

void Displayer::update_window() {
    window.clear();
    update_image();
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    window.draw(sprite);
    window.display();
}

void Displayer::update_image() {
    const auto pix_coords_x = reg.get_pix_coords_x();
    const auto pix_coords_y = reg.get_pix_coords_y();

    #pragma omp parallel for
    for (int i = 0; i < reg.get_num_pix_x(); i++) {
        for (int j = 0; j < reg.get_num_pix_y(); j++) {
            const auto x = pix_coords_x[i];
            const auto y = pix_coords_y[j];
            const auto color = color_calculator.get_color(x, y);
            image.setPixel(i, j, color);
        }
    }
}

void Displayer::translate_reg(const sf::Vector2i mouse_pos_change) {
    const float d_bounds_x = (static_cast<float>(mouse_pos_change.x) / window.getSize().x) * reg.get_width();
    const float d_bounds_y = (static_cast<float>(mouse_pos_change.y) / window.getSize().y) * reg.get_height();
    reg.set_bounds_x(reg.get_bounds_x() - sf::Vector2f{d_bounds_x, d_bounds_x});
    reg.set_bounds_y(reg.get_bounds_y() - sf::Vector2f{d_bounds_y, d_bounds_y});
}

void Displayer::zoom_reg(const float mouse_scroll_delta) {
    const auto [min_x, max_x] = reg.get_bounds_x();
    const auto [min_y, max_y] = reg.get_bounds_y();

    const float orig_x = (min_x + max_x) / 2;
    const float orig_y = (min_y + max_y) / 2;
    
    float zoom = std::pow(scroll_zoom_factor, -mouse_scroll_delta);
    const float new_min_x = orig_x + (min_x - orig_x) * zoom;
    const float new_max_x = orig_x + (max_x - orig_x) * zoom;
    const float new_min_y = orig_y + (min_y - orig_y) * zoom;
    const float new_max_y = orig_y + (max_y - orig_y) * zoom;

    reg.set_bounds_x({new_min_x, new_max_x});
    reg.set_bounds_y({new_min_y, new_max_y});
}