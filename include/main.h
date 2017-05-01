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
using glm::mat4;
using glm::translate;


/*
 * parameters
 */
const char* name = "smoke";
constexpr uint32_t version_major = 3;
constexpr uint32_t version_minor = 0;
constexpr uint32_t width  = 1000;
constexpr uint32_t height = 1000;
constexpr size_t log_size = 1024;


#include "gl.h"
