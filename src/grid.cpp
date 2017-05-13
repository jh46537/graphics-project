#include <iostream>
#include <vector>
#include <functional>

using std::cout;
using std::cerr;
using std::endl;
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

Cell::Cell(vec3 V, float Q, float Te) : T(0), MVP(0), V(V), Q(Q), Te(Te) {};

Cell::Cell(const vec3 T, const mat4 MVP)
    : T(T), MVP(MVP), V(vec3{}), Q(0) {};

Cell::Cell(const Cell& that)
    : T(that.T), MVP(that.MVP), V(that.V), Q(that.Q) {};

Cell& Cell::operator=(const Cell& that)
{
    // does not overwrite T, MVP
    V = that.V;
    Q = that.Q;
    Te = that.Te;
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

//float& Cell::divergence()
//{
//    return D;
//}

//const float& Cell::divergence() const
//{
//    return D;
//}


/*
 * grid
 */
Grid::Grid(const uvec3& dim, const float dx, function<void (Grid&)> setup)
    : dim_x(dim.x), dim_y(dim.y), dim_z(dim.z), dx(dx)
{
    cells.reserve(dim_x * dim_y * dim_y);

    for (size_t i = 0; i < dim_x; ++i) {
        for (size_t j = 0; j < dim_y; ++j) {
            for (size_t k = 0; k < dim_z; ++k) {
                vec3 T{
                      ((float)i - (dim_x / 2.0) + 0.5) * (2 * dx)
                    , ((float)j - (dim_y / 2.0) + 0.5) * (2 * dx)
                    , ((float)k - (dim_z / 2.0) + 0.5) * (2 * dx)
                };
                mat4 MVP{};
                MVP = translate(MVP, T);
                MVP = scale(MVP, vec3{ dx, dx, dx });
                cells.push_back(Cell{ T, MVP });
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

const float Grid::getDx() const
{
    return dx;
}

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

const float Grid::totalQuantity() const
{
    float total = 0.0;
    for (size_t i = 0; i < dim_x; ++i) {
        for (size_t j = 0; j < dim_y; ++j) {
            for (size_t k = 0; k < dim_z; ++k) {
                total += self(i, j, k).Q;
            }
        }
    }
    return total;
}

Cell Grid::bilerp(vec3 pos) const
{
    size_t x1 = clamp(floor(pos.x), 0.0f, float(dim_x - 1));
    size_t x2 = clamp(ceil (pos.x), 0.0f, float(dim_x - 1));
    size_t y1 = clamp(floor(pos.y), 0.0f, float(dim_y - 1));
    size_t y2 = clamp(ceil (pos.y), 0.0f, float(dim_y - 1));
    size_t z1 = clamp(floor(pos.z), 0.0f, float(dim_z - 1));
    size_t z2 = clamp(ceil (pos.z), 0.0f, float(dim_z - 1));

    float x_amt = pos.x - x1;
    float y_amt = pos.y - y1;
    float z_amt = pos.z - z1;

    vec3 v_00 = mix(self(x1, y1, z1).V, self(x2, y1, z1).V, x_amt);
    vec3 v_01 = mix(self(x1, y2, z1).V, self(x2, y2, z1).V, x_amt);
    vec3 v_10 = mix(self(x1, y1, z2).V, self(x2, y1, z2).V, x_amt);
    vec3 v_11 = mix(self(x1, y2, z2).V, self(x2, y2, z2).V, x_amt);
    vec3 v_0  = mix(v_00, v_01, y_amt);
    vec3 v_1  = mix(v_10, v_11, y_amt);
    vec3 v    = mix(v_0, v_1, z_amt);

    float q_00 = mix(self(x1, y1, z1).Q, self(x2, y1, z1).Q, x_amt);
    float q_01 = mix(self(x1, y2, z1).Q, self(x2, y2, z1).Q, x_amt);
    float q_10 = mix(self(x1, y1, z2).Q, self(x2, y1, z2).Q, x_amt);
    float q_11 = mix(self(x1, y2, z2).Q, self(x2, y2, z2).Q, x_amt);
    float q_0  = mix(q_00, q_01, y_amt);
    float q_1  = mix(q_10, q_11, y_amt);
    float q    = mix(q_0, q_1, z_amt);

    float t_00 = mix(self(x1, y1, z1).Te, self(x2, y1, z1).Te, x_amt);
    float t_01 = mix(self(x1, y2, z1).Te, self(x2, y2, z1).Te, x_amt);
    float t_10 = mix(self(x1, y1, z2).Te, self(x2, y1, z2).Te, x_amt);
    float t_11 = mix(self(x1, y2, z2).Te, self(x2, y2, z2).Te, x_amt);
    float t_0  = mix(t_00, t_01, y_amt);
    float t_1  = mix(t_10, t_11, y_amt);
    float t    = mix(t_0, t_1, z_amt);

    return Cell( v, q, t );
}

void Grid::calc_divergence()
{
    for (size_t k = 0; k < dim_z - 1; ++k) {
        // inner cells
        for (size_t i = 0; i < dim_x - 1; ++i) {
            for (size_t j = 0; j < dim_y - 1; ++j) {
                self(i, j, k).D = ((self(i + 1, j    , k    ).V.x - self(i, j, k).V.x)
                                 + (self(i    , j + 1, k    ).V.y - self(i, j, k).V.y)
                                 + (self(i    , j    , k + 1).V.z - self(i, j, k).V.z)) / dx;
            }
        }

        // edge cells
        for (size_t i = 1; i < dim_x - 1; ++i) {
            self(i, 0        , k).D = self(i, 1        , k).D;
            self(i, dim_y - 1, k).D = self(i, dim_y - 2, k).D;
        }

        for (size_t j = 1; j < dim_y - 1; ++j) {
            self(0        , j, k).D = self(1        , j, k).D;
            self(dim_x - 1, j, k).D = self(dim_x - 2, j, k).D;
        }

        // corner cells
        self(0        , 0        , k).D = self(1        , 1        , k).D;
        self(0        , dim_y - 1, k).D = self(1        , dim_y - 2, k).D;
        self(dim_x - 1, 0        , k).D = self(dim_x - 2, 1        , k).D;
        self(dim_x - 1, dim_y - 1, k).D = self(dim_x - 2, dim_y - 2, k).D;
    }
    for (size_t i = 0; i < dim_x; ++i) {
      for (size_t j = 0; j < dim_y; ++j) {
        self(i, j, 0).D = self(i, j, 1).D;
        self(i, j, dim_z - 1).D = self(i, j, dim_z - 2).D;
      }
    }
}
