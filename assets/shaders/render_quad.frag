#version 460 core

uniform sampler2D tex_sampler;
in vec2 tex_coords;

out vec4 frag_col;

void main() {
    frag_col = texture(tex_sampler, tex_coords);
}