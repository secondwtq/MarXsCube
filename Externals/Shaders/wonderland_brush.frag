#version 330 core

in vec2 frag_texcoord;

out vec4 color;

uniform sampler2D texture_mask;
uniform sampler2D texture_fill;

void main() {

	vec4 mask = texture(texture_mask, frag_texcoord);
	vec4 fill = texture(texture_fill, frag_texcoord);

	color = vec4(fill.xyz, mask.a * fill.a);

}