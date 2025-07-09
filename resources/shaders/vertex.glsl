#version 330 core
layout (location = 0) in vec2 aPos;      // Position of the quad vertex
layout (location = 1) in vec2 aCenter;   // Center of the ball instance
layout (location = 2) in float aRadius;  // Radius of the ball instance

out vec2 TexCoord; // Output texture coordinate for the fragment shader


uniform float aspectRatio;

void main() {
    // Scale and translate the quad based on instance data
    vec2 scaled = aPos * aRadius * 2.0f; // Removed the factor of 2, as each quad vertex is already in the range for ball size
    vec2 worldPos = aCenter + scaled; // Position in world coordinates
    gl_Position = vec4(worldPos.x * aspectRatio, worldPos.y, 0.0f, 1.0f); // Set the gl_Position
    TexCoord = aPos + 0.5; // Adjust TexCoord because your quad's coordinates range from -0.5 to +0.5
}
