#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <vector>
#include <string>

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
     * The WindowCreateInfo structure is passed as an argument to Window::create()
     * and controls many properties of the window and further functionality
     * of the library.
     * It is not required to specify a WindowCreateInfo, the default values
     * are sensible for simple tests. You'll want to customize these values
     * for more fleshed-out applications.
     *
     * The window also initializes the EventHandler and with it the gl_base
     * event system. If you wish **not** to use this funtionality, disable
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
         *
         * glb always tries to initialize the context with the highest
         * possible OpenGL version. If context creation fails, it will try
         * out lower version until context creation succeeds.
         *
         * It is possible to specify a minimum number for major and minor
         * versions. If these minimums cannot be met, window creation will
         * fail with an exception. The default minimum version is 4.5.
         */
        struct WindowCreateInfo {
            WindowCreateInfo() {} // GCC and Clang can't handle default ctors in nested structs

            size_t width{ DEFAULT_WINDOW_WIDTH };
            size_t height{ DEFAULT_WINDOW_HEIGHT };

            // Give the window a name
            std::string windowName;

            // The minimum accepted OpenGL version. Window creation fails
            // if no OpenGL context with at least this major version can
            // be created.
            int minOpenGlVersionMajor{ DEFAULT_OPENGL_VERSION_MAJOR };

            // The minimum accepted OpenGL version. Window creation fails
            // if no OpenGL context with at least this minor version can
            // be created.
            int minOpenGlVersionMinor{ DEFAULT_OPENGL_VERSION_MINOR };

            // Number of samples in the multisampling buffer in
            // multisampling is enabled
            int sampleCount{ DEFAULT_OPENGL_SAMPLE_COUNT };

            bool resizable{ false };
            bool transparent{ false };
            bool fullscreen{ false };
            bool vsync{ false };

            // A combination of InputModeFlags that control input behaviour
            InputModeFlags inputMode = static_cast<InputModeFlags>(
                InputModeFlags::stickyKeys | InputModeFlags::stickyMouseButtons
            );
            // A CursorMode that controls cursor behaviour
            CursorMode cursorMode{ CursorMode::normal };

            // Start an event handler thread if true. Setting this to false
            // disabled the event handler and thus the glb event system.
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
        static void create(const WindowCreateInfo& data = {});

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
         * @return bool True if an OpenGL context has been created. This is
         *              true once Window::create() has been called the first
         *              time. Stays true until the program terminates.
         */
        static bool isContextCreated();

    private:
        // Internal GLFW callbacks
        static void initCallbacks();

        static void makeContextCurrent();
        static inline bool contextCreated{ false };

        static inline GLFWwindow* window{ nullptr };
        static inline ivec2 sizePixels;
        static inline bool _isOpen{ false };
        static inline bool _isFullscreen{ false };
    };



    // ------------------------ //
    //      Window events       //
    // ------------------------ //

    /**
     * @brief Base class for all window events
     */
    class WindowEvent : public Event {};

    /**
     * @brief Signals that a window has been created
     *
     * Dispatched whenever a new window is created.
     */
    class WindowCreateEvent : public WindowEvent {};

    /**
     * @brief Signals that a window has been closed
     *
     * Dispatched whenever a window is closed
     */
    class WindowCloseEvent : public WindowEvent {};

    /**
     * @brief Singals that a window has been resized
     *
     * Dispatched whenever a window is resized. This might occur because
     * Window::resize() is called, or the window is resized by the window
     * manager.
     *
     * @property ivec2 oldSize The window size before it was resized.
     * @property ivec2 newSize This is the same as the result of a call to
     *                         Window::getSizePixels().
     */
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
