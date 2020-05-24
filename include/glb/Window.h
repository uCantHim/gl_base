#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <unordered_map>
#include <functional>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

#include "event/Event.h"

namespace glb
{
    constexpr size_t DEFAULT_WINDOW_WIDTH = 1920;
    constexpr size_t DEFAULT_WINDOW_HEIGHT = 1080;

    constexpr int DEFAULT_OPENGL_VERSION_MAJOR = 4;
    constexpr int DEFAULT_OPENGL_VERSION_MINOR = 5;
    constexpr int DEFAULT_OPENGL_SAMPLE_COUNT = 4;

    constexpr int SWAP_INTERVAL_VSYNC_DISABLED = 0;
    constexpr int SWAP_INTERVAL_VSYNC_ENABLED = 2;


    /**
     * Provides a static function to initialize statically created objects
     * lazily after an OpenGL constext has been created.
     */
    class OpenGlLazyInit
    {
    public:
        /**
         * @brief Add a function to execute at context creation
         *
         * This can be used to initialize OpenGL objects as soon as possible
         * but still after window creation. Is called in the same thread that
         * the OpenGL context was created.
         *
         * If the window has already been created, the function is called
         * immediately.
         *
         * @param std::function<void(void)> func The function to execute at
         *                                       window creation
         */
        static void addLazyInitializer(std::function<void(void)> func);

    private:
        friend class Window;

        /**
         * Calls all lazy initializers. Called by Window.
         */
        static void initAll();

        static inline std::vector<std::function<void(void)>> lazyInitializers;
    };


    /**
     * @brief Lazy initialization for OpenGL calls
     *
     * Subclasses must implement a method "openGlLazyInit" with the
     * signature std::function<void(void)>.
     *
     * Inheriting from this class and implementing openGlLazyInit promises
     * that the function will be called as soon as possible but after an
     * OpenGL context has been created. It is called in the OpenGL main
     * thread.
     *
     * Instantiating a class derived from this when an OpenGL context
     * already exists calls Derived::openGlLazyInit immediately.
     *
     * @tparam Derived The derived class, CRTP style
     */
    template <class Derived>
    class OpenGlLazyInitializer
    {
    public:
        OpenGlLazyInitializer();
    };


    /**
     * @brief A window. Represents an OpenGL rendering context.
     *
     * This is a static class. That means that there can only ever exist one
     * window.
     *
     * Creating a window is the basic entry point into most functionality of
     * the library. Of course, you can use helper classes like Texture or
     * ShaderProgram with other means of creating OpenGL contexts, for
     * example other libraries or a more low-level approach.
     *
     * Initialize the window with Window::create(), destroy it with
     * Window::close(). Calling Window::create while the window is still alive
     * does nothing. It creates a new window if called after the window has
     * been destroyed.
     * Query whether the window is created with Window::isOpen().
     *
     * The WindowData structure is passed as an argument to Window::create()
     * and controls many properties of the window and further functionality
     * of the library.
     * It is not required to specify a WindowData, the default values are
     * sensible for simple tests. You'll want to customize these values for
     * more fleshed-out applications.
     *
     * The window also initializes the EventHandler and with it the gl_base
     * event system. If you wish not to use this funtionality, disable
     * initialization of the EventHandler by setting the useEventHandler
     * flag in the WindowData passed to Window::create().
     *
     * The window generates various events, for example:
     *
     * - Key presses
     *
     * - Mouse movement/-button presses
     *
     * - Window-specific events like resize or open/close
     *
     * See @ref EventHandler.h "Event Handler" for more information on events.
     */
    class Window
    {
    public:
        /**
         * @brief Controls behaviour of input devies
         *
         * Combine these with OR to enable multiple flags.
         */
        enum InputModeFlags {
            stickyKeys = 0x1,
            stickyMouseButtons = 0x2
        };

        /**
         * @brief Controls cursor behaviour
         */
        enum class CursorMode {
            normal, hidden, disabled
        };

        /**
         * @brief Initialization information for windows
         */
        struct WindowData {
            WindowData() {} // GCC and Clang need this non-defaulted constructor. Not required for MSVC.

            size_t width{ DEFAULT_WINDOW_WIDTH };
            size_t height{ DEFAULT_WINDOW_HEIGHT };
            std::string windowName;

            int minOpenGlVersionMajor{ DEFAULT_OPENGL_VERSION_MAJOR };
            int minOpenGlVersionMinor{ DEFAULT_OPENGL_VERSION_MINOR };
            int sampleCount{ DEFAULT_OPENGL_SAMPLE_COUNT };

            bool resizable{ false };
            bool transparent{ false };
            bool fullscreen{ false };
            bool vsync{ false };

            InputModeFlags inputMode = static_cast<InputModeFlags>(InputModeFlags::stickyKeys | InputModeFlags::stickyMouseButtons);
            CursorMode cursorMode{ CursorMode::normal };

            bool useEventHandler{ true };
        };

        /**
         * @brief Create a window
         *
         * Creates and initializes a new window with the given data.
         * Does nothing if the window has already been created.
         *
         * Generates a WindowCreateEvent.
         *
         * @param WindowData data Initialization data
         */
        static void create(const WindowData& data = {});

        /**
         * @brief Close and destroy the window
         *
         * Generates a WindowCloseEvent.
         *
         * Does nothing if the window has already been destroyed
         */
        static void close();

        /**
         * @return GLFWwindow* The underlying GLFW window. Nullptr is the
         *                     window has not been created.
         */
        static auto getGlfwWindow() -> GLFWwindow*;

        /**
         * @brief Swap back- and front buffer
         *
         * Call this after a frame has been rendered. Swaps front- and back
         * buffer. That means that all rendered framebuffer contents will be
         * shown on the screen.
         */
        static void swapBuffers();

        /**
         * @brief Dispatch events
         *
         * Call this once per frame.
         * Only call this from the main thread.
         */
        static void pollEvents();

        /**
         * @brief Clear the current back buffer
         *
         * Calls glClear() internally to clear the color buffer and the depth
         * buffer of the current back buffer.
         *
         * Only call this from the main thread.
         */
        static void clear();

        /**
         * @brief Resize the OpenGL viewport to the current window size
         *
         * This is useful after you changed the OpenGL viewport (glViewport())
         * for cool algorithms or rendering subpasses. It sets the viewport to
         * the current size of the window.
         */
        static void updateViewport();

        /**
         * @return ivec2 The window size in pixels
         */
        [[nodiscard]]
        static ivec2 getSizePixels();

        /**
         * @brief resize the window
         *
         * @param ivec2 newSizePixels The desired window size in pixels
         */
        static void resize(ivec2 newSizePixels);

        /**
         * @return bool True if the window has been created and is open, false
         *              if it has not been created or has been destroyed
         */
        static bool isOpen();

        /**
         * @return bool True is OpenGL has been initialized. This is true once
         *              Window::create() has been called the first time. Stays
         *              true until the program is terminated.
         */
        static bool isOpenGlInitialized();

    private:
        // Internal GLFW callbacks
        static void initCallbacks();

        static void makeContextCurrent();
        static inline bool _openglInitialized{ false };

        static inline GLFWwindow* window{ nullptr };
        static inline ivec2 sizePixels;
        static inline bool _isOpen{ false };
        static inline bool _isFullscreen{ false };
    };


    class WindowEvent : public Event
    {
    };

    class WindowCreateEvent : public WindowEvent
    {
    };

    class WindowCloseEvent : public WindowEvent
    {
    };

    class WindowResizeEvent : public WindowEvent
    {
    public:
        WindowResizeEvent(ivec2 oldSize, ivec2 newSize)
            : oldSize(oldSize), newSize(newSize)
        {}

        ivec2 oldSize;
        ivec2 newSize;
    };
} // namespace glb

#endif
