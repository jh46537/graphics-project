#pragma once

/*
 * voxel
 */
class VoxelGrid
{
    GLuint vao;
    GLuint vbo;
    GLuint qbo;
    vector<vec3>   vertices;
    vector<GLuint> indices;
    vector<float>  quantities;
    size_t index_size;
    size_t num_vertices;

public:

    VoxelGrid(const Grid&);
    //VoxelGrid(VoxelGrid&&);
    ~VoxelGrid();
    void render(const Grid&);
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

    size_t width;
    size_t height;

    Fluid& sim;

    Camera camera;

    static bool keys[512];

    static bool click;
    size_t last_mouse_cell[2] = { (size_t)-1, (size_t)-1 };

public:

    static bool mesh;
    Window(size_t, size_t, size_t, size_t, const char*, Fluid&, float, float, float, float);
    ~Window();

    void start(const float, const GLint) const;
    bool alive() const;
    void render(VoxelGrid&, Fluid&, const GLint);

    static void key_callback(GLFWwindow*, int, int, int, int);
    void handle_input();
    static void mouse_callback(GLFWwindow*, int, int, int);
};
