#include "../hdr/shader_class.h"
#include <GLFW/glfw3.h>
#include <array>

#define SCREENWIDTH 1920
#define SCREENHEIGHT 1080

void FramebufferSizeCallback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

auto main() -> int {

    if (glfwInit() == 0) {
        std::cerr << "Couldn't initialize GLFW. Exiting!!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "Test fragment shader", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window. Exiting!!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Couldn't initialize GLEW. Exiting!!" << std::endl;
        return -1;
    }

    Shader new_shader("shaders/vertex.shader", "shaders/fragment.shader");

    std::array<float, 12> quad_vertices = {
        -1.0F, -1.0F, 0.0F,
         1.0F, -1.0F, 0.0F,
        -1.0F,  1.0F, 0.0F,
         1.0F,  1.0F, 0.0F,
    };

    unsigned int vao;
    unsigned int vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float time;
    int res_loc = new_shader.GetLocation("resolution");
    int time_loc = new_shader.GetLocation("time");

    while (glfwWindowShouldClose(window) == 0) {
        ProcessInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        new_shader.Use();

        time = static_cast<float>(glfwGetTime());

        glUniform2f(res_loc, SCREENWIDTH, SCREENHEIGHT);
        glUniform1f(time_loc, time);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    new_shader.DeleteProgram();

    return 0;
}
