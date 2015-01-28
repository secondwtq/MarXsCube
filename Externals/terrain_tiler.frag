// attribute vec4 gl_Color;

#define LIGHTING_ONLY 0

#define TEXATLAS_COUNT (8.0)

varying vec3 frag_normal;
varying vec3 frag_light_dir;
varying vec2 frag_texcoord;
varying float frag_blendweight;
varying vec2 frag_height_texcoord;
varying float frag_texture_index;

uniform sampler2D s_texture_main;
uniform sampler2D s_texture_second;
uniform sampler2D s_texture_heightfield;

uniform sampler2D s_texture_tileset;

void main() {

	float intensity = dot(frag_light_dir, frag_normal);

	vec2 texcoord_org = frag_texcoord.xy;
	vec2 texcoord_scaled = texcoord_org / 30.0;
	vec2 texcoord_scaled_2 = fract(vec2(texcoord_org.y, texcoord_org.x) / 15.0);
	texcoord_org = fract(frag_texcoord.xy);
	texcoord_scaled = fract(texcoord_scaled.xy);
	texcoord_scaled_2 = fract(texcoord_scaled_2.xy);

	float _texture_id = frag_texture_index / TEXATLAS_COUNT;
	vec2 offset_tile = vec2(fract(_texture_id)*TEXATLAS_COUNT, floor(_texture_id));
	vec2 offset = offset_tile / TEXATLAS_COUNT;

	vec2 texcoord_atlas = fract(texcoord_org) / TEXATLAS_COUNT + offset;
	vec2 texcoord_atlas_scaled = fract(texcoord_scaled) / TEXATLAS_COUNT + offset;
	vec2 texcoord_atlas_scaled_2 = fract(texcoord_scaled_2) / TEXATLAS_COUNT + offset;

#if LIGHTING_ONLY == 1
	gl_FragColor = vec4(1, 1, 1, 0) * intensity;
#else
	gl_FragColor = 12.0 * vec4(1, 1, 1, 0) * intensity * //texture2D(s_texture_main, texcoord_scaled) * texture2D(s_texture_main, texcoord_scaled_2) *
					texture2D(s_texture_tileset, texcoord_atlas) * texture2D(s_texture_tileset, texcoord_atlas_scaled)
					* texture2D(s_texture_tileset, texcoord_atlas_scaled_2);
				// mix(texture2D(s_texture_main, texcoord_org), texture2D(s_texture_second, texcoord_org), frag_blendweight) * 32.0;
#endif
}