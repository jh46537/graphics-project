#include "main.h"

int main(int argc, char** argv)
{
    // parameters
    int width  = 1280;
    int height = 720;
    const char* name = "window";

    // initialize window
    if (!glfwInit()) {
        cout << "[failed to initialize glfw]" << endl;
        exit(-1);
    }

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!window) { glfwTerminate(); exit(-1); }

    //glfwSetKeyCallback(window, keyboardCallback);
    glfwMakeContextCurrent(window);

//    // initialize OpenGL
//    if (
//
//    if(gl3wInit()) {
//        std::cerr << "Could not initialize OpenGL\n";
//        exit(1);
//    }
//
//    if(! gl3wIsSupported(4, 1)) {
//        std::cerr << "OpenGL 4.1 not supported\n";
//        exit(1);
//    }
//
//    // Enable depth test
//    glEnable(GL_DEPTH_TEST);
//
//    // Accept fragment if it closer to the camera than the former one
//    glDepthFunc(GL_LESS);
//
//    // Enable blending
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable(GL_DEPTH_TEST);

    while (true) {}

    return 0;
}
