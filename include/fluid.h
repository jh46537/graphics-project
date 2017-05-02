#pragma once

#include <functional>
#include <algorithm>
using std::function;

#include "grid.h"

class Fluid
{
    void swap()
    {
        std::swap(curGrid, workingGrid);
    }

    Grid* curGrid;
    Grid* workingGrid;

    void advect(float dt)
    {
        for (int i = 0; i < curGrid->xDim(); i++)
        {
            for (int j = 0; j < curGrid->yDim(); j++)
            {
                Grid::Cell& cur = (*curGrid)(i,j,0);
                vec3 pos = vec3(i,j,0) - cur.V * dt;
                (*workingGrid)(i,j,0) = curGrid->bilerp(pos);
            }
        }
    }

    void project(float dt)
    {

    }

public:

    Fluid(const vec3& dim, const double dx, function<void (Grid&)> setup)
    {
        curGrid     = new Grid(dim, dx, setup);
        workingGrid = new Grid(dim, dx, nullptr);
    }

    ~Fluid()
    {
        delete(curGrid);
        delete(workingGrid);
    }

    const Grid& getGrid() const { return *curGrid; }

    void step(float dt)
    {
        advect(dt);
        swap();
    }
};
