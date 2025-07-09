#pragma once
#include <glm/glm.hpp>
#include <string>
#include <iostream>

enum TextureIndex {
    WHITE = 0
};

const float G = -0.8f;

const float coeff = 0.5f;

struct Ball {
    glm::vec2 pos = { -10.f, 10.f };
    float r = 0.f;
    glm::vec2 prevPos;
    Ball() = default;
    Ball(glm::vec2 _pos, float _r) : pos(_pos), prevPos(_pos), r(_r) {

    }

    Ball(glm::vec2 _pos, glm::vec2 _prevPos, float _r) : pos(_pos), prevPos(_prevPos), r(_r) {

    }

    void update(float dt) {
        // Calculate velocity based on current and previous positions
        glm::vec2 velocity = pos - prevPos;

        // Apply constraints (like gravity, drag, etc.) if necessary
        glm::vec2 acceleration = glm::vec2(0.f, G); // Example: gravity pulling down

        // Update position using Verlet integration
        glm::vec2 newPos = pos + velocity + acceleration * dt * dt;

        // Store the current position as previous position for the next frame
        prevPos = pos;
        pos = newPos;


        if (pos.y - r < -1.f) {
            float boundary = -1.f + r;
            float diff = std::abs(boundary - pos.y);
            float diff2 = std::abs(boundary - prevPos.y);
            pos.y = boundary + diff * coeff;
            prevPos.y = boundary - diff2;
        }
        else if (pos.y + r > 1.f) {
            float boundary = 1.f - r;
            float diff = std::abs(boundary - pos.y);
            float diff2 = std::abs(boundary - prevPos.y);
            pos.y = boundary - diff * coeff;
            prevPos.y = boundary + diff2;
        }

        
        if (pos.x - r < -1.f) {
            float boundary = -1.f + r;
            float diff = std::abs(boundary - pos.x);
            float diff2 = std::abs(boundary - prevPos.x);
            pos.x = boundary + diff * coeff;
            prevPos.x = boundary - diff2;
        }
        else if (pos.x + r > 1.f) {
            float boundary = 1.f - r;
            float diff = std::abs(boundary - pos.x);
            float diff2 = std::abs(boundary - prevPos.x);
            pos.x = boundary - diff * coeff;
            prevPos.x = boundary + diff2;
        }
    }

    void collide(Ball& other) {
        glm::vec2 delta = pos - other.pos;
        float distance = glm::length(delta);

        // Check for collision
        if (distance < (r + other.r)) {
            // Normalize the delta vector
            delta /= distance;

            // Calculate the overlap distance
            float overlap = (r + other.r) - distance;

            // Adjust the positions of the balls based on the overlap
            pos += delta * (overlap * 0.5f); // Push the current ball out
            other.pos -= delta * (overlap * 0.5f); // Push the other ball out
        }
    }
    
};
