// attribute vec4 gl_Color;

varying vec3 frag_normal;
varying vec3 frag_light_dir;
varying vec3 frag_texcoord;
varying float frag_blendweight;

uniform sampler2D s_texture_main;
uniform sampler2D s_texture_second;

void main() {

	float intensity = dot(frag_light_dir, frag_normal);

	vec2 texcoord_org = frag_texcoord.xy;
	vec2 texcoord_scaled = texcoord_org / 30.0;
	vec2 texcoord_scaled_2 = vec2(texcoord_org.y, texcoord_org.x) / 15.0;

	gl_FragColor = vec4(1, 1, 1, 0) * intensity * texture2D(s_texture_main, texcoord_scaled) * texture2D(s_texture_main, texcoord_scaled_2) *
				mix(texture2D(s_texture_main, texcoord_org), texture2D(s_texture_second, texcoord_org), frag_blendweight) * 32.0;
}