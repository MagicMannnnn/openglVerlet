#pragma once

#include "Shader.h"
#include "camera.hpp"

class Renderable {
public:
    virtual void render(Shader& shader, Camera& camera) const = 0; // Pure virtual function
    virtual ~Renderable() = default; // Always have virtual destructor
};