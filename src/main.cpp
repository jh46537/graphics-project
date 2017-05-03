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

    /*
     * x:   -1 (left)             ->  +1 (right)
     * y:   -1 (down)             ->  +1 (up)
     * z:   -1 (out of screen)    ->  +1 (into screen)
     */
    function<void (Grid&)> setup = [] (Grid& g)
    {
        for (size_t i = 0; i < dim_x; i++) {
            for (size_t j = 0; j < dim_y; j++) {
                for (size_t k = 0; k < dim_z; k++) {
                    if (i >= dim_x / 4 && i < max<size_t>(dim_x * 3 / 4, 1) &&
                        j >= dim_y / 4 && j < max<size_t>(dim_y * 3 / 4, 1) &&
                        k >= dim_z / 4 && k < max<size_t>(dim_z * 3 / 4, 1)) {
                        g(i, j, k).quantity() = max_quantity;
                    }
                    else {
                        //g(i, j, k).quantity() = max_quantity / 2;
                    }
                    g(i, j, k).velocity() = vec3{ 0.0f, 1.0f, 0.0f };
                }
            }
        }
    };
    Fluid sim{ vec3{ dim_x, dim_y, dim_z }, dx, setup};

    /*
     * render loop
     */
    float dt;
    auto t_render    = clk::now();
    size_t num_ticks = 10;

    while (w.alive()) {
        if (clk::now() > t_render) {
            t_render += t_frame;
            auto t_start = clk::now();
            w.render(v, sim, mvp_loc, opc_loc, max_quantity);
            auto t_end   = clk::now();
            cout << "[render after " << num_ticks << " simulations]" << endl;
            cout << "[render took " << (t_end - t_start).count() << " ns]" << endl;
            dt = 1.0 / (fps * num_ticks);
            num_ticks = 0;
        }

        //auto t_start = clk::now();
        sim.step(dt);
        //auto t_end   = clk::now();

        ////dt = (t_end - t_start).count() / static_cast<float>(t_unit);
        //cout << "[simulation took " << (t_end - t_start).count() << " ns]" << endl;
        num_ticks++;
    }

    return 0;
}
