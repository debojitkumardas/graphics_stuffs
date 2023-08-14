#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <ctime>
#include <iostream>

#define SCREENWIDTH 800
#define SCREENHEIGHT 600

/*
const std::string shaderCode = R"(
    #version 330 core

)";
*/

const std::string shaderCode = R"(
    #version 330 core

    uniform vec2 iResolution;
    uniform float iTime;

    vec3 palette(float t) {

        vec3 a = vec3(0.5, 0.5, 0.5);
        vec3 b = vec3(0.5, 0.5, 0.5);
        vec3 c = vec3(1.0, 1.0, 1.0);
        vec3 d = vec3(0.263, 0.416, 0.557);

        return a + b * cos(6.28318 * (c * t + d));
    }

    void mainImage(out vec4 fragColor, in vec2 fragCoord) {

        vec2 uv = (fragCoord * 2.0 - iResolution.xy) / iResolution.y;
        vec2 uv0 = uv;
        vec3 finalColor = vec3(0.0);

        for (float i = 0.0; i < 4.0; ++i) {
            uv = fract(uv * 1.5) - 0.5;

            float d = length(uv) * exp(-length(uv0));

            vec3 col = palette(length(uv0) + i * 0.4 + iTime * 0.4);

            d = sin(d * 8.0 + iTime) / 8.0;
            d = abs(d);

            d = pow(0.01 / d, 1.2);

            finalColor += col * d;
        }


        fragColor = vec4(finalColor, 1.0);
    }

    void main() {
        vec2 fragCoord = gl_FragCoord.xy;
        mainImage(gl_FragColor, fragCoord);
    }

)";

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    sf::RenderWindow window(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT), "SFML Shader Example", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return 1;
    }

    // Create shader program
    sf::Shader shader;
    if (!shader.loadFromMemory(shaderCode, sf::Shader::Fragment)) {
        std::cerr << "Failed to load shader" << std::endl;
        return 1;
    }

    // Create a fullscreen quad
    sf::VertexArray quad(sf::Quads, 4);
    quad[0].position = sf::Vector2f(0, 0);
    quad[1].position = sf::Vector2f(SCREENWIDTH, 0);
    quad[2].position = sf::Vector2f(SCREENWIDTH, SCREENHEIGHT);
    quad[3].position = sf::Vector2f(0, SCREENHEIGHT);

    //sf::Clock clock;
    //sf::Time elapsedTime = clock.restart();
    float iTime = 0.5f, delta = 0.01f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // float iTime = elapsedTime.asSeconds();
        iTime += delta;

        /*
        if (iTime < 0.0f)
            delta = 0.01f;
        if (iTime > 1.0f)
            delta = -0.01f;
        */

        //std::cout << iTime << std::endl;

        shader.setUniform("iResolution", sf::Vector2f(SCREENWIDTH, SCREENHEIGHT));
        shader.setUniform("iTime", iTime);

        window.clear();
        window.draw(quad, &shader);
        window.display();

        sf::sleep(sf::microseconds(1));
    }

    return 0;
}
