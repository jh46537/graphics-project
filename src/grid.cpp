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

Cell::Cell(vec3 V, double Q) : T(0), MVP(0), V(V), Q(Q) {};

Cell::Cell(const vec3 T, const mat4 MVP, vec3 V, double Q)
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

double& Cell::quantity()
{
    return Q;
}

const double& Cell::quantity() const
{
    return Q;
}


/*
 * grid
 */
Grid::Grid(const vec3& dim, const double dx, function<void (Grid&)> setup)
    : dim_x(dim.x), dim_y(dim.y), dim_z(dim.z), dx(dx)
{
    for (size_t i = 0; i < dim_x; i++) {
        for (size_t j = 0; j < dim_y; j++) {
            for (size_t k = 0; k < dim_z; k++) {
                vec3 T{
                      ((double)i - (dim_x / 2.0) + 0.5) * (2 * dx)
                    , ((double)j - (dim_y / 2.0) + 0.5) * (2 * dx)
                    , ((double)k - (dim_z / 2.0) + 0.5) * (2 * dx)
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

Cell& Grid::operator()(vec3 index)
{
    return (*this)(index.x, index.y, index.z);
}

//const double Grid::scale() const
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
    int x1 = clamp(floor(pos.x), 0.0f, (float) dim_x - 1);
    int x2 = clamp(ceil(pos.x),  0.0f, (float) dim_x - 1);
    int y1 = clamp(floor(pos.y), 0.0f, (float) dim_y - 1);
    int y2 = clamp(ceil(pos.y),  0.0f, (float) dim_y - 1);

    float xamt = pos.x - x1;
    float yamt = pos.y - y1;
    int zamt = dim_z / 2;

    vec3 lerp1_v = mix(self(x1,y1,zamt).V, self(x2,y1,zamt).V, xamt);
    vec3 lerp2_v = mix(self(x1,y2,zamt).V, self(x2,y2,zamt).V, xamt);
    vec3 v = mix(lerp1_v, lerp2_v, yamt);

    float lerp1_q = mix(self(x1,y1,zamt).Q, self(x2,y1,zamt).Q, xamt);
    float lerp2_q = mix(self(x1,y2,zamt).Q, self(x2,y2,zamt).Q, xamt);
    float q = mix(lerp1_q, lerp2_q, yamt);

    return Cell( v, q );
}
