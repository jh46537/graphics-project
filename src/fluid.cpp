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

using glm::vec3;
using glm::uvec3;
using glm::mat4;


#include "grid.h"
#include "fluid.h"


/*
 * Navier–Stokes
 */
Fluid::Fluid(const vec3& dim, const float dx, function<void (Grid&)> setup)
{
    curGrid     = new Grid(dim, dx, setup);
    workingGrid = new Grid{ *curGrid };
    curGrid->id     = 1;
    workingGrid->id = 2;
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

void Fluid::step(const float dt)
{
    advect(dt);
    swap();
    //project(dt);
    //swap();
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


    for (size_t k = 0; k < Z; ++k) {
        // inner cells
        for (size_t i = 0; i < X; ++i) {
            for (size_t j = 0; j < Y; ++j) {
                vec3 pos = vec3(i, j, k) - g(i, j, k).V * dt;
                h(i, j, k) = g.bilerp(pos);
            }
        }

        // edge cells
        for (size_t i = 1; i < X - 1; ++i) {
            h(i, 0    , k).V = -1.0f * g(i, 1    , k).V;
            h(i, Y - 1, k).V = -1.0f * g(i, Y - 2, k).V;
        }

        for (size_t j = 1; j < Y - 1; ++j) {
            h(0    , j, k).V = -1.0f * g(1    , j, k).V;
            h(X - 1, j, k).V = -1.0f * g(X - 2, j, k).V;
        }

        // corner cells
        h(0    , 0    , k).V = -1.0f * g(1    , 1    , k).V;
        h(0    , Y - 1, k).V = -1.0f * g(1    , Y - 2, k).V;
        h(X - 1, 0    , k).V = -1.0f * g(X - 2, 1    , k).V;
        h(X - 1, Y - 1, k).V = -1.0f * g(X - 2, Y - 2, k).V;
    }
}

// TODO: test for convergence
constexpr size_t convergence = 1000;
void Fluid::project(const float dt)
{
    Grid& g = *curGrid;
    Grid& h = *workingGrid;

    const float dx = g.getDx();
    const size_t X = g.xDim();
    const size_t Y = g.yDim();
    const size_t Z = g.zDim();


    g.calc_divergence();

    /* Laplace - Jacobi iteration */
    float p[100][100][1] = { 0.0f };
    float q[100][100][1] = { 0.0f };
    for (size_t n = 0; n < convergence; ++n) {
        // single iteration
        for (size_t k = 0; k < Z; ++k) {
            // inner cells
            for (size_t i = 1; i < X - 1; ++i) {
                for (size_t j = 1; j < Y - 1; ++j) {
                    q[i][j][k] = ((p[i - 1][j    ][k] + p[i + 1][j    ][k] +
                                   p[i    ][j - 1][k] + p[i    ][j + 1][k])
                                 - (g(i, j, k).D * (dx * dx))) / 4;
                }
            }

            // edge cells
            for (size_t i = 1; i < X - 1; ++i) {
                q[i][0    ][k] = p[i][1    ][k];
                q[i][Y - 1][k] = p[i][Y - 2][k];
            }

            for (size_t j = 1; j < Y - 1; ++j) {
                q[0    ][j][k] = p[1    ][j][k];
                q[X - 1][j][k] = p[X - 2][j][k];
            }

            // corner cells
            q[0    ][0    ][k] = p[1    ][1    ][k];
            q[0    ][Y - 1][k] = p[1    ][Y - 2][k];
            q[X - 1][0    ][k] = p[X - 2][1    ][k];
            q[X - 1][Y - 1][k] = p[X - 2][Y - 2][k];
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
                    } /
                dx;
            }
        }
    }
}
