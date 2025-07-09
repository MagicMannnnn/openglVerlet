#pragma once

#include <array>
#include <app/ball.hpp>

template<int cellCapacity, int rows, int cols, int Nballs>
class CellManager {
public:
	CellManager() = default;

	void makeCells(std::array<Ball, Nballs>& balls, int lastBallPtr) {

		for (std::array<std::array<int, cellCapacity + 1>, cols>& row : grid) {
			for (std::array<int, cellCapacity + 1>&cell : row) {
				cell.fill(0);
				cell[0] = 1;
			}
				
		}

		if (lastBallPtr < 0) return;

		for (int i = 0; i <= lastBallPtr; i++) {
			Ball& ball = balls[i];

			int x = std::floor((ball.pos.x + 1.f) * cellWidth);
			int y = std::floor((ball.pos.y + 1.f) * cellHeight);

			std::cout << x << ", " << y << "\n";

			grid[y][x][grid[y][x][0]] = i;
			grid[y][x][0]++;
		}
	}

	void collideCells(std::array<Ball, Nballs>& balls, float _dt) {
		dt = _dt;
		
		for (int y = 0; y < rows; y++) {
			for (int x = 0; x < cols; x++) {
				collideCell(grid[y][x], balls);

				if (x > 0) {
					collideAdjacentCell(grid[y][x], grid[y][x - 1], balls);
					if (y > 0) {
						collideAdjacentCell(grid[y][x], grid[y - 1][x - 1], balls);
					}
					if (y < rows-1) {
						collideAdjacentCell(grid[y][x], grid[y + 1][x - 1], balls);
					}
				}
				if (y > 0) {
					collideAdjacentCell(grid[y][x], grid[y - 1][x], balls);
				}
				
			}

		}
	}

	void collideCell(std::array<int, cellCapacity + 1>& cell, std::array<Ball, Nballs>& balls) {
		
		for (int i = 1; i < cell[0]; i++) {
			Ball& ball = balls[cell[i]];
			
			for (int j = i+1; j < cell[0]; j++) {
				Ball& ball2 = balls[cell[j]];
				ball.collide(ball2);
			}
			ball.update(dt);
		}
	}

	void collideAdjacentCell(std::array<int, cellCapacity + 1>& cell, std::array<int, cellCapacity + 1>& cell2, std::array<Ball, Nballs>& balls) {

		for (int i = 1; i < cell[0]; i++) {
			Ball& ball = balls[cell[i]];
			
			for (int j = 1; j < cell2[0]; j++) {
				Ball& ball2 = balls[cell2[i]];
				ball.collide(ball2);
			}

			ball.update(dt);
		}
	}

private:
	std::array<std::array<std::array<int, cellCapacity+1>, cols>, rows> grid;
	float cellWidth = cols / 2.f;
	float cellHeight = rows / 2.f;
	float dt;

};