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
                for (int k = 0; k < curGrid->zDim(); k++)
                    {
                        Grid::Cell& cur = (*curGrid)(i,j,k);
                        vec3 pos = vec3(i,j,k) - vec3(0.02,-0.5,0.5) * 1.211f;
                        (*workingGrid)(i,j,k) = curGrid->bilerp(pos);
                    }
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
        workingGrid = new Grid{ *curGrid };
    }

    ~Fluid()
    {
        delete(curGrid);
        delete(workingGrid);
    }

    const Grid& getGrid() const { return *curGrid; }

    void step(float dt)
    {
        printf("wellistepped\n");
        advect(dt);
        swap();
    }
};
