#pragma once

#include <iostream>
//#include <fstream>
#include <vector>
#include <functional>
#include <chrono>
#include <thread>

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

#include "fluid.h"
#include "gl.h"


/*
 * parameters
 */
constexpr size_t version_major = 3;
constexpr size_t version_minor = 0;

constexpr size_t width  = 800;
constexpr size_t height = 800;
const     char*  name   = "smoke";

constexpr size_t dim_x = 50;
constexpr size_t dim_y = 50;
constexpr size_t dim_z = 1;
constexpr double dx    = 1.0 / 60;

constexpr uint64_t t_unit = 1'000'000'000;
constexpr uint64_t fps    = 60;
constexpr auto t_frame    = std::chrono::duration<int, std::ratio<1, t_unit>>(static_cast<uint64_t>(1.0 / fps * t_unit));
using clk                 = std::chrono::high_resolution_clock;

constexpr double   max_quantity = 1000;
