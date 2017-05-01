/*
 * grid
 */
class Grid
{
public:

    class Cell
    {
        vec3 t;

        vec3 velocity;
        float quantity;

    public:

        Cell(vec3 t) : t(t) {};

        const vec3 translate() const
        {
            return t;
        }

        const vec3 getVelocity() const
        {
            return velocity;
        }

        const float getQuantity() const
        {
            return quantity;
        }
    };

private:

    size_t dim_x;
    size_t dim_y;
    size_t dim_z;
    double delta;

    vector<Cell> cells;

public:

    Grid(const vec3& dim, const double delta)
        : dim_x(dim.x), dim_y(dim.y), dim_z(dim.z), delta(delta)
    {
        for (size_t i = 0; i < dim_x; i++) {
            for (size_t j = 0; j < dim_y; j++) {
                for (size_t k = 0; k < dim_z; k++) {
                    vec3 t{
                          ((double)i - (dim_x / 2.0) + 0.5) * (2 * delta)
                        , ((double)j - (dim_y / 2.0) + 0.5) * (2 * delta)
                        , ((double)k - (dim_z / 2.0) + 0.5) * (2 * delta)
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

    const Cell& operator()(size_t x, size_t y, size_t  z) const
    {
        size_t index = x * (dim_y * dim_z) + y * dim_z + z;
        return cells[index];
    }

    const Cell& operator()(vec3 index) const
    {
        return (*this)(index.x, index.y, index.z);
    }

    const double scale() const
    {
        return delta;
    }
};
