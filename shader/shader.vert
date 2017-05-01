#version 130

uniform mat4 mvp;

in vec3 position;
in vec3 in_color;

out vec3 out_color;

void main()
{
    gl_Position = mvp * vec4(position, 1.0);
    out_color = in_color;
}
