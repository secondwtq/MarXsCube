uniform sampler2D texture;

//	TODO: modified fm. MarKsCube_Mac
uniform vec4 colorMultiply;

void main() {
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // multiply it by the color
    gl_FragColor = gl_Color * pixel * colorMultiply;
}