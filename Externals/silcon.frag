#version 330 core

in vec2 frag_texcoord;

out vec4 color;

uniform sampler2D texture_main;
uniform vec4 color_multiply;

void main() {

	vec4 t = texture(texture_main, frag_texcoord);
	if (t.a < 0.9f) discard;

	color = t * color_multiply;

}