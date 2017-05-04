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
    vec3* omega;
    vec3* nu;
    vec3* psi;

    float vorticity_epsilon = 0.01;
    void swap();

    void advect(const float);
    void project(const float);
    void forces(const float);

public:

    Fluid(const vec3&, const float, function<void (Grid&)>);
    ~Fluid();

    const Grid& getGrid() const;

    void step(const float);
};
