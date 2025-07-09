#include "core/window.hpp"
#include <stdexcept>

Window::Window(AppContext& ctx, const std::string& title)
    : context(ctx) {
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); 

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (!mode)
        throw std::runtime_error("Failed to get video mode");

    context.windowWidth = mode->width;
    context.windowHeight = mode->height;

    // Create undecorated window the size of the monitor
    m_window = glfwCreateWindow(context.windowWidth, context.windowHeight, title.c_str(), nullptr, nullptr);
    if (!m_window)
        throw std::runtime_error("Failed to create GLFW window");

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, &context);

    setupCallbacks();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD");

    glEnable(GL_DEPTH_TEST);

    context.createShaders();
}



Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::swapBuffers() {
    glfwSwapBuffers(m_window);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::clear() const {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::setupCallbacks() {
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    glfwSetCursorPosCallback(m_window, mouseCallback);
    glfwSetScrollCallback(m_window, scrollCallback);
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
    glfwSetCharCallback(m_window, charCallback);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    auto* ctx = static_cast<AppContext*>(glfwGetWindowUserPointer(window));
    if (!ctx) return;

    ctx->windowWidth = width;
    ctx->windowHeight = height;

    glViewport(0, 0, width, height);

    ctx->updateProjection();
}


void Window::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    auto* ctx = static_cast<AppContext*>(glfwGetWindowUserPointer(window));
    if (!ctx) return;

    ctx->prevMousePos = ctx->mousePos;
    ctx->mousePos = glm::vec2(xpos, ypos);
    ctx->mouseDelta = ctx->mousePos - ctx->prevMousePos;
}

void Window::scrollCallback(GLFWwindow* window, double /*xoffset*/, double yoffset) {
    auto* ctx = static_cast<AppContext*>(glfwGetWindowUserPointer(window));
    if (!ctx) return;

    //ctx->camera.ProcessMouseScroll(static_cast<float>(yoffset), ctx->deltaTime);
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto* ctx = static_cast<AppContext*>(glfwGetWindowUserPointer(window));
    if (!ctx) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS && ctx->firstMouse) {
            ctx->leftClick = true;
            ctx->firstMouse = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        if (action == GLFW_RELEASE) {
            ctx->leftClick = false;
            ctx->firstMouse = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && !ctx->leftClick) {
        if (action == GLFW_PRESS) {
            ctx->rightClick = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        if (action == GLFW_RELEASE) {
            ctx->rightClick = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void Window::charCallback(GLFWwindow* window, unsigned int codepoint) {
    auto* ctx = static_cast<AppContext*>(glfwGetWindowUserPointer(window));
    if (!ctx) return;

    ctx->codepoint = codepoint;
    ctx->typing = true;
}

void Window::processInput() {
    auto& input = context.input;

    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);

    // Reset move flags each frame
    input.U = input.Up = input.U2 = false;
    input.D = input.Dp = input.D2 = false;
    input.L = input.Lp = input.L2 = false;
    input.R = input.Rp = input.R2 = false;
    input.F = input.Fp = input.F2 = false;
    input.B = input.Bp = input.B2 = false;
    input.M = input.Mp = input.M2 = false;
    input.UP = input.DOWN = input.LEFT = input.RIGHT = false;
    input.s = false;
    input.zero = false;
    input.nine = false;
    input.backspace = false;

    struct MoveDef {
        int key;
        bool& prev, & single, & prime, & dbl;
    };

    struct NonMoveDef {
        int key;
        bool& prev, & move;
    };

    static MoveDef moves[] = {
        { GLFW_KEY_U, input.prevU, input.U, input.Up, input.U2 },
        { GLFW_KEY_D, input.prevD, input.D, input.Dp, input.D2 },
        { GLFW_KEY_L, input.prevL, input.L, input.Lp, input.L2 },
        { GLFW_KEY_R, input.prevR, input.R, input.Rp, input.R2 },
        { GLFW_KEY_F, input.prevF, input.F, input.Fp, input.F2 },
        { GLFW_KEY_B, input.prevB, input.B, input.Bp, input.B2 },
        { GLFW_KEY_M, input.prevM, input.M, input.Mp, input.M2 }
    };

    static NonMoveDef nonMoves[] = {
        { GLFW_KEY_LEFT, input.prevLEFT, input.LEFT },
        { GLFW_KEY_RIGHT, input.prevRIGHT, input.RIGHT },
        { GLFW_KEY_DOWN, input.prevDOWN, input.DOWN },
        { GLFW_KEY_UP, input.prevUP, input.UP },
        { GLFW_KEY_S, input.prevS, input.s },
        { GLFW_KEY_0, input.prevZero, input.zero },
        { GLFW_KEY_9, input.prevNine, input.nine },
        { GLFW_KEY_BACKSPACE, input.prevBackspace, input.backspace }
    };

    // Process moves (U, D, L, R, etc.)
    for (auto& m : moves) {
        bool curr = (glfwGetKey(m_window, m.key) == GLFW_PRESS);
        // If just pressed this frame:
        if (curr && !m.prev) {
            bool prime = (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
            bool dbl = (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS);
            if (dbl)   m.dbl = true;
            else if (prime) m.prime = true;
            else            m.single = true;
        }
        m.prev = curr;
    }

    // Process non-move actions (LEFT, RIGHT, UP, DOWN, etc.)
    for (auto& r : nonMoves) {
        bool curr = (glfwGetKey(m_window, r.key) == GLFW_PRESS);
        // If just pressed this frame:
        if (curr && !r.prev) {
            r.move = true;
        }
        r.prev = curr;
    }
}


