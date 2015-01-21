// attribute vec4 gl_Color;

varying float intensity;

void main() {
	gl_FragColor = vec4(1, 1, 1, 0) * intensity;
}