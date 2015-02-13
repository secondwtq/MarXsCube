#version 330 core

#define USE_HEIGHTFIELD 1

#define TEXATLAS_COUNT (8.0)

out vec3 frag_normal;
out vec3 frag_light_dir;
out vec2 frag_texcoord;
out vec2 frag_height_texcoord;
out vec3 frag_blendweights;
out vec3 frag_texture_indexes;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 texcoord;
layout(location = 3) in vec3 blendweights;
layout(location = 4) in vec3 texindexes;

uniform vec3 chunk_position;
uniform mat4 model_view_and_projection;
uniform sampler2D texture_main;
uniform sampler2D texture_second;
uniform sampler2D texture_heightfield;
uniform sampler2D texture_tileset;

// solution fram
//		http://stackoverflow.com/questions/5281261/generating-a-normal-map-from-a-height-map
vec4 parse_heightfield(in sampler2D heightfield_sampler, in vec2 texcoord) {
	const vec2 size = vec2(2.0,0.0);
	const vec3 off = vec3(-1.0,0.0,1.0) * (1.0/30.0);

	vec4 height = texture(heightfield_sampler, texcoord);

    float s11 = height.x;
    float s01 = texture(heightfield_sampler, texcoord+off.xy).x;
    float s21 = texture(heightfield_sampler, texcoord+off.zy).x;
    float s10 = texture(heightfield_sampler, texcoord+off.yx).x;
    float s12 = texture(heightfield_sampler, texcoord+off.yz).x;

    vec3 va = normalize(vec3(size.xy,s21-s01));
    vec3 vb = normalize(vec3(size.yx,s12-s10));

    return vec4( cross(va,vb), s11 );
}

vec2 texatlas_offset(in float index, in float count) {
	float _texture_id = index / count;
	vec2 offset_tile = vec2(fract(_texture_id) * count, floor(_texture_id));
	return vec2(offset_tile / count);
}

void main() {

	vec3 light_dir = vec3(1.5, 1.5, 1);

	frag_normal = normalize(normal);
	frag_light_dir = normalize(light_dir);
	frag_texcoord = texcoord.xy;
	frag_height_texcoord = texcoord.xy / 30.0;
	frag_blendweights = blendweights;

	frag_texture_indexes = texindexes;

	// frag_texcoord = texatlas_offset(frag_texture_index.x, TEXATLAS_COUNT) + fract(frag_texcoord) / TEXATLAS_COUNT;

	vec4 position_4 = vec4(position.xyz + chunk_position.yxz, 1);

#if USE_HEIGHTFIELD == 1
    vec4 height = parse_heightfield(texture_heightfield, frag_height_texcoord);
    frag_normal = normalize(height.xyz);
	position_4.z += height.w * 128.0;
#endif
	// position_4.z += texture(s_texture_heightfield, frag_height_texcoord).x * 128.0;

	gl_Position = model_view_and_projection * position_4; // ftransform();
	// gl_Position = ftransform();
}