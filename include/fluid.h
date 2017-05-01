/*
 * grid
 */
class Grid
{
public:

    struct Cell
    {
        vec3 translate;
    };
    double scale;

private:

    size_t dim_x;
    size_t dim_y;
    size_t dim_z;
    double delta;

    vector<Cell> cells;

public:

    Grid(const vec3& dim, const double delta) : dim_x(dim.x), dim_y(dim.y), dim_z(dim.z), delta(delta)
    {
        scale = delta;
        for (size_t i = 0; i < dim_x; i++) {
            for (size_t j = 0; j < dim_y; j++) {
                for (size_t k = 0; k < dim_z; k++) {
                    vec3 t{
                          ((double)i - (dim_x / 2)) * delta
                        , ((double)j - (dim_y / 2)) * delta
                        , ((double)k - (dim_z / 2)) * delta
                    };
                    cells.push_back(Cell{t});
                }
            }
        }
    }

    size_t size() const
    {
        return cells.size();
    }

    const Cell& operator[](size_t i) const
    {
        return cells[i];
    }

    const Cell& at(vec3 addr) const
    {
        size_t index = addr.x * (dim_y * dim_z) + addr.y * dim_z + addr.z;
        return cells[index];
    }
};
