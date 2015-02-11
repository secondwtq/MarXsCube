#version 330 core

in vec2 frag_texcoord;

out vec3 color;

uniform sampler2D texture_main;
// uniform vec4 color_multiply;

void main() {

	vec4 t = texture(texture_main, frag_texcoord);

	// color = t.rgb * color_multiply.rgb;
	color = vec3(1, 1, 1);

}