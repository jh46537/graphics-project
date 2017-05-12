#include "main.h"

int main(int argc, char** argv)
{
    uvec3 dim{ dim_x, dim_y, dim_z };

    /*
     * x:   -1 (left)             ->  +1 (right)
     * y:   -1 (down)             ->  +1 (up)
     * z:   -1 (out of screen)    ->  +1 (into screen)
     */
    function<void (Grid&)> setup = [] (Grid& g)
    {
        for (size_t i = 0; i < dim_x; ++i) {
            for (size_t j = 0; j < dim_y; ++j) {
                for (size_t k = 0; k < dim_z; ++k) {
                    if (i >= dim_x / 4 && i < max<size_t>(dim_x * 3 / 4, 1) &&
                        j >= dim_y / 4 && j < max<size_t>(dim_y * 3 / 4, 1) &&
                        k >= dim_z / 4 && k < max<size_t>(dim_z * 3 / 4, 1)) {
                        g(i, j, k).quantity() = max_quantity/2;
                        g(i, j, k).Te = 350.0;
                    }
                    else {
                        //g(i, j, k).quantity() = max_quantity / 2;
                        g(i, j, k).Te = 300.0;
                    }
                    float r1 = (float(rand()) / float(RAND_MAX)) * 2.0 - 1.0;
                    float r2 = float(rand()) / float(RAND_MAX) * 2.0 - 1.0;
                    float r3 = float(rand()) / float(RAND_MAX) * 2.0 - 1.0;
                    g(i, j, k).velocity() = vec3{ r1 * 5.0, r2 * 5.0, r3 * 5.0 };
                }
            }
        }
    };
    Fluid* sim = new Fluid{ dim, dx, setup};

    Window* w = new Window{
          version_major
        , version_minor
        , width
        , height
        , name
        , speed_r
        , speed_t
        , speed_p
        , speed_div
    };

    Shader* s = new Shader;
    s->add(GL_VERTEX_SHADER  , "shader/shader.vert");
    s->add(GL_FRAGMENT_SHADER, "shader/shader.frag");
    ////s->bind_attrib({"position", "in_color"});
    s->bind_attrib({"position", "quantity_v"});
    s->activate();
    GLint mvp_loc = s->uniform("mvp");
    GLint max_loc = s->uniform("max_quantity");
    ////GLint opc_loc = s->uniform("opacity");

    w->start(max_quantity, max_loc);

    VoxelGrid* v = new VoxelGrid{ sim->getGrid() };

    /*
     * render loop
     */
    float dt         = 1.0f;
    auto t_render    = clk::now();
    size_t num_ticks = 10;          // initialize > 0

    while (w->alive()) {
        if (clk::now() > t_render) {
            t_render += t_frame;
            auto t_start = clk::now();
            w->render(*v, *sim, mvp_loc);
            auto t_end   = clk::now();
            cout << "[render after " << num_ticks << " simulations]" << endl;
            cout << "[render took " << (t_end - t_start).count() << " ns]" << endl;
            dt = 1.0 / (fps * num_ticks);
            num_ticks = 0;
        }

        //auto t_start = clk::now();
        sim->step(dt);
        //auto t_end   = clk::now();

        ////dt = (t_end - t_start).count() / static_cast<float>(t_unit);
        //cout << "[simulation took " << (t_end - t_start).count() << " ns]" << endl;
        ++num_ticks;
    }

    delete(v);
    delete(s);
    delete(w);
    delete(sim);

    return 0;
}
