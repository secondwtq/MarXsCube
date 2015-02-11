// attribute vec4 gl_Color;

varying vec2 frag_texcoord;

uniform sampler2D texture_main;

void main() {

	vec4 t = texture2D(texture_main, frag_texcoord);

	gl_FragColor = t;

}