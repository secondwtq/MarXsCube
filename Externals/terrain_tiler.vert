// uniform mat4 gl_ModelViewMatrix;
// uniform mat4 gl_ProjectionMatrix;
// uniform mat4 gl_ModelViewProjectionMatrix;
// attribute vec4 gl_Vertex;

varying float intensity;

attribute vec3 position;

void main() {

	vec3 light_dir = vec3(1, 0, 0);
	// intensity = dot(light_dir, gl_Normal);
	// intensity = 1.0;

	vec4 position_4 = vec4(position.xyz, 1);

	gl_Position = gl_ModelViewProjectionMatrix * position_4; // ftransform();
	// gl_Position = ftransform();
}