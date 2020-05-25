#pragma once

#include <vector>
#include <functional>
#include <type_traits>

namespace glb
{
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
        static_assert(std::is_member_function_pointer_v<decltype(&Derived::openGlLazyInit)>,
                      "Subclasses must implement a method \"openGlLazyInit()\"!");

        OpenGlLazyInitializer();
    };
}
