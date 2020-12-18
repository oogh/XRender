#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
out vec2 realTexCoord;

uniform sampler2D texture1;

void main() {
    realTexCoord = vec2(TexCoord.x, 1.0 - TexCoord.y);
	FragColor = texture(texture1, realTexCoord);
}
