#pragma once

#include "grid.h"

class Fluid
{
    void swap(Grid **working_grid, Grid **grid)
    {
        Grid *tmp_grid = *working_grid;
        *working_grid = *grid;
        *grid = tmp_grid;
    }

    Grid grid1, grid2;
    Grid *curGrid, *workingGrid;

public:
    Fluid(const vec3& dim, const double delta)
      : grid1(dim, delta), grid2(dim, delta),
        curGrid(&grid1), workingGrid(&grid2)
    {
    }

    const Grid& getGrid() const { return *curGrid; }

    void step(float dt)
    {

    }

    void advect(float dt)
    {

    }

    void project(float dt)
    {

    }
};
