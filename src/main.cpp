#include "main.h"

#include <fstream>
#include <string>
using std::ifstream;
using std::string;

float frand(float min = 0.0, float max = 1.0)
{
  return float(rand())/float(RAND_MAX) * (max - min) + min;
}

int main(int argc, char** argv)
{
    uvec3 dim{ dim_x, dim_y, dim_z };
    srand(time(NULL));

    /*
     * x:   -1 (left)             ->  +1 (right)
     * y:   -1 (down)             ->  +1 (up)
     * z:   -1 (out of screen)    ->  +1 (into screen)
     */
    ifstream mesh_f("mesh", ifstream::in);
    string header;
    mesh_f >> header;
    if (header != "abc123") {
        cout << "Invalid mesh format" << endl;
        return 1;
    }

    function<void (Grid&)> setup = [&mesh_f] (Grid& g)
    {
        for (size_t i = 0; i < dim_x; ++i) {
            for (size_t j = 0; j < dim_y; ++j) {
                for (size_t k = 0; k < dim_z; ++k) {
                    float r1 = frand(-5.0, 5.0);
                    float r2 = frand(-5.0, 5.0);
                    float r3 = frand(-5.0, 5.0);
                    g(i, j, k).velocity() = vec3{ r1 * 5.0, r2 * 5.0, r3 * 5.0 };
                    float m_x, m_y;
                    mesh_f >> m_x;
                    mesh_f >> m_y;
                    g(i, j, k).M = vec3 {m_x, m_y, 0.0f};
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
        , *sim
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
            //cout << "[render after " << num_ticks << " simulations]" << endl;
            //cout << "[render took " << (t_end - t_start).count() << " ns]" << endl;
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
