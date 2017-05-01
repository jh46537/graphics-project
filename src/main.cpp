#include "main.h"

int main(int argc, char** argv)
{
    Window w;

    Mesh m;

    Shader s;
    s.add(GL_VERTEX_SHADER  , "shader/shader.vert");
    s.add(GL_FRAGMENT_SHADER, "shader/shader.frag");
    s.bind_attrib({"position", "in_color"});
    s.activate();

    /*
     * render loop
     */
    while (w.alive()) {
        w.render(m);
    }

    return 0;
}
