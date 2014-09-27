uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;

uniform sampler2D textureBlend;

float clampe (float org) {
	return clamp(org, 0.0, 1.0);
}

void main() {

	vec4 pixel0 = texture2D(texture0, gl_TexCoord[0].xy);
	vec4 pixel1 = texture2D(texture1, gl_TexCoord[0].xy);
	vec4 pixel2 = texture2D(texture2, gl_TexCoord[0].xy);
	vec4 pixel3 = texture2D(texture3, gl_TexCoord[0].xy);
	vec4 pixel4 = texture2D(texture4, gl_TexCoord[0].xy);

	vec4 pixelBlend = texture2D(textureBlend, gl_TexCoord[0].xy);

	vec4 frag_color = vec4(0.0, 0.0, 0.0, 0.0);

	float ba = pixelBlend[0], bb = pixelBlend[1], bc = pixelBlend[2],
			 bd = pixelBlend[3], be = 1.0 - (ba+bb+bc+bd);
	clampe(ba);
	clampe(bb);
	clampe(bc);
	clampe(bd);
	clampe(be);

	frag_color = pixel0 * ba + pixel1 * bb + pixel2 * bc + pixel3 * bd;// + pixel4 * be;
	//frag_color *= gl_Color;

	gl_FragColor = frag_color;
}