
#include <app/texture.hpp>


//#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
//#endif

#include <glad/glad.h>
#include <string>
#include <iostream>

namespace Textures {

	unsigned int texture;

	void loadTexture() {
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// Texture path
		std::string texturePath = RESOURCES_PATH"textures/ball.png";

		int width, height, nrChannels;

		// Load the texture data
		unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			// Upload texture data
			glTexImage2D(GL_TEXTURE_2D, 0, (nrChannels == 4) ? GL_RGBA : GL_RGB, width, height, 0, (nrChannels == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);


			// Set texture wrapping and filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Generate mipmaps
			glGenerateMipmap(GL_TEXTURE_2D);

			// Free the image memory
			stbi_image_free(data);
		}
		else {
			std::cerr << "Failed to load texture: " << texturePath << std::endl;
		}

		// Unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}