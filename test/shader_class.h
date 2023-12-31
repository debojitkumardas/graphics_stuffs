#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <array>

class Shader {
    public:
    // the program ID
    // --------------
    unsigned int ID;

    // constructor reads and builds the shader
    // ---------------------------------------
    Shader(const char *vertex_path, const char *fragment_path) {
        // 1. retrive the vertex/fragment source code from filepath
        std::string vertex_code;
        std::string fragment_code;
        std::ifstream v_shader_file;
        std::ifstream f_shader_file;

        // ensure ifstream objects can throw exceptions
        v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            // open files
            v_shader_file.open(vertex_path);
            f_shader_file.open(fragment_path);

            std::stringstream v_shader_stream;
            std::stringstream f_shader_stream;

            // read file's buffer contents into streams
            v_shader_stream << v_shader_file.rdbuf();
            f_shader_stream << f_shader_file.rdbuf();

            // close file handlers
            v_shader_file.close();
            f_shader_file.close();

            // convert stream into string
            vertex_code = v_shader_stream.str();
            fragment_code = f_shader_stream.str();
        }
        catch (std::ifstream::failure &e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }

        const char *v_shader_code = vertex_code.c_str();
        const char *f_shader_code = fragment_code.c_str();

        // 2. compile shaders
        int success;
        std::array<char, 512> info_log;

        // vertex shader initialization and compilation
        // --------------------------------------------
        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &v_shader_code, nullptr);
        glCompileShader(vertex);

        // error handling: checking for compilation errors
        // -----------------------------------------------
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (success == 0) {
            glGetShaderInfoLog(vertex, 512, nullptr, info_log.data());
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log.data() << std::endl;
        }

        // fragment shader initialization and compilation
        // ----------------------------------------------
        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &f_shader_code, nullptr);
        glCompileShader(fragment);

        // error handling: checking for compilation errors
        // -----------------------------------------------
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (success == 0) {
            glGetShaderInfoLog(fragment, 512, nullptr, info_log.data());
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log.data() << std::endl;
        }

        // shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        // error handling: checking for linking errors
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (success == 0) {
            glGetProgramInfoLog(ID, 512, nullptr, info_log.data());
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log.data() << std::endl;
        }

        // deleting the shader objects as we've linked them
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // use/activate the shader
    void Use() const {
        glUseProgram(ID);
    }

    // utility uniform functions
    void SetBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
    }

    void SetInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void SetFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
};

#endif
