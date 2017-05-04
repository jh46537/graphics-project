#version 130

uniform float opacity;

in vec3 out_color;
in float depth;

out vec4 FragColor;

void main()
{
    FragColor = vec4(1 - depth, 0.0, depth, opacity);
}
