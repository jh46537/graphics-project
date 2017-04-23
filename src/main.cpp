#include "main.h"

int main(int argc, char** argv)
{
    /*
     * parameters
     */
    int width  = 1280;
    int height = 720;
    const char* name = "window";
    const char* shader_file_name = "color.frag";
    constexpr size_t log_size = 1024;
    GLchar log[log_size];


    /*
     * initialize window
     */
    if (!glfwInit()) {
        cerr << "[failed to initialize glfw]" << endl;
        exit(-1);
    }

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!window) {
        glfwTerminate();
        cerr << "[failed to create window]" << endl;
        exit(-1);
    }

    //glfwSetKeyCallback(window, keyboardCallback);
    glfwMakeContextCurrent(window);


    /*
     * initialize OpenGL
     */
    if (gl3wInit()) {
        cerr << "[failed to initialize OpenGL]" << endl;
        exit(-1);
    }

    if (!gl3wIsSupported(4, 1)) {
        cerr << "[OpenGL 4.1 not supported]" << endl;
        exit(-1);
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    /*
     * make mesh
     */
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    constexpr float irt3 = 1.0f / sqrt(3);
    vec3 vertices[] = {
          vec3{ 0.0f , -irt3 / 2, -irt3    }
        , vec3{ -0.5f, -irt3 / 2, irt3 / 2 }
        , vec3{ 0.5f , -irt3 / 2, irt3 / 2 }
        , vec3{ 0.0f , irt3     , 0.0f     }
    };
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(vec3), vertices, GL_STATIC_DRAW);

    size_t indices[] = {
          0, 1, 2
        , 0, 2, 3
        , 0, 3, 1
        , 1, 3, 2
    };
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(size_t), indices, GL_STATIC_DRAW);


    /*
     * shader
     */
    GLuint program = glCreateProgram();
    if (!program) {
        cerr << "[failed to create shader program]" << endl;
        exit(-1);
    }

    GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
    if (!shader) {
        cerr << "[failed to create shader type]" << endl;
        exit(-1);
    }

    std::ifstream file {shader_file_name, std::ios::binary | std::ios::ate};
    if (!file.is_open()) {
        cerr << "[failed to open shader file: " << shader_file_name << "]" << endl;
        exit(-1);
    }

    size_t file_size = file.tellg();
    std::string file_data( file_size, '\0' );
    file.seekg(0, std::ios::beg);
    if (!file.read(&file_data[0], file_size)) {
        cerr << "[failed to read shader file: " << shader_file_name << "]" << endl;
        exit(-1);
    }

    const GLchar* shader_data[1] = {file_data.c_str()};
    const GLint   shader_size[1] = {(GLint) file_size};

    glShaderSource(shader, 1, shader_data, shader_size);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(shader, log_size, NULL, log);
        cerr << "[failed to compile shader: " << shader_file_name << "]" << endl << log;
        exit(-1);
    }

    glAttachShader(program, shader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        glGetProgramInfoLog(program, log_size * sizeof(GLchar), NULL, log);
        cerr << "[failed to link shader program]" << endl << log;
        exit(-1);
    }

    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (!status) {
        glGetProgramInfoLog(program, log_size * sizeof(GLchar), NULL, log);
        cerr << "[failed to validate shader program]" << endl << log;
        exit(-1);
    }

    glUseProgram(program);


    /*
     * render loop
     */
    while (!glfwWindowShouldClose(window)) {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(size_t), GL_UNSIGNED_INT, 0);
        glDisableVertexAttribArray(0);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
