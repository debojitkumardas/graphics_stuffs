#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <ctime>
#include <iostream>

#define HEIGHT 800
#define WIDTH 600

const std::string shaderCode = R"(
    #version 330 core

    uniform vec3 iResolution;
    uniform float iTime;
    uniform sampler2D iChannel0;

    void mainImage(out vec4 I, vec2 u) {

        float M = 0.0f,
              A = 0.0f,
              T = iTime,
              R = 0.0f;
        for(I *= R; R++ < 66.;) {
            vec4 X = iResolution.xyzz, p = A * normalize(vec4((u+u-X.xy) * mat2(cos(A*sin(T*.1)*.3 + vec4(0,33,11,0))), X.y, 0));
            p.z += T;
            p.y = abs(abs(p.y) - 1.);

            X = fract(dot(X=ceil(p*4.), sin(X)) + X);
            X.g += 4.;
            M = 4.*pow(smoothstep(1., .5, texture(iChannel0, (p.xz+ceil(T+X.x))/4.).a), 8.)-5.;

            A += p.y*.6 - (M+A+A+3.)/67.;

            I += (X.a + .5) * (X + A) * ( 1.4 - p.y ) / 2e2 / M / M / exp(A*.1);
        }
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

    sf::RenderWindow window(sf::VideoMode(HEIGHT, WIDTH), "SFML Shader Example", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    // Initialize GLEW
    /*
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return 1;
    }
    */

    // Create shader program
    sf::Shader shader;
    if (!shader.loadFromMemory(shaderCode, sf::Shader::Fragment)) {
        std::cerr << "Failed to load shader" << std::endl;
        return 1;
    }

    // Load texture
    sf::Texture texture;
    if (!texture.loadFromFile("texture.png")) {
        std::cerr << "Failed to load texture" << std::endl;
        return 1;
    }

    // Set texture on the shader
    shader.setUniform("iChannel0", sf::Shader::CurrentTexture);

    // Create a fullscreen quad
    sf::VertexArray quad(sf::Quads, 4);
    quad[0].position = sf::Vector2f(0, 0);
    quad[1].position = sf::Vector2f(HEIGHT, 0);
    quad[2].position = sf::Vector2f(HEIGHT, WIDTH);
    quad[3].position = sf::Vector2f(0, WIDTH);

    //sf::Clock clock;
    //sf::Time elapsedTime = clock.restart();
    float iTime = 100.0f, delta = 0.01f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // float iTime = elapsedTime.asSeconds();
        iTime += delta;

        // std::cout << iTime << std::endl;

        shader.setUniform("iResolution", sf::Vector3f(HEIGHT, WIDTH, 0));
        shader.setUniform("iTime", iTime);

        window.clear();
        window.draw(quad, &shader);
        window.display();

        sf::sleep(sf::microseconds(1));
    }

    return 0;
}
