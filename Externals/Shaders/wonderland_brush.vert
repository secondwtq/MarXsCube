#version 330 core

out vec2 frag_texcoord;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 texcoord;

uniform mat4 model_view_and_projection;

void main() {

	frag_texcoord = texcoord.xy;

	vec3 position_t = position.xyz;
	vec4 position_4 = vec4(position_t, 1);

	gl_Position = model_view_and_projection * position_4; // ftransform();
}