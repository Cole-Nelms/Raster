
#version 330 core

in vec2 frag_uv;

uniform sampler2D u_tex;

out vec4 out_color;

void main()
{
	out_color = texture(u_tex, frag_uv);
} 