#include <iostream>
#include <fstream>
#include <vector>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using glm::vec3;


/*
 * parameters
 */
constexpr uint32_t version_major = 3;
constexpr uint32_t version_minor = 0;
constexpr uint32_t width  = 1000;
constexpr uint32_t height = 1000;
const char* name = "smoke";
constexpr size_t log_size = 1024;


#include "gl.h"
