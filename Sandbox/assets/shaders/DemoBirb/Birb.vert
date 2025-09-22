#version 460 core

// Uniform buffers
layout (std140) uniform PerFrameData
{
    mat4 u_View;
    mat4 u_Projection;
    vec3 u_ViewPosition;
};

layout (std140) uniform PerObjectData
{
    mat4 u_Transform;
};

// Inputs (vertex data in this case)
layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec3 a_Color;

// Outputs
out VertexData
{
    vec3 Color;
} vertexData;

void main() {
    gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0, 1.0);
    vertexData.Color = a_Color;
}
