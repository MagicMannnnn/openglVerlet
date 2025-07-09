#pragma once
#include <app/ball.hpp>
#include <app/appContext.hpp>

#include <array>

#include <glad/glad.h>
#include <app/texture.hpp>
#include <iostream>

#include <app/cellManager.hpp>


template <int N>
class BallManager {
public:
	BallManager() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &quadVBO);
		glGenBuffers(1, &EBO);
		glGenBuffers(1, &instanceVBO);

		glBindVertexArray(VAO);

		// Setup quad VBO
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Setup Element Buffer Object
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

		// Setup instance VBO
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, N * sizeof(Ball), nullptr, GL_DYNAMIC_DRAW); // Create a buffer without data

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Ball), (void*)0); // Assuming Ball has glm::vec2 as the first member
		glEnableVertexAttribArray(1);
		glVertexAttribDivisor(1, 1); // 1 means to update for each instance

		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Ball), (void*)(offsetof(Ball, r)));
		glEnableVertexAttribArray(2);
		glVertexAttribDivisor(2, 1); // 1 means to update for each instance

		Textures::loadTexture();

		std::cout << (0.0 / 2.f) * 10;
	}


	void update(float dt) {

		m_cellManager.makeCells(m_balls, m_lastBallPointer);
		m_cellManager.collideCells(m_balls, dt);

		/*
		for (int i = 0; i <= m_lastBallPointer; i++) {
			m_balls[i].update(dt);
			for (int j = i+1; j <= m_lastBallPointer; j++) {
				m_balls[i].collide(m_balls[j]);

			}
		}*/


		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, N * sizeof(Ball), &m_balls[0]);
	}

	void draw(AppContext& ctx) const {

		ctx.ballShader.use();
		ctx.ballShader.setInt("ballTexture", 0);
		ctx.ballShader.setFloat("aspectRatio", ctx.aspectRatio);

		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Textures::texture);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, N);
	}

	void addBall(const glm::vec2& center, float radius) {
		if (m_lastBallPointer == N-1) return;
		m_lastBallPointer++;
		m_balls[m_lastBallPointer] = Ball(center, radius);
	}

	void addBall(const glm::vec2& center, const glm::vec2& Pcenter, float radius) {
		if (m_lastBallPointer == N - 1) return;
		m_lastBallPointer++;
		m_balls[m_lastBallPointer] = Ball(center, Pcenter, radius);
	}

private:
	std::array<Ball, N> m_balls;
	int m_lastBallPointer = -1;

	const std::array<float, 8> quad = {
	   -0.5f, -0.5f,
	   0.5f, -0.5f,
	   0.5f, 0.5f,
	   -0.5f, 0.5f
	};

	const std::array<unsigned int, 6> indices = {
		0, 1, 2,
		2, 3, 0
	};
	unsigned int quadVBO, VAO, EBO, instanceVBO;

	CellManager<N, 10, 10, N> m_cellManager;
	

};

