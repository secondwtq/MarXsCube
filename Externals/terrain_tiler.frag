// attribute vec4 gl_Color;

#define LIGHTING_ONLY 0

varying vec3 frag_normal;
varying vec3 frag_light_dir;
varying vec2 frag_texcoord;
varying float frag_blendweight;
varying vec2 frag_height_texcoord;

uniform sampler2D s_texture_main;
uniform sampler2D s_texture_second;
uniform sampler2D s_texture_heightfield;

void main() {

	float intensity = dot(frag_light_dir, frag_normal);

	vec2 texcoord_org = frag_texcoord.xy;
	vec2 texcoord_scaled = texcoord_org / 30.0;
	vec2 texcoord_scaled_2 = fract(vec2(texcoord_org.y, texcoord_org.x) / 15.0);
	texcoord_org = fract(frag_texcoord.xy);
	texcoord_scaled = fract(texcoord_scaled.xy);
	texcoord_scaled_2 = fract(texcoord_scaled_2.xy);

#if LIGHTING_ONLY == 1
	gl_FragColor = vec4(1, 1, 1, 0) * intensity;
#else
	gl_FragColor = vec4(1, 1, 1, 0) * intensity * texture2D(s_texture_main, texcoord_scaled) * texture2D(s_texture_main, texcoord_scaled_2) *
				mix(texture2D(s_texture_main, texcoord_org), texture2D(s_texture_second, texcoord_org), frag_blendweight) * 32.0;
#endif
}