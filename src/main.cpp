#include "main.h"

int main(int argc, char** argv)
{
    Window w;

    vector<Voxel> v;
    v.push_back(Voxel{});
    v.push_back(Voxel{});

    Shader s;
    s.add(GL_VERTEX_SHADER  , "shader/shader.vert");
    s.add(GL_FRAGMENT_SHADER, "shader/shader.frag");
    s.bind_attrib({"position", "in_color"});
    s.activate();
    GLint mvp_id = s.uniform("mvp");

    /*
     * render loop
     */
    while (w.alive()) {
        w.render(mvp_id, v);
    }

    return 0;
}
