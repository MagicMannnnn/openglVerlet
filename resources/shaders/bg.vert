#version 330 core
layout(location = 0) in vec2 aPos;
out vec3 vertexColor;

uniform float aspectRatio;

void main() {
    gl_Position = vec4(aPos.x * aspectRatio, aPos.y, 0.0, 1.0);
    vertexColor = vec3(1, 1, 1); // Pass the color to the fragment shader
}