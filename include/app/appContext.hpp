#pragma once

#include "core/camera.hpp"
#include <glm/glm.hpp>
#include "core/Shader.h"
#include <GLFW/glfw3.h>

class AppContext {
public:

    int windowWidth, windowHeight;

    //Camera camera = Camera(glm::vec3(0.0f, 0.0f, 2.0f));
    //glm::mat4 projection;
    Shader ballShader;
    Shader bgShader;

    float aspectRatio;

    glm::vec2 mousePos;
    glm::vec2 prevMousePos;
    glm::vec2 mouseDelta;
    bool firstMouse = true;

    bool leftClick = false;
    bool rightClick = false;

    float deltaTime = 0.0f;
    float lastFrame;
    unsigned int codepoint = 0;
    bool typing = false;

    // Input state tracking
    struct KeyFlags {
        bool U = false, Up = false, U2 = false;
        bool D = false, Dp = false, D2 = false;
        bool L = false, Lp = false, L2 = false;
        bool R = false, Rp = false, R2 = false;
        bool F = false, Fp = false, F2 = false;
        bool B = false, Bp = false, B2 = false;
        bool M = false, Mp = false, M2 = false;
        bool UP = false, DOWN = false, LEFT = false, RIGHT = false;
        bool s = false, zero = false, nine = false;
        bool backspace = false;

        // Previous state
        bool prevU = false, prevD = false, prevL = false,
            prevR = false, prevF = false, prevB = false, prevM = false,
            prevUP = false, prevDOWN = false, prevLEFT = false, prevRIGHT = false,
            prevS = false, prevZero = false, prevNine = false,
            prevBackspace = false;

    } input;

    float fixedDT = 1.f / 60.f;

    void createShaders() {
        ballShader = Shader(RESOURCES_PATH"shaders/vertex.glsl", RESOURCES_PATH"shaders/fragment.glsl");
        bgShader = Shader(RESOURCES_PATH"shaders/bg.vert", RESOURCES_PATH"shaders/bg.frag");
    }

    void updateProjection() {
        aspectRatio = (float)windowHeight / (float)windowWidth;
        //projection = glm::ortho(0, windowWidth, 0, windowHeight);
    }

    void updateDeltaTime() {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    float getFixedDeltaTime() const {
        return fixedDT;
    }
};

