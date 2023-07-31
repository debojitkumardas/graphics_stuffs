#include <SFML/Graphics.hpp>
#include <cstdio>

#define WIDTH  1920
#define HEIGHT 1080

sf::Color linear_interpolation(const sf::Color& v, const sf::Color& u, double a) {
    auto const b = 1 - a;

    return sf::Color(b * v.r + a * u.r, b * v.g + a * u.g, b * v.b + a * u.b);
}

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

    int max_iter = 128;
    double min_re = -2.5, max_re = 1.0;
    double min_im = -1.0, max_im = 1.0;
    double zoom = 1.0;

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
                    zoom_x(5);
                    zoom *= 5;
                }

                // right click zoom-out
                if (event.mouseButton.button == sf::Mouse::Right) {
                    zoom_x(1.0 / 5);
                    zoom /= 5.0;
                }
            }

            // moving around
            if (event.type == sf::Event::KeyPressed) {

                // deltas
                double w = (max_re - min_re) * 0.3;
                double h = (max_im - min_im) * 0.3;

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
        }
        window.clear();

        // #pragma omp parallel for
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                double cr = min_re + (max_re - min_re) * x / WIDTH;
                double ci = min_im + (max_im - min_im) * y / HEIGHT;
                double re = 0, im = 0;
                int iter;

                for (iter = 0; iter < max_iter; ++iter) {
                    double tr = re * re - im * im + cr;

                    im = 2 * re * im + ci;
                    re = tr;

                    if (re * re + im * im > 4)
                        break;
                }

                // int r = 1.0 * (max_iter - iter) / max_iter * 0xff;
                // int g = r, b = r;

                // color pallet similar to wikipedia
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

                image.setPixel(x, y, sf::Color(c));
            }

            texture.loadFromImage(image);
            sprite.setTexture(texture);
            window.draw(sprite);

            char str[100];

            sprintf(str, "max_iter: %d\nzoom: x%2.2lf", max_iter, zoom);
            text.setString(str);
            window.draw(text);
        }
        window.display();
    }

    return 0;
}
