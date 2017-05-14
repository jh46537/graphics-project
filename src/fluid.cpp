#include <iostream>
#include <vector>
#include <functional>

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::function;


#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>


using glm::vec3;
using glm::uvec3;
using glm::mat4;

#include "grid.h"
#include "fluid.h"
#include "gl.h"

constexpr float max_quantity = 1000.0f;

/*
 * Navier–Stokes
 */
Fluid::Fluid(const uvec3& dim, const float dx, function<void (Grid&)> setup)
{
    curGrid     = new Grid(dim, dx, setup);
    workingGrid = new Grid{ *curGrid };
    curGrid->id     = 1;
    workingGrid->id = 2;

    const size_t X = dim.x;
    const size_t Y = dim.y;
    const size_t Z = dim.z;
    p = field<float>(X, vector<vector<float>>(Y, vector<float>(Z, 0.0f)));
    q = p;
    omega = field<vec3>(X, vector<vector<vec3>>(Y, vector<vec3>(Z)));
    nu = omega;
    psi = omega;
}


Fluid::~Fluid()
{
    delete(curGrid);
    delete(workingGrid);
}

const Grid& Fluid::getGrid() const
{
    return *curGrid;
}

Grid& Fluid::getGrid()
{
    return *curGrid;
}

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::function;
using std::max;
static float frand(float min = 0.0, float max = 1.0)
{
  return float(rand())/float(RAND_MAX) * (max - min) + min;
}
void Fluid::step(const float dt)
{
    advect(dt);
    swap();
    forces(dt);
    if (Window::mesh)
      mesh(dt);
    project(dt);
    swap();

    size_t X = (*curGrid).xDim();
    size_t Y = (*curGrid).yDim();
    size_t Z = (*curGrid).zDim();
    // Create conditions for nice looking simulation =)
    for (size_t i = 0; i < X; ++i) {
        for (size_t j = 0; j < Y; ++j) {
            vec3 pos = vec3(i, j, 0) - (*curGrid)(i, j, 0).V * dt;
            (*workingGrid)(i, j, 0).Q = (*curGrid).bilerp(pos).Q;
            (*workingGrid)(i, j, 0).Te = (*curGrid).bilerp(pos).Te;
            (*workingGrid)(i, j, 0).V = (*curGrid).bilerp(pos).V;
        }
    }
    swap();

    Grid& g = *curGrid;
    for (size_t i = 0; i < X; ++i) {
      for (size_t j = 0; j < Y; ++j) {
        for (size_t k = 0; k < Z; ++k) {
          if (i >= X/2 - 5 && i <= X/2 + 5 &&
              j >= Y/2 - 5 && j <= Y/2 + 5) {
            if (g.totalQuantity() < 1000000) {
              g(i, j, k).quantity() += frand(25,35);
              g(i, j, k).Te += frand(10, 40.0);
            }
            float scale = 10.0f;
            g(i, j, k).V += dt * vec3(scale, 0, 0);
          }
        }
      }
    }
    cout << g.totalQuantity() << endl;
}

void Fluid::swap()
{
    std::swap(curGrid, workingGrid);
}

void Fluid::advect(const float dt)
{
    Grid& g = *curGrid;
    Grid& h = *workingGrid;

    const size_t X = g.xDim();
    const size_t Y = g.yDim();
    const size_t Z = g.zDim();
    const float b_scale = -1.0f;


    for (size_t k = 0; k < Z; ++k) {
        // inner cells
        for (size_t i = 0; i < X; ++i) {
            for (size_t j = 0; j < Y; ++j) {
                vec3 pos = vec3(i, j, k) - g(i, j, k).V * dt;
                h(i, j, k).V = g.bilerp(pos).V;
            }
        }

        // edge cells
        for (size_t i = 1; i < X - 1; ++i) {
            h(i, 0    , k).V = b_scale * h(i, 1    , k).V;
            h(i, Y - 1, k).V = b_scale * h(i, Y - 2, k).V;
        }

        for (size_t j = 1; j < Y - 1; ++j) {
            h(0    , j, k).V = b_scale * h(1    , j, k).V;
            h(X - 1, j, k).V = b_scale * h(X - 2, j, k).V;
        }

        // corner cells
        h(0    , 0    , k).V = b_scale * h(1    , 1    , k).V;
        h(0    , Y - 1, k).V = b_scale * h(1    , Y - 2, k).V;
        h(X - 1, 0    , k).V = b_scale * h(X - 2, 1    , k).V;
        h(X - 1, Y - 1, k).V = b_scale * h(X - 2, Y - 2, k).V;
    }
}

// TODO: test for convergence
constexpr size_t convergence = 40;
void Fluid::project(const float dt)
{
    Grid& g = *curGrid;
    Grid& h = *workingGrid;

    const float dx = g.getDx();
    const size_t X = g.xDim();
    const size_t Y = g.yDim();
    const size_t Z = g.zDim();


    g.calc_divergence();
    for (size_t i = 0; i < X; ++i) {
      for (size_t j = 0; j < Y; ++j) {
        for (size_t k = 0; k < Z; ++k) {
          p[i][j][k] = 0.0;
          q[i][j][k] = 0.0;
        }
      }
    }

    /* Laplace - Jacobi iteration */
    for (size_t n = 0; n < convergence; ++n) {
        // single iteration
        for (size_t k = 0; k < Z; ++k) {
            // inner cells
            for (size_t i = 1; i < X - 1; ++i) {
                for (size_t j = 1; j < Y - 1; ++j) {
                    q[i][j][k] = ((p[i - 1][j    ][k] + p[i + 1][j    ][k] +
                                   p[i    ][j - 1][k] + p[i    ][j + 1][k])
                                 - (g(i, j, k).D * (dx * dx))) / 4.0f;
                }
            }

            // edge cells
            for (size_t i = 1; i < X - 1; ++i) {
                q[i][0    ][k] = q[i][1    ][k];
                q[i][Y - 1][k] = q[i][Y - 2][k];
            }

            for (size_t j = 1; j < Y - 1; ++j) {
                q[0    ][j][k] = q[1    ][j][k];
                q[X - 1][j][k] = q[X - 2][j][k];
            }

            // corner cells
            q[0    ][0    ][k] = q[1    ][1    ][k];
            q[0    ][Y - 1][k] = q[1    ][Y - 2][k];
            q[X - 1][0    ][k] = q[X - 2][1    ][k];
            q[X - 1][Y - 1][k] = q[X - 2][Y - 2][k];
        }

        // swap
        std::swap(p, q);
    }

    /* velocity -= gradient */
    for (size_t k = 0; k < Z; ++k) {
        for (size_t i = 1; i < X - 1; ++i) {
            for (size_t j = 1; j < Y - 1; ++j) {
                h(i, j, k).V = g(i, j, k).V -
                    vec3{
                          p[i][j][k] - p[i - 1][j    ][k]
                        , p[i][j][k] - p[i    ][j - 1][k]
                        , 0.0f
                    } / dx;
            }
        }
    }
}

void Fluid::forces(const float dt)
{
    float alpha = 3.7453;
    float beta = 0.1453;
    float grav = -9.08;
    float total_accel = 0.0;

    Grid& g = *curGrid;

    const size_t X = g.xDim();
    const size_t Y = g.yDim();
    const size_t Z = g.zDim();

    const float dx = g.getDx();

    for (size_t k = 1; k < Z - 1; ++k) {
        for (size_t i = 1; i < X - 1; ++i) {
            for (size_t j = 1; j < Y - 1; ++j) {
                omega[i][j][k] = vec3(
                                       abs(g(i, j+1, k).V.z - g(i, j-1, k).V.z - g(i, j, k+1).V.y - g(i, j, k-1).V.y) * 0.5 * 1 / dx
                                     , abs(g(i, j, k+1).V.x - g(i, j, k-1).V.x - g(i+1, j, k).V.z - g(i-1, j, k).V.z) * 0.5 * 1 / dx
                                     , abs(g(i+1, j, k).V.y - g(i-1, j, k).V.y - g(i, j+1, k).V.x - g(i, j-1, k).V.x) * 0.5 * 1 / dx
                                 );
            }
        }
    }

    for (size_t i = 0; i < X; ++i) {
      for (size_t j = 0; j < Y; ++j) {
        for (size_t k = 0; k < Z; ++k) {
          omega[i][j][k] = vec3(0.0, 0.0, 0.0);
          nu[i][j][k] = vec3(0.0, 0.0, 0.0);
          psi[i][j][k] = vec3(0.0, 0.0, 0.0);
          if(g(i, j, k).Q != 0.0){
            total_accel = (-alpha * (g(i, j, k).Q / max_quantity * 0.8) - beta * (300 - g(i,j,k).Te)) / (g(i, j, k).Q / max_quantity * 0.8 * dx * dx + 1.0 );
            g(i, j, k).V += vec3(0.0, (total_accel * dt), 0.0);
          }
        }
      }
    }
    /*
    float averagedensity = 0.0;
    for (size_t i = 1; i < X-1; ++i) {
      for (size_t j = 1; j < Y-1; ++j) {
        for (size_t k = 0; k < Z; ++k) {
          averagedensity = (g(i+1, j, k).Q + g(i-1, j, k).Q + g(i, j+1, k).Q + g(i, j-1, k).Q + g(i, j, k).Q) / 5;
          total_accel = (-alpha * (g(i, j, k).Q / 500.0) + beta * (30.0 * (averagedensity / 500.0))); // / (g(i, j, k).Q  + 1.0);
          total_accel += 0.01 * grav;
          h(i, j, k).V = vec3(0.0, (total_accel * dt), 0.0) + g(i, j, k).V;
        }
      }
    }
    */


    for (size_t k = 1; k < Z - 1; ++k) {
        for (size_t i = 1; i < X - 1; ++i) {
            for (size_t j = 1; j < Y - 1; ++j) {
                nu[i][j][k] = vec3(
                                    omega[i+1][j][k].x - omega[i-1][j][k].x * 0.5 * 1 / dx
                                  , omega[i][j+1][k].y - omega[i][j-1][k].y * 0.5 * 1 / dx
                                  , omega[i][j][k+1].z - omega[i][j][k-1].z * 0.5 * 1 / dx
                              );
                psi[i][j][k] = vec3(
                                    nu[i][j][k].x / abs(nu[i][j][k].x)
                                  , nu[i][j][k].y / abs(nu[i][j][k].y)
                                  , nu[i][j][k].z / abs(nu[i][j][k].z)
                              );
                vec3 crossed = cross(psi[i][j][k], omega[i][j][k]);
                crossed.x *= vorticity_epsilon * dx * dt;
                crossed.y *= vorticity_epsilon * dx * dt;
                crossed.z *= vorticity_epsilon * dx * dt;
                g(i, j, k).V += crossed;
            }
        }
    }
}

constexpr float radius = 30.0;
void Fluid::mesh(const float dt)
{
    Grid& g = *curGrid;

    const size_t X = g.xDim();
    const size_t Y = g.yDim();
    const size_t Z = g.zDim();

    const float dx = g.getDx();

    for (size_t i = 1; i < X - 1; ++i) {
        for (size_t j = 1; j < Y - 1; ++j) {
            size_t y = Y-j;
            size_t x = X-i;
            vec3 mesh = g(x,y,0).M;
            mesh *= length(mesh) * length(mesh);
            if (length(mesh) != 0.0) {
                g(i,j,0).V = -1.0f * dt * (mesh);
                g(i,j,0).V *= g(i,j,0).Q / (max_quantity);
            }
        }
    }
}
