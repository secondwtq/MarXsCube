#version 330 core

out vec2 frag_texcoord;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 texcoord;

uniform vec4 color_multiply;
uniform vec2 sprite_position;
uniform sampler2D texture_main;
uniform mat4 model_view_and_projection;

void main() {

	frag_texcoord = texcoord.xy;

	vec2 position_t = position.xy + sprite_position;
	vec4 position_4 = vec4(position_t, position.z, 1);

	gl_Position = model_view_and_projection * position_4; // ftransform();
}