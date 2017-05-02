#include "main.h"

int main(int argc, char** argv)
{
    Window w{ version_major, version_minor, width, height, name };

    Voxel v;

    Shader s;
    s.add(GL_VERTEX_SHADER  , "shader/shader.vert");
    s.add(GL_FRAGMENT_SHADER, "shader/shader.frag");
    s.bind_attrib({"position", "in_color"});
    s.activate();
    GLint mvp_loc = s.uniform("mvp");
    GLint opc_loc = s.uniform("opacity");

    function<void (Grid&)> setup = [] (Grid& g)
    {
        for (size_t i = dim_x / 4; i < max<size_t>(dim_x * 3 / 4, 1); i++)
            for (size_t j = dim_y / 4; j < max<size_t>(dim_y * 3 / 4, 1); j++)
                for (size_t k = dim_z / 4; k < max<size_t>(dim_z * 3 / 4, 1); k++)
                    g(i, j, k).quantity() = max_quantity;
    };
    Fluid sim{ vec3{ dim_x, dim_y, dim_z }, dx, setup};

    /*
     * render loop
     */
    double dt        = 1.0 / (fps * 10);
    auto t_render    = clk::now();
    size_t num_ticks = 0;

    while (w.alive()) {
        if (clk::now() > t_render) {
            t_render += t_frame;
            auto t_start = clk::now();
            w.render(v, sim, mvp_loc, opc_loc, max_quantity);
            auto t_end   = clk::now();
            //cout << "[render after " << num_ticks << " simulations]" << endl;
            //cout << "[render took " << (t_end - t_start).count() << " ns]" << endl;
            dt = 1.0 / (fps * num_ticks);
            num_ticks = 0;
        }

        auto t_start = clk::now();
        sim.step(dt);
        auto t_end   = clk::now();

        //dt = (t_end - t_start).count() / static_cast<double>(t_unit);
        //cout << "[simulation took " << (t_end - t_start).count() << " ns]" << endl;
        num_ticks++;
    }

    return 0;
}
