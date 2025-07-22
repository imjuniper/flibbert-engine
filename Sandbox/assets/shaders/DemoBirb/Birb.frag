#version 410 core

in FRAGMENT_INPUTS
{
    vec2 Color;
} fragment_inputs;

out vec4 frag_color;

void main() {
    frag_color = fragment_inputs.Color;
}
