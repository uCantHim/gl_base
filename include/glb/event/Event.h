#pragma once
#ifndef EVENT_H
#define EVENT_H

#include <type_traits>

namespace glb
{
    /**
     * @brief The base class for all events
     *
     * Events are polymorphic. Whenever you listen to events, you'll want to
     * know which event you're dealing with. Events are differentiated by
     * their type. Use Event::is() and event::to() to test and cast events.
     *
     * Example:
     *
     *      if (event->is<MouseEvent>())
     *      {
     *          // handle mouse event
     *      }
     *      else if (event->is<WindowEvent>())
     *      {
     *          auto e = event->to<WindowEvent>();
     *          // handle window event
     *      }
     */
    class Event
    {
    public:
        template<typename T>
        static constexpr bool isEventType = std::is_base_of_v<Event, T>;

        template<typename T>
        static constexpr bool isDecayed = std::is_same_v<std::decay_t<T>, T>;

        Event() noexcept = default;
        Event(const Event&) = default;
        Event(Event&&) noexcept = default;
        virtual ~Event() = default;

        Event& operator=(const Event&) = default;
        Event& operator=(Event&&) noexcept = default;

        /**
         * @brief Check whether the event is of a specific type
         *
         * @tparam T The event type to check. Must be derived from Event. Specify
         *           the basic type, no pointer, const or reference type.
         *
         * @return True if the event is of the specified type, false otherwise
         */
        template<class T> [[nodiscard]]
        bool is() noexcept {
            static_assert(isEventType<T>, "glb::Event::is<> template parameter must be derived from glb::Event");
            static_assert(isDecayed<T>, "glb::Event::is<> template parameter must be a decayed type");

            return dynamic_cast<T*>(this) != nullptr;
        }

        /**
         * @brief Check whether the event is of a specific type
         *
         * @tparam T The event type to check. Must be derived from Event. Specify
         *           the basic type, no pointer, const or reference type.
         *
         * @return True if the event is of the specified type, false otherwise
         */
        template<class T> [[nodiscard]]
        bool is() const noexcept {
            static_assert(isEventType<T>, "glb::Event::is<> template parameter must be derived from glb::Event");
            static_assert(isDecayed<T>, "glb::Event::is<> template parameter must be a decayed type");

            return dynamic_cast<const T*>(this) != nullptr;
        }

        /**
         * @brief dynamic_cast the event to another event type
         *
         * @tparam The event type to cast to.
         *
         * @return A pointer if the cast was successful, nullptr otherwise
         */
        template<class T> [[nodiscard]]
        T* to() noexcept {
            static_assert(isEventType<T>, "glb::Event::to<> template parameter must be derived from glb::Event");
            static_assert(isDecayed<T>, "glb::Event::to<> template parameter must be a decayed type");

            return dynamic_cast<T*>(this);
        }

        /**
         * @brief dynamic_cast the event to another event type
         *
         * @tparam The event type to cast to.
         *
         * @return A pointer if the cast was successful, nullptr otherwise
         */
        template<class T> [[nodiscard]]
        T* to() const noexcept {
            static_assert(isEventType<T>, "glb::Event::to<> template parameter must be derived from glb::Event");
            static_assert(isDecayed<T>, "glb::Event::to<> template parameter must be a decayed type");

            return dynamic_cast<const T*>(this);
        }
    };
} // namespace glb

#endif
