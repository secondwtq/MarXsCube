#version 330 core

out vec3 frag_color;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

uniform mat4 model_view_and_projection;

void main() {
	frag_color = color;

	vec4 position_4 = vec4(position, 1);
	gl_Position = model_view_and_projection * position_4;
}