#version 410 core

out vec4 color;

in FRAGMENT_INPUTS
{
    vec2 TexCoord;
} fragment_inputs;

uniform sampler2D u_Texture;

void main(){
    vec4 texColor = texture(u_Texture, fragment_inputs.TexCoord);
    if (gl_FrontFacing)
        color = texColor;
    else
        color = vec4(1.0, 0.0, 0.0, 1.0);
}
