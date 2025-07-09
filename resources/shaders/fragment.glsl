#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ballTexture;

void main(){
	vec4 texColor = texture(ballTexture, TexCoord);
	FragColor = texColor;
}