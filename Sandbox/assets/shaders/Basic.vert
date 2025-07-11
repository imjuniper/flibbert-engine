#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out FRAGMENT_INPUTS
{
    vec2 TexCoord;
} fragment_inputs;

layout (std140) uniform Matrices
{
    mat4 u_Projection;
    mat4 u_View;
};

uniform mat4 u_Transform;

void main(){
    gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
    fragment_inputs.TexCoord = a_TexCoord;
}
