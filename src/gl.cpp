#include <iostream>
#include <fstream>
#include <vector>

using std::cout;
using std::cerr;
using std::endl;
using std::vector;


#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
//using glm::vec4;
using glm::mat4;
//using glm::translate;
//using glm::scale;


#include "fluid.h"
#include "grid.h"
#include "gl.h"


constexpr size_t log_size = 1024;


/*
 * voxel
 */
Voxel::Voxel()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    vec3 vertices[] = {
          vec3{ -1.0f, -1.0f, -1.0f }, vec3{ 1.0f, 1.0f, 1.0f }
        , vec3{  1.0f, -1.0f, -1.0f }, vec3{ 1.0f, 1.0f, 1.0f }
        , vec3{ -1.0f,  1.0f, -1.0f }, vec3{ 1.0f, 1.0f, 1.0f }
        , vec3{  1.0f,  1.0f, -1.0f }, vec3{ 1.0f, 1.0f, 1.0f }
        , vec3{ -1.0f, -1.0f,  1.0f }, vec3{ 1.0f, 1.0f, 1.0f }
        , vec3{  1.0f, -1.0f,  1.0f }, vec3{ 1.0f, 1.0f, 1.0f }
        , vec3{ -1.0f,  1.0f,  1.0f }, vec3{ 1.0f, 1.0f, 1.0f }
        , vec3{  1.0f,  1.0f,  1.0f }, vec3{ 1.0f, 1.0f, 1.0f }
    };
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint indices[] = {
          0, 1, 2
        , 2, 1, 3

        , 0, 4, 1
        , 1, 4, 5

        , 0, 2, 4
        , 4, 2, 6

        , 1, 5, 3
        , 3, 5, 7

        , 2, 3, 6
        , 6, 3, 7

        , 5, 4, 7
        , 7, 4, 6
    };
    index_size = sizeof(indices) / sizeof(GLuint);
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
}

Voxel::Voxel(Voxel&& that)
{
    vao = that.vao;
    index_size = that.index_size;
    that.vao = 0;
    that.index_size = 0;
}

Voxel::~Voxel()
{
    if (vao) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void Voxel::render() const
{
    //glBindBuffer(GL_ARRAY_BUFFER, vao);

    //glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(0));
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glDrawElements(GL_TRIANGLES, index_size, GL_UNSIGNED_INT, 0);
    //glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/*
 * shader
 */
Shader::Shader()
{
    program = glCreateProgram();
    if (!program) {
        cerr << "[failed to create vertex shader program]" << endl;
        exit(-1);
    }
}

Shader::~Shader()
{
    glUseProgram(GL_NONE);
}

void Shader::add(size_t type, const char* file_name) const
{
    GLint status;
    GLchar info_log[log_size];

    GLuint shader = glCreateShader(type);
    if (!shader) {
        cerr << "[failed to create vertex shader]" << endl;
        exit(-1);
    }

    std::ifstream file {file_name, std::ios::binary | std::ios::ate};
    if (!file.is_open()) {
        cerr << "[failed to open shader file: " << file_name << "]" << endl;
        exit(-1);
    }


    size_t file_size = file.tellg();
    std::string file_data( file_size, '\0' );
    file.seekg(0, std::ios::beg);
    if (!file.read(&file_data[0], file_size)) {
        cerr << "[failed to read shader file: " << file_name << "]" << endl;
        exit(-1);
    }

    const GLchar* data[1] = {file_data.c_str()};
    const GLint   size[1] = {(GLint) file_size};

    glShaderSource(shader, 1, data, size);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(shader, log_size, NULL, info_log);
        cerr << "[failed to compile shader: " << file_name << "]" << endl << info_log;
        exit(-1);
    }

    glAttachShader(program, shader);
}

void Shader::activate() const
{
    GLint status;
    GLchar info_log[log_size];

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        glGetProgramInfoLog(program, log_size * sizeof(GLchar), NULL, info_log);
        cerr << "[failed to link fragment shader program]" << endl << info_log;
        exit(-1);
    }

    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (!status) {
        glGetProgramInfoLog(program, log_size * sizeof(GLchar), NULL, info_log);
        cerr << "[failed to validate fragment shader program]" << endl << info_log;
        exit(-1);
    }

    glUseProgram(program);
}

GLint Shader::uniform(const char* name) const
{
    return glGetUniformLocation(program, name);
}

void Shader::bind_attrib(const vector<const char*>& names) const
{
    for (size_t i = 0; i < names.size(); i++) {
        glBindAttribLocation(program, i, names[i]);
    }
}


/*
 * window
 */
Window::Window(
      size_t version_major
    , size_t version_minor
    , size_t width
    , size_t height
    , const char* name
)
{
    /* initialize window */
    if (!glfwInit()) {
        cerr << "[failed to initialize glfw]" << endl;
        exit(-1);
    }

    ////glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version_minor);

    window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!window) {
        glfwTerminate();
        cerr << "[failed to create window]" << endl;
        exit(-1);
    }

    ////glfwSetKeyCallback(window, keyboardCallback);
    glfwMakeContextCurrent(window);


    /* initialize OpenGL */
    if (gl3wInit()) {
        cerr << "[failed to initialize OpenGL]" << endl;
        exit(-1);
    }

    if (!gl3wIsSupported(version_major, version_minor)) {
        cerr << "[OpenGL " << version_major << "." << version_minor << " not supported]" << endl;
        exit(-1);
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::alive() const
{
    return !glfwWindowShouldClose(window);
}

void Window::render(const Voxel& v, const Fluid& sim, const GLint mvp_loc, const GLint opc_loc, const float max_quantity) const
{
    const Grid& g = sim.getGrid();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (size_t i = 0; i < g.size(); i++) {
        mat4 mvp = g[i].mvp();
        glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));
        float opacity = g[i].quantity() / max_quantity;
        glUniform1f(opc_loc, (GLfloat)opacity);
        v.render();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}
