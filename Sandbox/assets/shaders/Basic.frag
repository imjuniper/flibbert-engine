#version 460 core
#extension GL_ARB_bindless_texture : require

// Uniform buffers
layout(std140) uniform BasicData {
    sampler2D u_Texture;
};

// Inputs
in VertexData
{
    vec2 TexCoord;
} vertexData;

// Outputs
layout(location = 0) out vec4 frag_color;

void main() {
    vec4 texColor = texture(u_Texture, vertexData.TexCoord);
    if (gl_FrontFacing)
        frag_color = texColor;
    else
        frag_color = vec4(1.0, 0.0, 0.0, 1.0);
}
