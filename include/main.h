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
using std::max;


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

constexpr size_t dim_x = 100;
constexpr size_t dim_y = 100;
constexpr size_t dim_z = 5;
constexpr float dx     = 1.0 / 100;

using clk                  = std::chrono::system_clock;
constexpr uint64_t fps     = 3;
constexpr uint64_t t_unit  = clk::period::den / clk::period::num;
constexpr auto     t_frame = clk::duration(static_cast<uint64_t>((1.0 / fps) * t_unit));

constexpr float max_quantity = 1000;
