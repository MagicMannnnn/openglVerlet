#pragma once

#include <glad/glad.h>
#include <app/appContext.hpp>

class Backround {
public:
	Backround() {
        float vertices[] = {
    -1.0f, -1.0f, // Bottom left
     1.0f, -1.0f, // Bottom right
     1.0f,  1.0f, // Top right
    -1.0f,  1.0f  // Top left
        };

        unsigned int indices[] = {
            0, 1, 2, // First triangle
            0, 2, 3  // Second triangle
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Bind VAO
        glBindVertexArray(VAO);

        // Bind VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Bind EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Specify vertex attributes
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glad_glEnableVertexAttribArray(0);

        // Unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
	}

    void draw(AppContext& ctx) const{
        ctx.bgShader.use();
        ctx.bgShader.setFloat("aspectRatio", ctx.aspectRatio);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }


private:
    unsigned int VAO, EBO, VBO;
};