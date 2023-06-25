#include <cmath>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "displayer.hpp"


Displayer::Displayer(
    const sf::Vector2f& init_bounds_x,
    const unsigned num_pix_x,
    const float scroll_zoom_factor,
    const float keyboard_scroll_delta
)
    :
      color_calculator{},
      scroll_zoom_factor{scroll_zoom_factor},
      keyboard_scroll_delta{keyboard_scroll_delta},
      desktop_width{sf::VideoMode::getDesktopMode().width},
      desktop_height{sf::VideoMode::getDesktopMode().height},
      reg{init_bounds_x, compute_bounds_y(init_bounds_x), num_pix_x, compute_num_pix_y(num_pix_x)},
      window{sf::VideoMode{desktop_width, desktop_height}, "Mandelbrot Set", sf::Style::Fullscreen},
      mouse_pressed{false} {

    // image is updated as pixels change
    // these changes propogate into texture and then sprite
    image.create(reg.get_num_pix_x(), reg.get_num_pix_y());
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    // the number of pixels in the computed image is usually smaller than the desktop,
    // so we resize the sprite (containing the image) to fill the screen
    const auto x_ratio = static_cast<float>(desktop_width) / reg.get_num_pix_x();
    const auto y_ratio = static_cast<float>(desktop_height) / reg.get_num_pix_y();
    sprite.setScale(x_ratio, y_ratio);
}

sf::Vector2f Displayer::compute_bounds_y(const sf::Vector2f& bounds_x) {
    const float init_max_y = 0.5f * (bounds_x.y - bounds_x.x) * desktop_height / desktop_width;
    return {-init_max_y, init_max_y};
}

unsigned Displayer::compute_num_pix_y(const unsigned num_pix_x) {
    const unsigned num_pix_y = num_pix_x * desktop_height / desktop_width;
    return num_pix_y;
}

void Displayer::display(const bool continuous_update) {
    // main loop
    int i = 0;
    std::vector<float> time_elapsed_vec;
    while (window.isOpen()) {
        handle_events();
        // update displayed pixels
        if (i == 0 || continuous_update) {
            const auto start = std::chrono::high_resolution_clock::now();
            update_window();
            const auto end = std::chrono::high_resolution_clock::now();
            const float time_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.;
            // std::cout << "Iteration " << i << ": " << time_elapsed << " ms\n";

            // compute mean iteration time for testing purposes
            if (i >= 1) {
                // first few iterations take much longer and have higher variance,
                // so they throw off the mean estimate
                time_elapsed_vec.push_back(time_elapsed);
            }
            const unsigned targ_num_iter = 50;
            if (time_elapsed_vec.size() == targ_num_iter) {
                const auto mean_time_elapsed = std::reduce(time_elapsed_vec.begin(), time_elapsed_vec.end()) / targ_num_iter;
                std::cout << "First " << targ_num_iter << " iterations: " << mean_time_elapsed << " ms\n";
                // std::exit(0);
            }
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
            cur_mouse_pos = {event.mouseMove.x, event.mouseMove.y};
            const sf::Vector2i mouse_pos_change = cur_mouse_pos - prev_mouse_pos;
            translate_reg(mouse_pos_change);
            prev_mouse_pos = cur_mouse_pos;
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            mouse_pressed = false;
        }
        else if (event.type == sf::Event::MouseWheelScrolled) {
            const float mouse_scroll_delta = event.mouseWheelScroll.delta;
            zoom_reg(mouse_scroll_delta);
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::J) {
            const float mouse_scroll_delta = event.mouseWheelScroll.delta;
            zoom_reg(keyboard_scroll_delta);
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::K) {
            const float mouse_scroll_delta = event.mouseWheelScroll.delta;
            zoom_reg(-keyboard_scroll_delta);
        }
    }
}

void Displayer::update_window() {
    window.clear();
    update_image_simd();
    texture.loadFromImage(image);
    window.draw(sprite);
    window.display();
}

void Displayer::update_image_simd() {
    const auto& pix_coords_x = reg.get_pix_coords_x();
    const auto& pix_coords_y = reg.get_pix_coords_y();

    const auto num_pix_x = reg.get_num_pix_x();
    const auto num_pix_y = reg.get_num_pix_y();

    #pragma omp parallel for
    for (int j = 0; j < num_pix_y; ++j) {
        for (int i = 0; i < num_pix_x; i += SIMD_MULTIPLIER) {
            const auto x_ptr = &pix_coords_x[i];
            const auto y = pix_coords_y[j];
            const auto color_ptrs = color_calculator.get_color_simd(x_ptr, y);
            for (int k = 0; k < SIMD_MULTIPLIER; ++k) {
                image.setPixel(i + k, j, *color_ptrs[k]);
            }
        }
    }
}

void Displayer::update_image() {
    const auto& pix_coords_x = reg.get_pix_coords_x();
    const auto& pix_coords_y = reg.get_pix_coords_y();

    const auto num_pix_x = reg.get_num_pix_x();
    const auto num_pix_y = reg.get_num_pix_y();

    #pragma omp parallel for
    for (int j = 0; j < num_pix_y; ++j) {
        for (int i = 0; i < num_pix_x; ++i) {
            const auto x = pix_coords_x[i];
            const auto y = pix_coords_y[j];
            const auto color = color_calculator.get_color(x, y);
            image.setPixel(i, j, color);
        }
    }
}

void Displayer::translate_reg(const sf::Vector2i& mouse_pos_change) {
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