#pragma once

using glm::clamp;
using glm::floor;
using glm::ceil;
using glm::mix;

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

        Cell(vec3 T) : T(T), V(0), Q(0) {};
        Cell(vec3 V, double Q) : V(V), Q(Q) {};

        Cell& operator=(Cell const & that)
        {
            V = that.V;
            Q = that.Q;
            return *this;
        }

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

        const double& quantity() const
        {
            return Q;
        }
    };

private:

    size_t dim_x;
    size_t dim_y;
    size_t dim_z;
    double dx;
    Grid& self = *this;

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

    Grid(const Grid& that)
        : dim_x(that.dim_x), dim_y(that.dim_y), dim_z(that.dim_z), dx(that.dx)
    {
        cells.clear();
        for (size_t i; i < that.size(); i++)
            cells.push_back(that[i]);
    }

    size_t size() const
    {
        return cells.size();
    }

    const Cell& operator[](size_t i) const
    {
        return cells[i];
    }

    Cell& operator()(size_t x, size_t y, size_t z)
    {
        size_t index = x + y * dim_x + z * (dim_x * dim_y);
        return cells[index];
    }

    Cell& operator()(vec3 index)
    {
        return (*this)(index.x, index.y, index.z);
    }

    Cell bilerp(vec3 pos)
    {
        float x1 = clamp(floor(pos.x), 0.0f, (float) dim_x - 1);
        float x2 = clamp(ceil(pos.x),  0.0f, (float) dim_x - 1);
        float y1 = clamp(floor(pos.y), 0.0f, (float) dim_y - 1);
        float y2 = clamp(ceil(pos.y),  0.0f, (float) dim_y - 1);

        float xamt = pos.x - x1;
        float yamt = pos.y - y1;

        vec3 lerp1_v = mix(self(x1,y1,0).V, self(x2,y1,0).V, xamt);
        vec3 lerp2_v = mix(self(x1,y2,0).V, self(x2,y2,0).V, xamt);
        vec3 v = mix(lerp1_v, lerp2_v, yamt);

        float lerp1_q = mix(self(x1,y1,0).Q, self(x2,y1,0).Q, xamt);
        float lerp2_q = mix(self(x1,y2,0).Q, self(x2,y2,0).Q, xamt);
        float q = mix(lerp1_q, lerp2_q, yamt);

        return Cell(v, q);
    }

    const double scale() const
    {
        return dx;
    }

    const size_t xDim() const { return dim_x; }
    const size_t yDim() const { return dim_y; }
    const size_t zDim() const { return dim_z; }
};
