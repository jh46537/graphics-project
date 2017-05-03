#include <iostream>
#include <vector>
#include <functional>

using std::cout;
using std::cerr;
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

    for (size_t i = 0; i < X; i++)
    {
        for (size_t j = 0; j < Y; j++)
        {
            for (size_t k = 0; k < Z; k++)
            {
                vec3 pos = vec3(i, j, k) - vec3(0.02, -0.50, 0.50) * 1.211f;
                //vec3 pos = vec3(i, j, k) - g(i, j, k).V * dt;
                (*workingGrid)(i,j,k) = curGrid->bilerp(pos);

                ///* advect */
                //vec3 pos = vec3(i, j, k) - g(i, j, k).V * dt;
                //h(i, j, k) = g.bilerp(pos);

                //// corner cases
                //if (i == 0 || i == X - 1 ||
                //    j == 0 || j == Y - 1) {
                //    if ((i == 0 || i == X - 1) &&
                //        (j == 0 || j == Y - 1)) {
                //        if (i == 0 && j == 0)
                //            h(i, j, k).V = -1.0f * (g(i + 1, j, k).V + g(i, j + 1, k).V) / 2.0f;
                //        else if (i == 0 && j == Y - 1)
                //            h(i, j, k).V = -1.0f * (g(i + 1, j, k).V + g(i, j - 1, k).V) / 2.0f;
                //        else if (i == X - 1 && j == 0)
                //            h(i, j, k).V = -1.0f * (g(i - 1, j, k).V + g(i, j + 1, k).V) / 2.0f;
                //        else
                //            h(i, j, k).V = -1.0f * (g(i - 1, j, k).V + g(i, j - 1, k).V) / 2.0f;
                //    }
                //    else if (i == 0 || i == X - 1) {
                //        if (i == 0)
                //            h(i, j, k).V = -1.0f * g(i + 1, j, k).V;
                //        else
                //            h(i, j, k).V = -1.0f * g(i - 1, j, k).V;
                //    }
                //    else {
                //        if (j == 0)
                //            h(i, j, k).V = -1.0f * g(i, j + 1, k).V;
                //        else
                //            h(i, j, k).V = -1.0f * g(i, j - 1, k).V;
                //    }
                //}
            }
        }
    }
}

void Fluid::project(const float dt)
{

}
