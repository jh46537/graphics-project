#pragma once

/*
 * grid
 */
class Grid
{
public:

    class Cell
    {
        const vec3 T;
        const mat4 MVP;

    public:

        vec3  V;
        float Q;

        Cell(vec3, float);
        Cell(const vec3, const mat4, vec3, float);
        Cell(const Cell&);

        Cell& operator=(const Cell&);

        //const vec3& translate() const;
        const mat4 mvp() const;

        vec3& velocity();
        const vec3& velocity() const;
        float& quantity();
        const float& quantity() const;
    };

private:

    const size_t dim_x;
    const size_t dim_y;
    const size_t dim_z;
    const float dx;
    Grid& self = *this;

    vector<Cell> cells;

public:

    size_t id;

    Grid(const vec3&, const float, function<void (Grid&)>);
    Grid(const Grid&);

    size_t size() const;
    const Cell& operator[](size_t) const;
    Cell& operator()(size_t, size_t, size_t);
    Cell& operator()(vec3);

    //const float scale() const;
    const size_t xDim() const;
    const size_t yDim() const;
    const size_t zDim() const;

    Cell bilerp(vec3) const;
};
