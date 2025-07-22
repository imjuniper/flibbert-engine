#version 410 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec4 a_Color;

layout (std140) uniform Matrices
{
    mat4 u_Projection;
    mat4 u_View;
    mat4 u_Model;
};

out FRAGMENT_INPUTS
{
    vec4 Color;
} fragment_inputs;

void main() {
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 0.0, 1.0);
    fragment_inputs.Color = a_Color;
}
