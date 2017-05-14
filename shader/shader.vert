#version 130

uniform mat4 mvp;

in vec3 position;
in float quantity_v;
//in vec3 in_color;

out float quantity_f;
//out float depth;
//out vec3 out_color;

void main()
{
    gl_Position = mvp * vec4(position, 1.0);
    //depth = gl_Position.z - 0.5;
    quantity_f = quantity_v;
    //out_color = in_color;
}
