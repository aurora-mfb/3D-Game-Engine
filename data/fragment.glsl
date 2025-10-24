
uniform sampler2D texSampler;

varying vec2 ftex;
varying vec2 fcolor;

void main() {
	gl_FragColor = texture2D(texSampler,ftex);
}
