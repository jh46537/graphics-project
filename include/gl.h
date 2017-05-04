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


/*
 * camera
 */
class Camera
{
    float R;    // radius
    float T;    // theta
    float P;    // phi

    const float speed_r;
    const float speed_t;
    const float speed_p;
    const float speed_div;

public:

    Camera(float, float, float, float, float, float, float);

    const mat4 view() const;

    void left(bool);
    void right(bool);
    void up(bool);
    void down(bool);
    void in(bool);
    void out(bool);
};


/*
 * window
 */
class Window
{
    GLFWwindow* window;

    Camera camera;

    static bool keys[512];

public:

    Window(size_t, size_t, size_t, size_t, const char*, float, float, float, float);
    ~Window();

    bool alive() const;
    void render(const Voxel&, const Fluid&, const GLint, const GLint, const float);

    static void key_callback(GLFWwindow*, int, int, int, int);
    void handle_input();
};
