#pragma once
#ifndef KEYDEFINITIONS_H
#define KEYDEFINITIONS_H

#include <GL/glew.h> // Include glew.h before gl.h (which is included in glfw.h)
#include <GLFW/glfw3.h>

namespace glb
{
    /**
     * These have the same numeric value as the corresponding GLFW defines.
     */
    enum class eKey {
        a = GLFW_KEY_A,
        b = GLFW_KEY_B,
        c = GLFW_KEY_C,
        d = GLFW_KEY_D,
        e = GLFW_KEY_E,
        f = GLFW_KEY_F,
        g = GLFW_KEY_G,
        h = GLFW_KEY_H,
        i = GLFW_KEY_I,
        j = GLFW_KEY_J,
        k = GLFW_KEY_K,
        l = GLFW_KEY_L,
        m = GLFW_KEY_M,
        n = GLFW_KEY_N,
        o = GLFW_KEY_O,
        p = GLFW_KEY_P,
        q = GLFW_KEY_Q,
        r = GLFW_KEY_R,
        s = GLFW_KEY_S,
        t = GLFW_KEY_T,
        u = GLFW_KEY_U,
        v = GLFW_KEY_V,
        w = GLFW_KEY_W,
        x = GLFW_KEY_X,
        y = GLFW_KEY_Y,
        z = GLFW_KEY_Z,

        _0 = GLFW_KEY_0,
        _1 = GLFW_KEY_1,
        _2 = GLFW_KEY_2,
        _3 = GLFW_KEY_3,
        _4 = GLFW_KEY_4,
        _5 = GLFW_KEY_5,
        _6 = GLFW_KEY_6,
        _7 = GLFW_KEY_7,
        _8 = GLFW_KEY_8,
        _9 = GLFW_KEY_9,

        f1 = GLFW_KEY_F1,
        f2 = GLFW_KEY_F2,
        f3 = GLFW_KEY_F3,
        f4 = GLFW_KEY_F4,
        f5 = GLFW_KEY_F5,
        f6 = GLFW_KEY_F6,
        f7 = GLFW_KEY_F7,
        f8 = GLFW_KEY_F8,
        f9 = GLFW_KEY_F9,
        f10 = GLFW_KEY_F10,
        f11 = GLFW_KEY_F11,
        f12 = GLFW_KEY_F12,

        left_shift	= GLFW_KEY_LEFT_SHIFT,
        right_shift	= GLFW_KEY_RIGHT_SHIFT,
        left_alt	= GLFW_KEY_LEFT_ALT,
        right_alt	= GLFW_KEY_RIGHT_ALT,
        left_ctrl	= GLFW_KEY_LEFT_CONTROL,
        right_ctrl	= GLFW_KEY_RIGHT_CONTROL,

        tab			= GLFW_KEY_TAB,
        backspace	= GLFW_KEY_BACKSPACE,
        capslock	= GLFW_KEY_CAPS_LOCK,
        escape		= GLFW_KEY_ESCAPE,

        UNKNOWN_KEY
    };

    /**
     * These have the same numeric value as the corresponding GLFW defines.
     *
     * This is not an enum class because mods have to be combineable
     */
    enum eKeyMod {
        shift = GLFW_MOD_SHIFT,
        control = GLFW_MOD_CONTROL,
        alt = GLFW_MOD_ALT,
        super = GLFW_MOD_SUPER,
        MAX_ENUM
    };

    /**
     * These have the same numeric value as the corresponding GLFW defines.
     */
    enum class eMouseButton {
        left = GLFW_MOUSE_BUTTON_LEFT,
        right = GLFW_MOUSE_BUTTON_RIGHT,
        wheel = GLFW_MOUSE_BUTTON_MIDDLE,
        middle = wheel,
        button_4 = GLFW_MOUSE_BUTTON_4,
        button_5,
        button_6,
        button_7,
        button_8,
        MAX_ENUM
    };

    /**
     * These have the same numeric value as the corresponding GLFW defines.
     */
    enum class eInputAction {
        release = GLFW_RELEASE,
        press = GLFW_PRESS,
        repeat = GLFW_REPEAT,
        MAX_ENUM
    };
} // namespace glb

#endif
