#version 330 core

out vec2 frag_texcoord;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 texcoord;

uniform mat4 model_view_and_projection;
uniform vec2 brush_position;
uniform vec2 brush_scale;

uniform sampler2D texture_mask;
uniform sampler2D texture_fill;

void main() {

	frag_texcoord = texcoord.xy;

	vec2 position_t = position.yx * brush_scale + vec2(brush_position.x, 2048 - brush_position.y);
	vec4 position_4 = vec4(position_t.x, position_t.y, 0, 1);

	gl_Position = model_view_and_projection * position_4; // ftransform();
}