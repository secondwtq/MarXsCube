// uniform mat4 gl_ModelViewMatrix;
// uniform mat4 gl_ProjectionMatrix;
// uniform mat4 gl_ModelViewProjectionMatrix;
// attribute vec4 gl_Vertex;

varying vec3 frag_normal;
varying vec3 frag_light_dir;
varying vec3 frag_texcoord;

attribute vec3 position;
attribute vec3 s_normal;
attribute vec3 s_texcoord;

uniform sampler2D s_texture_main;

void main() {

	vec3 light_dir = vec3(1, 1, 3);

	frag_normal = normalize(s_normal);
	frag_light_dir = normalize(light_dir);
	frag_texcoord = s_texcoord;

	vec4 position_4 = vec4(position.xyz, 1);

	gl_Position = gl_ModelViewProjectionMatrix * position_4; // ftransform();
	// gl_Position = ftransform();
}