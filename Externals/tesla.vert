// uniform mat4 gl_ModelViewMatrix;
// uniform mat4 gl_ProjectionMatrix;
// uniform mat4 gl_ModelViewProjectionMatrix;
// attribute vec4 gl_Vertex;

#define USE_HEIGHTFIELD 1

#define TEXATLAS_COUNT (8.0)

varying vec3 frag_normal;
varying vec3 frag_light_dir;
varying vec2 frag_texcoord;
varying vec2 frag_height_texcoord;
varying vec3 frag_blendweights;
varying vec3 frag_texture_indexes;

attribute vec3 position;
attribute vec3 normal;
attribute vec3 texcoord;
attribute vec3 blendweights;
attribute vec3 texindexes;

uniform sampler2D texture_main;
uniform sampler2D texture_second;
uniform sampler2D texture_heightfield;
uniform sampler2D texture_tileset;

// solution fram
//		http://stackoverflow.com/questions/5281261/generating-a-normal-map-from-a-height-map
vec4 parse_heightfield(in sampler2D heightfield_sampler, in vec2 texcoord) {
	const vec2 size = vec2(2.0,0.0);
	const vec3 off = vec3(-1.0,0.0,1.0) * (1.0/30.0);

	vec4 height = texture2D(heightfield_sampler, texcoord);

    float s11 = height.x;
    float s01 = texture2D(heightfield_sampler, texcoord+off.xy).x;
    float s21 = texture2D(heightfield_sampler, texcoord+off.zy).x;
    float s10 = texture2D(heightfield_sampler, texcoord+off.yx).x;
    float s12 = texture2D(heightfield_sampler, texcoord+off.yz).x;

    vec3 va = normalize(vec3(size.xy,s21-s01));
    vec3 vb = normalize(vec3(size.yx,s12-s10));

    return vec4( cross(va,vb), s11 );
}

vec2 texatlas_offset(in float index, in float count) {
	float _texture_id = index / count;
	vec2 offset_tile = vec2(fract(_texture_id) * count, floor(_texture_id));
	return vec2(offset_tile / count);
}

void main() {

	vec3 light_dir = vec3(1.5, 1.5, 1);

	frag_normal = normalize(normal);
	frag_light_dir = normalize(light_dir);
	frag_texcoord = texcoord.xy;
	frag_height_texcoord = texcoord.xy / 30.0;
	frag_blendweights = blendweights;

	frag_texture_indexes = texindexes;

	// frag_texcoord = texatlas_offset(frag_texture_index.x, TEXATLAS_COUNT) + fract(frag_texcoord) / TEXATLAS_COUNT;

	vec4 position_4 = vec4(position.xyz, 1);

#if USE_HEIGHTFIELD == 1
    vec4 height = parse_heightfield(texture_heightfield, frag_height_texcoord);
    frag_normal = normalize(height.xyz);
	position_4.z += height.w * 128.0;
#endif
	// position_4.z += texture2D(s_texture_heightfield, frag_height_texcoord).x * 128.0;

	gl_Position = gl_ModelViewProjectionMatrix * position_4; // ftransform();
	// gl_Position = ftransform();
}