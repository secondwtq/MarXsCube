// attribute vec4 gl_Color;

varying vec3 frag_normal;
varying vec3 frag_light_dir;
varying vec3 frag_texcoord;

uniform sampler2D s_texture_main;

void main() {

	float intensity = dot(frag_light_dir, frag_normal);

	gl_FragColor = vec4(1, 1, 1, 0) * intensity * texture2D(s_texture_main, frag_texcoord.xy);
}