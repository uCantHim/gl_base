#pragma once

#include "Keys.h"

namespace glb
{
    ///////////////////////////
    //      Key Events       //
    ///////////////////////////

    struct KeyEventBase
    {
        Key key;
        int mods;
        InputAction action;

    protected:
        KeyEventBase(Key key, int mod, InputAction action)
            : key(key), mods(mod), action(action) {}
    };

    struct KeyPressEvent : public KeyEventBase
    {
        KeyPressEvent(Key key, int mod)
            : KeyEventBase(key, mod, InputAction::press) {}

    };

    struct KeyReleaseEvent : public KeyEventBase
    {
        KeyReleaseEvent(Key key, int mod)
            : KeyEventBase(key, mod, InputAction::release) {}
    };

    struct KeyRepeatEvent : public KeyEventBase
    {
        KeyRepeatEvent(Key key, int mod)
            : KeyEventBase(key, mod, InputAction::repeat) {}
    };


    struct CharInputEvent
    {
        uint32_t character;
    };


    /////////////////////////////
    //      Mouse Events       //
    /////////////////////////////

    struct MouseMoveEvent
    {
        float x;
        float y;
    };

    struct MouseButtonEventBase
    {
        MouseButton button;
        int mods;
        InputAction action;

    protected:
        MouseButtonEventBase(MouseButton button, int mods, InputAction action)
            : button(button), mods(mods), action(action) {}
    };

    struct MouseClickEvent : public MouseButtonEventBase
    {
        MouseClickEvent(MouseButton button, int mods)
            : MouseButtonEventBase(button, mods, InputAction::press) {}
    };

    struct MouseReleaseEvent : public MouseButtonEventBase
    {
        MouseReleaseEvent(MouseButton button, int mods)
            : MouseButtonEventBase(button, mods, InputAction::release) {}
    };


    /////////////////////////////
    //      Other Events       //
    /////////////////////////////

    struct ScrollEvent
    {
        float xOffset;
        float yOffset;
    };
}
