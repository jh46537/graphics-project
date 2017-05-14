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
using std::function;
using std::max;


#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using glm::vec3;
using glm::uvec3;
using glm::vec4;
using glm::mat4;


#include "grid.h"
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

constexpr float speed_r   = 0.10f;
constexpr float speed_t   = 0.01f;
constexpr float speed_p   = 0.01f;
constexpr float speed_div = 0.10f;

constexpr size_t N     = 100;
constexpr size_t dim_x = N;
constexpr size_t dim_y = N;
constexpr size_t dim_z = 1;
constexpr float  dx    = 1.0 / N;

constexpr float fov    = M_PI / 32;

using clk                  = std::chrono::system_clock;
constexpr uint64_t fps     = 30;
constexpr uint64_t t_unit  = clk::period::den / clk::period::num;
constexpr auto     t_frame = clk::duration(static_cast<uint64_t>((1.0 / fps) * t_unit));

constexpr float max_quantity = 1000;
