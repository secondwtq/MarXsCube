// uniform mat4 gl_ModelViewMatrix;
// uniform mat4 gl_ProjectionMatrix;
// uniform mat4 gl_ModelViewProjectionMatrix;
// attribute vec4 gl_Vertex;

varying vec2 frag_texcoord;

attribute vec3 position;
attribute vec3 texcoord;

uniform sampler2D texture_main;

void main() {

	frag_texcoord = texcoord.xy;

	vec4 position_4 = vec4(position.xyz, 1);

	gl_Position = gl_ModelViewProjectionMatrix * position_4; // ftransform();
}