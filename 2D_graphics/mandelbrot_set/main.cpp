#include <SFML/Graphics.hpp>
// #include <immintrin.h>

#define WIDTH  1920
#define HEIGHT 1080

int max_iter = 64;
double min_re = -2.5, max_re = 1.0;
double min_im = -1.0, max_im = 1.0;
double zoom = 1.0;

sf::Color linear_interpolation(const sf::Color& v, const sf::Color& u, double a) {
    auto const b = 1 - a;

    return sf::Color(b * v.r + a * u.r, b * v.g + a * u.g, b * v.b + a * u.b);
}

void naive_method(sf::RenderWindow *window, sf::Texture *texture, sf::Sprite *sprite, sf::Text *text, sf::Image *image) {
    int iter;

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            double cr = min_re + (max_re - min_re) * x / WIDTH;
            double ci = min_im + (max_im - min_im) * y / HEIGHT;
            double re = 0, im = 0;

            for (iter = 0; iter < max_iter; ++iter) {
                double tr = re * re - im * im + cr;

                im = 2 * re * im + ci;
                re = tr;

                if (re * re + im * im > 4)
                    break;
            }

            static const std::vector<sf::Color> colors {
                {0, 7, 100},
                {32, 107, 203},
                {237, 255, 255},
                {255, 170, 0},
                {0, 2, 0}
            };

            static const auto max_color = colors.size() - 1;

            if (iter == max_iter)
                iter = 0;

            double mu = 1.0 * iter / max_iter;

            mu *= max_color;
            auto i_mu = static_cast<size_t>(mu);
            auto color1 = colors[i_mu];
            auto color2 = colors[std::min(i_mu + 1, max_color)];
            sf::Color c = linear_interpolation(color1, color2, mu - i_mu);

            image->setPixel(x, y, sf::Color(c));
        }

        texture->loadFromImage(*image);
        sprite->setTexture(*texture);
        window->draw(*sprite);

        char str[100];

        sprintf(str, "max_iter: %d\nzoom: x%2.2lf", max_iter, zoom);
        text->setString(str);
        window->draw(*text);
    }
}

/* Optimizing the code using compiler intrinsics */
/*
void using_comp_int(sf::RenderWindow *window, sf::Texture *texture, sf::Sprite *sprite, sf::Text *text, sf::Image *image) {
    int iter;

    __m256d _a, _b, _two, _four, _mask1;
    __m256d _zr, _zi, _zr2, _cr, _ci;
    __m256d _x_pos_offsets, _x_pos, _x_scale, _jump;
    __m256i _one, _c, _n, _iterations, _mask2;

    _one = _mm256_set1_epi64x(1);
    _two = _mm256_set1_pd(2.0);
    _four = _mm256_set1_pd(4.0);
    _iterations = _mm256_set1_epi64x(iter);

    _x_scale = _mm256_set1_pd(x_scale)
}
*/

int main() {

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set");
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Font font;
    sf::Text text;

    font.loadFromFile("SourceCodePro-Regular.otf");
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    image.create(WIDTH, HEIGHT);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // zooming
            if (event.type == sf::Event::MouseButtonPressed) {
                auto zoom_x = [&](double z) {
                    // mouse point will be center point
                    double cr = min_re + (max_re - min_re) * event.mouseButton.x / WIDTH;
                    double ci = min_im + (max_im - min_im) * event.mouseButton.y / HEIGHT;

                    // zoom
                    double tminr = cr - (max_re - min_re) / 2.0 / z;
                    max_re = cr + (max_re - min_re) / 2 / z;
                    min_re = tminr;

                    double tmini = ci - (max_im - min_im) / 2.0 / z;
                    max_im = ci + (max_im - min_im) / 2 / z;
                    min_im = tmini;
                };

                // left click zoom-in
                if (event.mouseButton.button == sf::Mouse::Left) {
                    zoom_x(2);
                    zoom *= 2;
                }

                // right click zoom-out
                if (event.mouseButton.button == sf::Mouse::Right) {
                    zoom_x(1.0 / 2);
                    zoom /= 2.0;
                }
            }

            // moving around
            if (event.type == sf::Event::KeyPressed) {

                // deltas
                double w = (max_re - min_re) * 0.1;
                double h = (max_im - min_im) * 0.1;

                // move left
                if (event.key.code == sf::Keyboard::Left) {
                    min_re -= w;
                    max_re -= w;
                }

                // move right
                if (event.key.code == sf::Keyboard::Right) {
                    min_re += w;
                    max_re += w;
                }

                // move up
                if (event.key.code == sf::Keyboard::Up) {
                    min_im -= h;
                    max_im -= h;
                }

                // move down
                if (event.key.code == sf::Keyboard::Down) {
                    min_im += h;
                    max_im += h;
                }
            }

            // set iteration levels
            /*
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.MouseWheelScrolled != 0) {
                    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                        if (event.mouseWheelScroll.delta > 0)
                            max_iter *= 2;
                        else
                            max_iter /= 2;

                        if (max_iter < 1)
                            max_iter = 1;
                    }
                }
            }
            */
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::U)
                    max_iter *= 2;
                if (event.key.code == sf::Keyboard::P)
                    max_iter /= 2;
            }
        }

        window.clear();

        naive_method(&window, &texture, &sprite, &text, &image);

        window.display();
    }

    return 0;
}
