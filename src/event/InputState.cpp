#include "event/InputState.h"

#include "event/EventHandler.h"
#include "event/InputEvents.h"



void glb::Keyboard::init()
{
    EventHandler<KeyPressEvent>::addListener([](const auto& e) {
        states[static_cast<size_t>(e.key)] = InputAction::press;
    });

    EventHandler<KeyReleaseEvent>::addListener([](const auto& e) {
        states[static_cast<size_t>(e.key)] = InputAction::release;
    });
}

bool glb::Keyboard::isPressed(Key key)
{
    return states[static_cast<size_t>(key)] == InputAction::press;
}

bool glb::Keyboard::isReleased(Key key)
{
    return states[static_cast<size_t>(key)] == InputAction::release;
}

auto glb::Keyboard::getState(Key key) -> InputAction
{
    return states[static_cast<size_t>(key)];
}



void glb::Mouse::init()
{
    EventHandler<MouseClickEvent>::addListener([](const auto& e) {
        states[static_cast<size_t>(e.button)] = InputAction::press;
    });

    EventHandler<MouseReleaseEvent>::addListener([](const auto& e) {
        states[static_cast<size_t>(e.button)] = InputAction::release;
    });

    EventHandler<MouseMoveEvent>::addListener([](const auto& e) {
        mousePos = { e.x, e.y };
    });
}

bool glb::Mouse::isPressed(MouseButton button)
{
    return states[static_cast<size_t>(button)] == InputAction::press;
}

bool glb::Mouse::isReleased(MouseButton button)
{
    return states[static_cast<size_t>(button)] == InputAction::release;
}

auto glb::Mouse::getState(MouseButton button) -> InputAction
{
    return states[static_cast<size_t>(button)];
}

auto glb::Mouse::getPosition() -> glm::vec2
{
    return mousePos;
}
