#pragma once

/*
 * voxel
 */
class Voxel
{
    GLuint vao;
    size_t index_size;

public:

    Voxel();
    Voxel(Voxel&&);
    ~Voxel();
    void render() const;
};


/*
 * shader
 */
class Shader
{
    GLuint program;

public:

    Shader();
    ~Shader();

    void add(size_t, const char*) const;
    void activate() const;

    GLint uniform(const char*) const;
    void bind_attrib(const vector<const char*>&) const;
};


class Fluid;

/*
 * window
 */
class Window
{
    GLFWwindow* window;

public:

    Window(size_t, size_t, size_t, size_t, const char*);
    ~Window();

    bool alive() const;
    void render(const Voxel&, const Fluid&, const GLint, const GLint, const float) const;
};
