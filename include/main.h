#include <iostream>
//#include <fstream>
#include <vector>

using std::cout;
using std::cerr;
using std::endl;
using std::vector;


#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
//#define GLM_FORCE_RADIANS
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
//using glm::translate;


#include "gl.h"
#include "fluid.h"


/*
 * parameters
 */
const char* name = "smoke";
constexpr size_t version_major = 3;
constexpr size_t version_minor = 0;
constexpr size_t width  = 1000;
constexpr size_t height = 1000;
