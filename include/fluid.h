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

    void advect(double dt)
    {

    }

    void project(double dt)
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

    void step(double dt)
    {

    }
};
