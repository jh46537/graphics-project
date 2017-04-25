#include "main.h"

int main(int argc, char** argv)
{
    /*
     * parameters
     */
    int width  = 1280;
    int height = 720;
    const char* name = "window";
    const char* vert_shader_file_name = "shader.vert";
    const char* frag_shader_file_name = "shader.frag";
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

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


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
    GLint status;


    GLuint vert_program = glCreateProgram();
    if (!vert_program) {
        cerr << "[failed to create vertex shader program]" << endl;
        exit(-1);
    }

    GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
    if (!vert_shader) {
        cerr << "[failed to create vertex shader]" << endl;
        exit(-1);
    }

    std::ifstream vert_file {vert_shader_file_name, std::ios::binary | std::ios::ate};
    if (!vert_file.is_open()) {
        cerr << "[failed to open shader file: " << vert_shader_file_name << "]" << endl;
        exit(-1);
    }

    size_t vert_file_size = vert_file.tellg();
    std::string vert_file_data( vert_file_size, '\0' );
    vert_file.seekg(0, std::ios::beg);
    if (!vert_file.read(&vert_file_data[0], vert_file_size)) {
        cerr << "[failed to read shader file: " << vert_shader_file_name << "]" << endl;
        exit(-1);
    }

    const GLchar* vert_shader_data[1] = {vert_file_data.c_str()};
    const GLint   vert_shader_size[1] = {(GLint) vert_file_size};

    glShaderSource(vert_shader, 1, vert_shader_data, vert_shader_size);
    glCompileShader(vert_shader);

    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(vert_shader, log_size, NULL, log);
        cerr << "[failed to compile shader: " << vert_shader_file_name << "]" << endl << log;
        exit(-1);
    }

    glAttachShader(vert_program, vert_shader);

    glLinkProgram(vert_program);
    glGetProgramiv(vert_program, GL_LINK_STATUS, &status);
    if (!status) {
        glGetProgramInfoLog(vert_program, log_size * sizeof(GLchar), NULL, log);
        cerr << "[failed to link vertex shader program]" << endl << log;
        exit(-1);
    }

    glValidateProgram(vert_program);
    glGetProgramiv(vert_program, GL_VALIDATE_STATUS, &status);
    if (!status) {
        glGetProgramInfoLog(vert_program, log_size * sizeof(GLchar), NULL, log);
        cerr << "[failed to validate vertex shader program]" << endl << log;
        exit(-1);
    }

    glUseProgram(vert_program);


    GLuint frag_program = glCreateProgram();
    if (!frag_program) {
        cerr << "[failed to create fragment shader program]" << endl;
        exit(-1);
    }

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    if (!frag_shader) {
        cerr << "[failed to create fragment shader]" << endl;
        exit(-1);
    }

    std::ifstream frag_file {frag_shader_file_name, std::ios::binary | std::ios::ate};
    if (!frag_file.is_open()) {
        cerr << "[failed to open shader file: " << frag_shader_file_name << "]" << endl;
        exit(-1);
    }

    size_t frag_file_size = frag_file.tellg();
    std::string frag_file_data( frag_file_size, '\0' );
    frag_file.seekg(0, std::ios::beg);
    if (!frag_file.read(&frag_file_data[0], frag_file_size)) {
        cerr << "[failed to read shader file: " << frag_shader_file_name << "]" << endl;
        exit(-1);
    }

    const GLchar* frag_shader_data[1] = {frag_file_data.c_str()};
    const GLint   frag_shader_size[1] = {(GLint) frag_file_size};

    glShaderSource(frag_shader, 1, frag_shader_data, frag_shader_size);
    glCompileShader(frag_shader);

    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(frag_shader, log_size, NULL, log);
        cerr << "[failed to compile shader: " << frag_shader_file_name << "]" << endl << log;
        exit(-1);
    }

    glAttachShader(frag_program, frag_shader);

    glLinkProgram(frag_program);
    glGetProgramiv(frag_program, GL_LINK_STATUS, &status);
    if (!status) {
        glGetProgramInfoLog(frag_program, log_size * sizeof(GLchar), NULL, log);
        cerr << "[failed to link fragment shader program]" << endl << log;
        exit(-1);
    }

    glValidateProgram(frag_program);
    glGetProgramiv(frag_program, GL_VALIDATE_STATUS, &status);
    if (!status) {
        glGetProgramInfoLog(frag_program, log_size * sizeof(GLchar), NULL, log);
        cerr << "[failed to validate fragment shader program]" << endl << log;
        exit(-1);
    }

    glUseProgram(frag_program);


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
