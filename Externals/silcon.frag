// attribute vec4 gl_Color;

varying vec2 frag_texcoord;

uniform sampler2D texture_main;
uniform vec4 color_multiply;

void main() {

	vec4 t = texture2D(texture_main, frag_texcoord);

	gl_FragColor = t * color_multiply;

}