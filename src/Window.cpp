#include "Window.h"

#include <stdexcept>
#include <iostream>

#include "event/EventHandler.h"
#include "LazyInitializer.h"



void initGLFW()
{
    // Init GLFW
    if (glfwInit() == 0)
    {
        const char* error;
        glfwGetError(&error);
        std::cout << "--- Failed to initialze GLFW!\n";
        throw std::runtime_error("Failed to initialize GLFW: " + std::string(error));
    }

    std::cout << "--- GLFW initialized.\n";
}

void initGLEW()
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW!\n";
        throw std::runtime_error("Failed to initialize GLEW!\n");
    }
    glGetError(); // Get the first error that always occurs.

    std::cout << "--- GLEW initialized.\n";
}

GLFWwindow* createWindow(const glb::Window::WindowCreateInfo& data)
{
    // Create window
    int currentMajorVersion{ glb::DEFAULT_OPENGL_VERSION_MAJOR };
    int currentMinorVersion{ glb::DEFAULT_OPENGL_VERSION_MINOR };

    GLFWwindow* window{ nullptr };
    while (window == nullptr
        && currentMajorVersion >= data.minOpenGlVersionMajor
        && currentMinorVersion >= data.minOpenGlVersionMinor)
    {
        glfwWindowHint(GLFW_SAMPLES, data.sampleCount);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, data.minOpenGlVersionMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, data.minOpenGlVersionMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, data.resizable);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, data.transparent);
        glfwWindowHint(GLFW_VISIBLE, true);
        glfwWindowHint(GLFW_FOCUSED, true);

        // Enable to have window always on top
        // glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

        window = glfwCreateWindow(
            static_cast<int>(data.width),
            static_cast<int>(data.height),
            data.windowName.c_str(),
            nullptr,
            nullptr
        );
        if (window == nullptr)
        {
            // Calculate new version
            currentMinorVersion--;
            if (currentMinorVersion < 0)
                currentMajorVersion--;

            // Silence warnings
            constexpr int _6 = 6;
            constexpr int _5 = 5;
            constexpr int _3 = 3;

            switch (currentMajorVersion)
            {
            case 4:
                if (currentMinorVersion < 0)
                    currentMinorVersion = _6;
                break;
            case 3:
                if (currentMinorVersion < 0)
                    currentMinorVersion = _3;
                break;
            case 2:
                if (currentMinorVersion < 0)
                    currentMinorVersion = 1;
                break;
            case 1:
                if (currentMinorVersion < 0)
                    currentMinorVersion = _5;
                break;
            case 0:
                break;
            }
        }
    }

    // Window is still nullptr if the specified major and minor version could not be provided
    if (window == nullptr)
    {
        std::cout << "The specified OpenGL requirements major-version = "
                  << data.minOpenGlVersionMajor << " and minor-version "
                  << data.minOpenGlVersionMinor << " could not be met.";
        glfwTerminate();
        throw std::runtime_error("Window creation failed: OpenGL version not supported.");
    }

    std::cout << "--- OpenGL context created with version " << currentMajorVersion
              << "." << currentMinorVersion << "\n";

    return window;
}

void glb::Window::initCallbacks()
{
    glfwSetKeyCallback(window, [](GLFWwindow*, int key, int /*scancode*/, int action, int mods) {
        if (action == static_cast<int>(eInputAction::press))
        {
            EventHandler::notify(
                std::make_unique<KeyPressEvent>(
                    static_cast<eKey>(key),
                    static_cast<eKeyMod>(mods)
            ));
        }
        if (action == static_cast<int>(eInputAction::release))
        {
            EventHandler::notify(
                std::make_unique<KeyReleaseEvent>(
                    static_cast<eKey>(key),
                    static_cast<eKeyMod>(mods)
            ));
        }
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow*, int button, int action, int mods) {
        if (action == static_cast<int>(eInputAction::press))
        {
            EventHandler::notify(
                std::make_unique<MouseButtonPressEvent>(
                    static_cast<eMouseButton>(button),
                    static_cast<eKeyMod>(mods)
            ));
        }
        if (action == static_cast<int>(eInputAction::release))
        {
            EventHandler::notify(
                std::make_unique<MouseButtonReleaseEvent>(
                    static_cast<eMouseButton>(button),
                    static_cast<eKeyMod>(mods)
            ));
        }
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow*, double xpos, double ypos) {
        EventHandler::notify(std::make_unique<MouseMoveEvent>(vec2(xpos, ypos)));
    });

    glfwSetWindowCloseCallback(window, [](GLFWwindow*) {
        close();
    });

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
        ivec2 oldSize = sizePixels;
        sizePixels = ivec2(width, height);
        EventHandler::notify(std::make_unique<WindowResizeEvent>(oldSize, sizePixels));
    });

    glfwSetScrollCallback(window, [](GLFWwindow*, double xOffset, double yOffset) {
        EventHandler::notify(std::make_unique<MouseScrollEvent>(vec2(xOffset, yOffset)));
    });

    std::cout << "--- Event handler initialized.\n";
}

void glb::Window::create(const WindowCreateInfo& data)
{
    if (_isOpen) return;

    // First, init GLFW
    initGLFW();

    // Create and init window
    window = createWindow(data);
    glfwSetInputMode(window, GLFW_STICKY_KEYS,
                     data.inputMode & InputModeFlags::stickyKeys);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS,
                     data.inputMode & InputModeFlags::stickyMouseButtons);
    glfwSetInputMode(window, GLFW_CURSOR,
                     static_cast<int>(data.cursorMode));

    // Vsync must be set after the window is created, idk why
    makeContextCurrent();
    if (data.vsync) {
        glfwSwapInterval(SWAP_INTERVAL_VSYNC_ENABLED);
    }
    else {
        glfwSwapInterval(SWAP_INTERVAL_VSYNC_DISABLED); // Interval == 0 -> disable vsync
    }

    // Initialize additional resources
    // GLEW must be initialized after an OpenGL context (aka. the window) has been created.
    initGLEW();
    contextCreated = true;

    if (data.useEventHandler == true) {
        EventHandler::init();
    }
    initCallbacks();

    // Call lazy initializers
    OpenGlLazyInit::initAll();

    // Poll events once to make the window responsive
    pollEvents();

    sizePixels = ivec2(data.width, data.height);
    _isOpen = true;
    EventHandler::notify(std::make_unique<WindowCreateEvent>());
    std::cout << "--- Window created successfully.\n";

    // Resize the window to the actual framebuffer size.
    // Tiling window managers for example might resize the window
    // forcefully, in which case the resize is not recognized as
    // such.
    ivec2 framebufferSize;
    glfwGetFramebufferSize(window, &framebufferSize.x, &framebufferSize.y);
    resize(framebufferSize);
}

void glb::Window::close()
{
    if (!_isOpen) return;

    _isOpen = false;
    EventHandler::notify(std::make_unique<WindowCloseEvent>());
    glfwDestroyWindow(window);
}

auto glb::Window::getGlfwWindow() -> GLFWwindow*
{
    return window;
}

void glb::Window::swapBuffers()
{
    glfwSwapBuffers(window);
}

void glb::Window::pollEvents()
{
    glfwPollEvents();
}

void glb::Window::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void glb::Window::updateViewport()
{
    glViewport(0, 0, sizePixels.x, sizePixels.y);
}

ivec2 glb::Window::getSizePixels()
{
    return sizePixels;
}

void glb::Window::resize(ivec2 newSizePixels)
{
    glfwSetWindowSize(window, newSizePixels.x, newSizePixels.y);

    // I was too lazy to extract this piece of code into a function, I just copy-pasted
    // it from initCallbacks(). Appearently, glfwSetWindowSize() does not call my callbacks.
    ivec2 oldSize = sizePixels;
    sizePixels = ivec2(newSizePixels.x, newSizePixels.y);
    updateViewport();
    EventHandler::notify(std::make_unique<WindowResizeEvent>(oldSize, sizePixels));
}

bool glb::Window::isOpen()
{
    return _isOpen;
}

bool glb::Window::isContextCreated()
{
    return contextCreated;
}

void glb::Window::makeContextCurrent()
{
    glfwMakeContextCurrent(window);
}
