#pragma once

/*
 * Navier–Stokes
 */
class Fluid
{
    Grid* curGrid;
    Grid* workingGrid;

    template <typename T>
    using field = vector<vector<vector<T>>>;
    field<float> p;
    field<float> q;
    field<vec3> omega;
    field<vec3> nu;
    field<vec3> psi;

    float vorticity_epsilon = 0.01;
    void swap();

    void advect(const float);
    void project(const float);
    void forces(const float);
    void mesh(const float);

public:

    Fluid(const uvec3&, const float, function<void (Grid&)>);
    ~Fluid();

    const Grid& getGrid() const;
    Grid& getGrid();

    void step(const float);
};
