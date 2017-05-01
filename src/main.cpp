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

    Grid g{ vec3{ dim_x, dim_y, dim_z }, delta};

    /*
     * render loop
     */
    while (w.alive()) {
        w.render(v, g, mvp_loc);
    }

    return 0;
}
