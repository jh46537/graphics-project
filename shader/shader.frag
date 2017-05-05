#version 130

//uniform float opacity;
uniform float max_quantity;

in float quantity_f;
in float depth;
//in vec3 out_color;

out vec4 FragColor;

void main()
{
    //FragColor = vec4(1.0, 1.0, 1.0, opacity);
    float opacity = quantity_f / max_quantity;
    FragColor = vec4(1 - depth, 0.0, depth, opacity);
}
