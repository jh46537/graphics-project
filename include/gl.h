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

    float dr;
    float dt;
    float dp;

public:

    Camera(float, float, float, float, float, float);

    const mat4 view() const;

    void left();
    void right();
    void up();
    void down();
    void in();
    void out();
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

    Window(size_t, size_t, size_t, size_t, const char*, float, float, float);
    ~Window();

    bool alive() const;
    void render(const Voxel&, const Fluid&, const GLint, const GLint, const float);

    static void key_callback(GLFWwindow*, int, int, int, int);
    void handle_input();
};
