#pragma once
#ifndef INPUTEVENTS_H
#define INPUTEVENTS_H

#include <glm/glm.hpp>
using namespace glm;

#include "Event.h"
#include "KeyDefinitions.h"

namespace glb
{
    /* +++ InputEvent +++ */
    class InputEvent : public Event
    {
    };

    /* +++ KeyEvent +++ */
    class KeyEvent : public InputEvent
    {
    protected:
        KeyEvent(eKey key, eKeyMod mods);

    public:
        KeyEvent(const KeyEvent&) = default;
        KeyEvent(KeyEvent&&) noexcept = default;
        ~KeyEvent() override = default;

        KeyEvent& operator=(KeyEvent&&) noexcept = delete;
        KeyEvent& operator=(const KeyEvent&) = delete;

        const eKey key;
        const eKeyMod mods;
    };

    class KeyPressEvent : public KeyEvent
    {
    public:
        KeyPressEvent(eKey key, eKeyMod mods)
            : KeyEvent(key, mods) {}
    };

    class KeyReleaseEvent : public KeyEvent
    {
    public:
        KeyReleaseEvent(eKey key, eKeyMod mods)
            : KeyEvent(key, mods) {}
    };


    /* +++ MouseEvent +++ */
    class MouseEvent : public InputEvent
    {
    public:
        explicit MouseEvent(vec2 cursorPosition = cursorPos);
        MouseEvent(const MouseEvent&) = default;
        MouseEvent(MouseEvent&&) noexcept = default;
        ~MouseEvent() override = default;

        MouseEvent& operator=(const MouseEvent&) = default;
        MouseEvent& operator=(MouseEvent&&) noexcept = default;

        static inline vec2 cursorPos;
    };


    /* +++ MouseButtonEvent +++ */
    class MouseButtonEvent : public MouseEvent
    {
    protected:
        MouseButtonEvent(eMouseButton button, eKeyMod mods);

    public:
        const eMouseButton button;
        const eKeyMod mods;
    };

    class MouseButtonPressEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressEvent(eMouseButton button, eKeyMod mods)
            : MouseButtonEvent(button, mods) {}
    };

    class MouseButtonReleaseEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleaseEvent(eMouseButton button, eKeyMod mods)
            : MouseButtonEvent(button, mods) {}
    };


    /* +++ MouseMoveEvent +++ */
    class MouseMoveEvent : public MouseEvent
    {
    public:
        explicit MouseMoveEvent(vec2 cursorPosition)
            : MouseEvent(cursorPosition) {}
    };
} // namespace glb

#endif
