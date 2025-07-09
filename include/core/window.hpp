#pragma once

#include <glad/glad.h>  
#include <GLFW/glfw3.h>
#include <string>
#include "app/appContext.hpp"


class Window {
public:
    Window(AppContext& ctx, const std::string& title);
    ~Window();

    void pollEvents();
    void swapBuffers();
    bool shouldClose() const;
    void processInput();
    void clear() const;

private:
    GLFWwindow* m_window = nullptr;
    AppContext& context;

    void setupCallbacks();

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void charCallback(GLFWwindow* window, unsigned int codepoint);

    
};

