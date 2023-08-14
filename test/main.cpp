#include "shader_class.h"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <cmath>

#define SCREENWIDTH 800
#define SCREENHEIGHT 600

/** glfw: whenever the window size changes (by os or user resize) this callback function executes */
// -------------------------------------------------------------------------------------------------
void FramebufferSizeCallback(GLFWwindow*  /*window*/, int width, int height) {
    // set the left and right corner of the window
    // -------------------------------------------
    glViewport(0, 0, width, height);
}

/** process all input: query GLFW wheter relevant keys are pressed/released this frame and react accordingly */
// ------------------------------------------------------------------------------------------------------------
void ProcessInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

/*
const std::string kVertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    out vec3 vertex_color;

    void main() {
        gl_Position = vec4(aPos, 1.0F);
        vertex_color = aColor; // vec4(0.5, 0.0, 0.0, 1.0);
    }
)";

const std::string kFragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    in vec3 vertex_color;

    // uniform vec4 vertex_color;

    void main() {
        FragColor = vec4(vertex_color, 1.0); // vec4(1.0F, 0.5F, 0.2F, 1.0F);
    }
)";
*/

auto main() -> int {

    // initialize glfw
    // ---------------
    if (glfwInit() == 0) {
        std::cerr << "Couldn't initialize GLFW. Exiting!!" << std::endl;
        return -1;
    }

    // set opengl version and profile
    // ------------------------------
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "Cool Shaders", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window. Exiting!!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // make the context of window the main context on the current thread
    // -----------------------------------------------------------------
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    // initialize glew
    // ---------------
    if (glewInit() != GLEW_OK) {
        std::cerr << "Couldn't initialize GLEW. Exiting!!" << std::endl;
        return -1;
    }

    Shader new_shader("vertex.shader", "fragment.shader");

    /*
    // to log errors
    // -------------
    int success;
    std::array<char, 512> info_log;

    // vertex shader initialization and compilation
    // --------------------------------------------
    const char *vertex_shader_source = kVertexShaderSource.c_str();
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);

    // error handling: checking for compilation errors
    // -----------------------------------------------
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log.data());
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log.data() << std::endl;
    }

    // fragment shader initialization and compilation
    // ----------------------------------------------
    const char *fragment_shader_source = kFragmentShaderSource.c_str();
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);

    // error handling: checking for compilation errors
    // -----------------------------------------------
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log.data());
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log.data() << std::endl;
    }

    // linking shaders
    // ---------------
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    // error handling: checking for linking errors
    // -------------------------------------------
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (success == 0) {
        glGetProgramInfoLog(shader_program, 512, nullptr, info_log.data());
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log.data() << std::endl;
    }

    // deleting the shader objects as we've linked them
    // ------------------------------------------------
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    */

    // set up vertex data (and buffers) and configure vertex attributes
    // ----------------------------------------------------------------
    std::array<float, 24> vertices = {
         0.5F,  0.5F, 0.0F,  1.0F, 0.0F, 0.0F,
         0.5F, -0.5F, 0.0F,  0.0F, 1.0F, 0.0F,
        -0.5F, -0.5F, 0.0F,  0.0F, 0.0F, 1.0F,
        -0.5F,  0.5F, 0.0F,  1.0F, 1.0F, 0.0F
    };

    std::array<unsigned int, 6> indices = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

    // generate vao, vbo and ebo
    // -------------------------
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // bind the vertex array object first, then bind and set vertex buffers, and then configure vertex attributes
    // ----------------------------------------------------------------------------------------------------------
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // it's allowed but not necessary: new to understand it more clearly
    // -----------------------------------------------------------------
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // it's allowed but not necessary: new to understand it more clearly
    // -----------------------------------------------------------------
    glBindVertexArray(0);

    // to draw the wireframe (GL_FILL is the default)
    // ----------------------------------------------
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // float time_value;
    // float green_value;
    // float red_value;
    // int vertex_color_location = glGetUniformLocation(shader_program, "vertex_color");
    // render loop
    // -----------
    while (glfwWindowShouldClose(window) == 0) {
        // input
        // -----
        ProcessInput(window);

        // render
        // ------
        glClearColor(0.2F, 0.3F, 0.3F, 0.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw the triangle
        // -----------------
        // glUseProgram(shader_program);
        new_shader.Use();

        // time_value = static_cast<float>(glfwGetTime());
        // green_value = (std::sin(time_value) / 2.0F) + 0.5F;
        // red_value = (std::cos(time_value) / 2.0F) + 0.5F;
        // glUniform4f(vertex_color_location, red_value, green_value, 0.0F, 1.0F);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        // glfw: swap buffers and poll IO events
        // -------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /*
    int n_attr;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n_attr);
    std::cout << "Max attribs: " << n_attr << std::endl;
    */

    // de-allocate all resources once they've outlived their purpose
    // -------------------------------------------------------------
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    // glDeleteProgram(shader_program);

    // glfw: terminate, clearing all previously allocated GLFW resources
    // -----------------------------------------------------------------
    glfwTerminate();

    return 0;
}
