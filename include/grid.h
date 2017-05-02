#pragma once

/*
 * grid
 */
class Grid
{
public:

    class Cell
    {
        vec3 T;

    public:
        vec3   V;
        double Q;

        Cell(vec3);
        Cell(vec3, double);
        Cell(vec3, vec3, double);

        Cell(const Cell&);

        Cell& operator=(const Cell&);

        const vec3& translate() const;

        vec3& velocity();

        double& quantity();

        const double& quantity() const;
    };

private:

    size_t dim_x;
    size_t dim_y;
    size_t dim_z;
    double dx;
    Grid& self = *this;

    vector<Cell> cells;

public:

    size_t id;

    Grid(const vec3&, const double, function<void (Grid&)>);

    Grid(const Grid&);

    size_t size() const;

    const Cell& operator[](size_t) const;

    Cell& operator()(size_t, size_t, size_t);

    Cell& operator()(vec3);

    Cell bilerp(vec3) const;

    const double scale() const;

    const size_t xDim() const;
    const size_t yDim() const;
    const size_t zDim() const;
};
