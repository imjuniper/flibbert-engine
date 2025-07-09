#version 410 core

out vec4 color;

in VS_OUT
{
    vec2 TexCoord;
} fs_in;

uniform sampler2D u_Texture;

void main(){
    vec4 texColor = texture(u_Texture, fs_in.TexCoord);
    if (gl_FrontFacing)
        color = texColor;
    else
        color = vec4(1.0, 0.0, 0.0, 1.0);
}
