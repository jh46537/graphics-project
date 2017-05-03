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

    for (size_t i = 0; i < g.xDim(); i++)
    {
        for (size_t j = 0; j < g.yDim(); j++)
        {
            for (size_t k = 0; k < g.zDim(); k++)
            {
                vec3 pos = vec3(i,j,k) - vec3(0.02,-0.5,0.5) * 1.211f;
                (*workingGrid)(i,j,k) = curGrid->bilerp(pos);
            }
        }
    }
}

void Fluid::project(const float dt)
{

}
