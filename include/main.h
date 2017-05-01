#pragma once

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


#include "grid.h"
#include "gl.h"


/*
 * parameters
 */
constexpr size_t version_major = 3;
constexpr size_t version_minor = 0;

constexpr size_t width  = 800;
constexpr size_t height = 800;
const     char*  name   = "smoke";

constexpr size_t dim_x  = 50;
constexpr size_t dim_y  = 50;
constexpr size_t dim_z  = 1;
constexpr double delta  = 1.0/60;
