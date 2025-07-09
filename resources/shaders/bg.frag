#version 330 core
in vec3 vertexColor;
out vec4 fragColor;
void main() {
    fragColor = vec4(vertexColor, 0.2); // Set the fragment color
}