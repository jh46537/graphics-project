#include <iostream>
#include <vector>
#include <functional>

using std::cout;
using std::cerr;
using std::vector;
using std::function;


#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
using glm::uvec3;
using glm::mat4;
using glm::translate;
using glm::scale;
using glm::clamp;
using glm::floor;
using glm::ceil;
using glm::mix;

#include "grid.h"


/*
 * grid cell
 */
using Cell = Grid::Cell;

Cell::Cell(vec3 V, float Q) : T(0), MVP(0), V(V), Q(Q) {};

Cell::Cell(const vec3 T, const mat4 MVP, vec3 V, float Q)
    : T(T), MVP(MVP), V(V), Q(Q) {};

Cell::Cell(const Cell& that)
    : T(that.T), MVP(that.MVP), V(that.V), Q(that.Q) {};

Cell& Cell::operator=(const Cell& that)
{
    // does not overwrite T, MVP
    V = that.V;
    Q = that.Q;
    return *this;
}

//const vec3& Cell::translate() const
//{
//    return T;
//}

const mat4 Cell::mvp() const
{
    return MVP;
}

vec3& Cell::velocity()
{
    return V;
}

const vec3& Cell::velocity() const
{
    return V;
}

float& Cell::quantity()
{
    return Q;
}

const float& Cell::quantity() const
{
    return Q;
}


/*
 * grid
 */
Grid::Grid(const vec3& dim, const float dx, function<void (Grid&)> setup)
    : dim_x(dim.x), dim_y(dim.y), dim_z(dim.z), dx(dx)
{
    for (size_t i = 0; i < dim_x; i++) {
        for (size_t j = 0; j < dim_y; j++) {
            for (size_t k = 0; k < dim_z; k++) {
                vec3 T{
                      ((float)i - (dim_x / 2.0) + 0.5) * (2 * dx)
                    , ((float)j - (dim_y / 2.0) + 0.5) * (2 * dx)
                    , ((float)k - (dim_z / 2.0) + 0.5) * (2 * dx)
                };
                mat4 MVP{};
                MVP = translate(MVP, T);
                MVP = scale(MVP, vec3{ dx, dx, dx });
                cells.push_back(Cell{ T, MVP, vec3{ 0.0, 0.0, 0.0 }, 0.0 });
            }
        }
    }
    if (setup)
        setup(*this);
}

Grid::Grid(const Grid& that)
    : dim_x(that.dim_x), dim_y(that.dim_y), dim_z(that.dim_z), dx(that.dx),
      cells(that.cells)
{}

size_t Grid::size() const
{
    return cells.size();
}

const Cell& Grid::operator[](size_t i) const
{
    return cells[i];
}

Cell& Grid::operator()(size_t x, size_t y, size_t z)
{
    size_t index = x * (dim_y * dim_z) + y * dim_z + z;
    return cells[index];
}

Cell& Grid::operator()(uvec3 index)
{
    return (*this)(index.x, index.y, index.z);
}

//const float Grid::scale() const
//{
//    return dx;
//}

const size_t Grid::xDim() const
{
    return dim_x;
}

const size_t Grid::yDim() const
{
    return dim_y;
}

const size_t Grid::zDim() const
{
    return dim_z;
}

Cell Grid::bilerp(vec3 pos) const
{
    size_t x1 = clamp(floor(pos.x), 0.0f, float(dim_x - 1));
    size_t x2 = clamp(ceil (pos.x), 0.0f, float(dim_x - 1));
    size_t y1 = clamp(floor(pos.y), 0.0f, float(dim_y - 1));
    size_t y2 = clamp(ceil (pos.y), 0.0f, float(dim_y - 1));

    float x_amt = pos.x - x1;
    float y_amt = pos.y - y1;
    size_t z    = dim_z / 2;

    vec3 v_x1 = mix(self(x1, y1, z).V, self(x2, y1, z).V, x_amt);
    vec3 v_x2 = mix(self(x1, y2, z).V, self(x2, y2, z).V, x_amt);
    vec3 v    = mix(v_x1, v_x2, y_amt);

    float q_x1 = mix(self(x1, y1, z).Q, self(x2, y1, z).Q, x_amt);
    float q_x2 = mix(self(x1, y2, z).Q, self(x2, y2, z).Q, x_amt);
    float q    = mix(q_x1, q_x2, y_amt);

    return Cell( v, q );
}
