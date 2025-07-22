#version 410 core

// Inputs
in VertexData
{
    vec3 Color;
} vertexData;

// Outputs
layout(location = 0) out vec4 frag_color;

void main() {
    frag_color = vec4(vertexData.Color, 1.0);
}
