#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <ctime>
#include <iostream>

const std::string shaderCode = R"(
    #version 330 core

    #define LOW_PERF      1   // set to 1 for better performances

    // spacing controls
    #define spacing       7.  // columns repetition spacing
    #define light_spacing 2.  // light   repetition spacing (try 1. for a psychedelic effect!)

    #define attenuation  22.  // light   attenuation

        // speed controls
    #define GLOBAL_SPEED  .7
    #define camera_speed  1.
    #define lights_speed 30.
    #define columns_speed 4.

    #if LOW_PERF
        #define iterations 30.
        #define max_dist   30.
    #else
        #define iterations 50.
        #define max_dist   80.
    #endif

    #define epsilon 0.005
    #define iTime (iTime*GLOBAL_SPEED)

    #define rot(a) mat2(cos(a), -sin(a), sin(a), cos(a))
    #define rep(p, r) (mod(p+r/2., r)-r/2.)
    #define torus(p) (length( vec2(length(p.xz)-.6,p.y) ) - .06)

    float hash12(vec2 p) {
        vec3 p3  = fract(vec3(p.xyx) * .1031);
        p3 += dot(p3, p3.yzx + 33.33);
        return fract((p3.x + p3.y) * p3.z);
    }

    vec3 getLight(vec3 p, vec3 color) {
        return max(vec3(0.), color / (1. + pow(abs(torus(p) * attenuation), 1.3)) - .001);
    }

    vec3 geo(vec3 po, inout float d, inout vec2 f) {
        // shape repetition
        float r = hash12(floor(po.yz/spacing+vec2(.5)))-.5;
        vec3  p = rep(po + vec3(iTime*r*columns_speed, 0., 0.), vec3(.5, spacing, spacing));
        p.xy   *= rot(1.57);
        d       = min(d, torus(p));

        // light repetition
        f       = floor(po.yz/(spacing*light_spacing)-vec2(.5));
        r       = hash12(f)-.5;
        if (r > -.45) p = rep(po + vec3(iTime*lights_speed*r, 0., 0.), spacing*light_spacing*vec3(r+.54, 1., 1.));
        else p  = rep(po + vec3(iTime*lights_speed*.5*(1.+r*0.003*hash12(floor(po.yz*spacing))), 0., 0.), spacing*light_spacing);
        p.xy   *= rot(1.57);
        f       = (cos(f.xy)*.5+.5)*.4;

        return p;
    }

    vec4 map(vec3 p) {
        float d = 1e6;
        vec3 po, col = vec3(0.);
        vec2 f;

        po = geo(p, d, f);
        col  += getLight(po, vec3(1., f));        // x

        p.z  += spacing/2.;
        p.xy *= rot(1.57);
        po    = geo(p, d, f);
        col  += getLight(po, vec3(f.x, .5, f.y)); // y

        p.xy += spacing/2.;
        p.xz *= rot(1.57);
        po    = geo(p, d, f);
        col  += getLight(po, vec3(f, 1.));        // z

        return vec4(col, d);
    }

    vec3 getOrigin(float t) {
        t = (t+35.)*-.05*camera_speed;
        float rad = mix(50., 80., cos(t*1.24)*.5+.5);
        return vec3(rad*sin(t*.97), rad*cos(t*1.11), rad*sin(t*1.27));
    }

    void initRayOriginAndDirection(vec2 uv, inout vec3 ro, inout vec3 rd) {
        vec2 m = iMouse.xy/iResolution.xy*2.-1.;

        ro = getOrigin(iTime+m.x*10.);

        vec3 f = normalize(getOrigin(iTime+m.x*10.+.5) - ro);
        vec3 r = normalize(cross(normalize(ro), f));
        rd = normalize(f + uv.x*r + uv.y*cross(f, r));
    }

    void mainImage(out vec4 O, in vec2 F) {
        vec2 uv = (2.*F - iResolution.xy)/iResolution.y;
        vec3 p, ro, rd, col;

        initRayOriginAndDirection(uv, ro, rd);

        float t = 2.;
        for (float i = 0.; i < iterations; i++) {
            p = ro + t*rd;

            vec4 res = map(p);
            col += res.rgb;
            t += abs(res.w);

            if (abs(res.w) < epsilon) t += epsilon;

            if (col.r >= 1. && col.g >= 1. && col.b >= 1.) break;
            if (t > max_dist) break;
        }

        col = pow(col, vec3(.45));
        O = vec4(col, 1.0);
    }

    void main() {
        //
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
    quad[1].position = sf::Vector2f(800, 0);
    quad[2].position = sf::Vector2f(800, 600);
    quad[3].position = sf::Vector2f(0, 600);

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

        //std::cout << iTime << std::endl;

        shader.setUniform("iResolution", sf::Vector2f(800, 600));
        shader.setUniform("iTime", iTime);

        window.clear();
        window.draw(quad, &shader);
        window.display();

        sf::sleep(sf::microseconds(100));
    }

    return 0;
}
