// attribute vec4 gl_Color;

varying vec3 frag_normal;
varying vec3 frag_light_dir;
varying vec3 frag_texcoord;
varying float frag_blendweight;

uniform sampler2D s_texture_main;
uniform sampler2D s_texture_second;

void main() {

	float intensity = dot(frag_light_dir, frag_normal);

	gl_FragColor = vec4(1, 1, 1, 0) * intensity *
				mix(texture2D(s_texture_main, frag_texcoord.xy), texture2D(s_texture_second, frag_texcoord.xy), frag_blendweight) * 1.3;
}