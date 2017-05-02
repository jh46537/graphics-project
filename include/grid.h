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

        vec3   V;
        double Q;

    public:

        Cell(vec3 T) : T(T) {};

        const vec3& translate() const
        {
            return T;
        }

        vec3& velocity()
        {
            return V;
        }

        double& quantity()
        {
            return Q;
        }
    };

private:

    size_t dim_x;
    size_t dim_y;
    size_t dim_z;
    double dx;

    vector<Cell> cells;

public:

    Grid(const vec3& dim, const double dx, function<void (Grid&)> setup)
        : dim_x(dim.x), dim_y(dim.y), dim_z(dim.z), dx(dx)
    {
        for (size_t i = 0; i < dim_x; i++) {
            for (size_t j = 0; j < dim_y; j++) {
                for (size_t k = 0; k < dim_z; k++) {
                    cells.push_back(
                        vec3 {
                              ((double)i - (dim_x / 2.0) + 0.5) * (2 * dx)
                            , ((double)j - (dim_y / 2.0) + 0.5) * (2 * dx)
                            , ((double)k - (dim_z / 2.0) + 0.5) * (2 * dx)
                        }
                    );
                }
            }
        }
        if (setup)
            setup(*this);
    }

    size_t size() const
    {
        return cells.size();
    }

    const Cell& operator[](size_t i) const
    {
        return cells[i];
    }

    Cell& operator()(size_t x, size_t y, size_t  z)
    {
        size_t index = x * (dim_y * dim_z) + y * dim_z + z;
        return cells[index];
    }

    Cell& operator()(vec3 index)
    {
        return (*this)(index.x, index.y, index.z);
    }

    const double scale() const
    {
        return dx;
    }
};
