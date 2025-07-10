#version 410 core

layout(location = 0) in vec2 a_Position;

layout (std140) uniform Matrices
{
    mat4 u_Projection;
    mat4 u_View;
};

uniform mat4 u_Transform;

void main() {
    gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0, 1.0);
}
