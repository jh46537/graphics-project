#include "main.h"

int main(int argc, char** argv)
{
    /*
     * parameters
     */
    int width  = 1280;
    int height = 720;
    const char* name = "window";


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
     * program
     */
    while (true);

    return 0;
}
