#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>

const std::string shaderCode = R"(
    #version 330 core

    uniform vec2 iResolution;
    uniform float iTime;

    #define r(a) mat2(cos(a + vec4(0,33,11,0)))

    #define s(p) ( q = p,                                    \
        d = length(vec2(length(q.xy += .5)-.5, q.z)) - .01,  \
        q.yx *= r(round((atan(q.y,q.x)-T) * 3.8) / 3.8 + T), \
        q.x -= .5,                                           \
        O += (sin(t+T)*.1+.1)*(1.+cos(t+T*.5+vec4(0,1,2,0))) \
             / (.5 + pow(length(q)*50., 1.3))            , d ) // return d

    void mainImage(out vec4 O, vec2 F) {
        vec3  p, q,    R = vec3(iResolution.x, iResolution.y, 0.0);
        float i, t, d, T = iTime;

        for (O *= i, F += F - R.xy; i++ < 28.;          // raymarch for 28 iterations
            p = t * normalize(vec3(F*r(t*.1), R.y)),    // ray position
            p.zx *= r(T/4.), p.zy *= r(T/3.), p.x += T, // camera movement
            t += min(min(s( p = fract(p) - .5 ),        // distance to torus + color (x3)
                         s( vec3(-p.y, p.zx)  )),
                         s( -p.zxy            ))
        );
    }

    void main() {
        vec2 fragCoord = gl_FragCoord.xy;
        fragCoord.y = iResolution.y - fragCoord.y; // Flip Y coordinate
        fragCoord.x *= iResolution.x / iResolution.y; // Adjust X coordinate for aspect ratio

        vec4 fragColor;
        mainImage(fragColor, fragCoord);
        gl_FragColor = fragColor;
    }
)";

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Shader Example", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    // Create shader program
    sf::Shader shader;
    if (!shader.loadFromMemory(shaderCode, sf::Shader::Fragment)) {
        std::cerr << "Failed to load shader" << std::endl;
        return 1;
    }

    float iTime = 100.0f, delta = 0.01f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        iTime += delta;

        shader.setUniform("iResolution", sf::Vector2f(800, 600));
        shader.setUniform("iTime", iTime);

        window.clear();
        sf::RectangleShape quad(sf::Vector2f(800, 600));
        quad.setFillColor(sf::Color::White);
        window.draw(quad, &shader);
        window.display();

        sf::sleep(sf::microseconds(100));
    }

    return 0;
}
