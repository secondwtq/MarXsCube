// attribute vec4 gl_Color;

#define LIGHTING_ONLY 0
#define USE_LIGHTING 1

#define TEXATLAS_COUNT (8.0)

varying vec3 frag_normal;
varying vec3 frag_light_dir;
varying vec2 frag_texcoord;
varying vec3 frag_blendweights;
varying vec2 frag_height_texcoord;
varying vec3 frag_texture_indexes;

uniform sampler2D texture_main;
uniform sampler2D texture_second;
uniform sampler2D texture_heightfield;

uniform sampler2D texture_tileset;

// from
//		http://gamedev.stackexchange.com/questions/73586/how-can-i-repeat-scroll-a-tile-which-is-part-of-an-texture-atlas
vec2 texatlas_offset(in float index, in float count) {
	float _texture_id = index / count;
	vec2 offset_tile = vec2(fract(_texture_id) * count, floor(_texture_id));
	return vec2(offset_tile / count);
}

vec4 sample_scale_and_offset(in vec2 texcoord, in sampler2D tileset, in vec2 offset, in float scale, in float count) {
	vec2 texcoord_org = texcoord * scale;

	vec2 texcoord_scaled = fract(texcoord_org);
	vec2 texcoord_atlas = fract(texcoord_scaled) / count + offset;
	return texture2D(tileset, texcoord_atlas);
}

void main() {

#if USE_LIGHTING == 1
	float intensity = dot(frag_light_dir, frag_normal);
#else
	float intensity = 1.0;
#endif

	vec2 texcoord_org = frag_texcoord.xy;
	texcoord_org = fract(frag_texcoord.xy);

	vec2 offset = texatlas_offset(frag_texture_indexes.x, TEXATLAS_COUNT);
	vec2 offset_secondary = texatlas_offset(frag_texture_indexes.y, TEXATLAS_COUNT);

	vec2 texcoord_atlas_primary = fract(texcoord_org) / TEXATLAS_COUNT + offset;
	vec2 texcoord_atlas_secondary = fract(texcoord_org) / TEXATLAS_COUNT + offset_secondary;

#if LIGHTING_ONLY == 1
	gl_FragColor = vec4(1, 1, 1, 0) * intensity;
#else
	
	vec4 color_original = mix(texture2D(texture_tileset, texcoord_atlas_primary), 
						texture2D(texture_tileset, texcoord_atlas_secondary), frag_blendweights.x);
	vec4 color_scaled_1 = mix(sample_scale_and_offset(frag_texcoord, texture_tileset, offset, 1.0/6.0, TEXATLAS_COUNT), 
						sample_scale_and_offset(frag_texcoord, texture_tileset, offset_secondary, 1.0/6.0, TEXATLAS_COUNT), frag_blendweights.x);
	vec4 color_scaled_2 = mix(sample_scale_and_offset(frag_texcoord, texture_tileset, offset, 1.0/12.0, TEXATLAS_COUNT), 
						sample_scale_and_offset(frag_texcoord, texture_tileset, offset_secondary, 1.0/12.0, TEXATLAS_COUNT), frag_blendweights.x);

	vec4 colort1 = mix(color_original, color_scaled_1, 0.3);
	vec4 colort2 = mix(colort1, color_scaled_2, 0.3);

	gl_FragColor = 3.0 * vec4(1, 1, 1, 0) * intensity *
					 colort2;

#endif
}