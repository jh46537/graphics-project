#pragma once

/*
 * Navier–Stokes
 */
class Fluid
{
    Grid* curGrid;
    Grid* workingGrid;

    using float3 = vector<vector<vector<float>>>;
    float3 p;
    float3 q;

    void swap();

    void advect(const float);
    void project(const float);

public:

    Fluid(const vec3&, const float, function<void (Grid&)>);
    ~Fluid();

    const Grid& getGrid() const;

    void step(const float);
};
