#ifndef LISTENER_H
#define LISTENER_H

#include <memory>

#include "Event.h"
#include "EventHandler.h"

namespace glb
{
    /* +++ Listener +++
    Event listener class. Inherit from this and implement onEvent() to receive events.
    The Listener()-constructor must be called in the subclass's constructor. */
    class Listener
    {
    public:
        Listener() {
            EventHandler::add(*this);
        }
        Listener(const Listener&) = default;
        Listener(Listener&&) noexcept = default;
        Listener& operator=(const Listener&) = default;
        Listener& operator=(Listener&&) noexcept = default;
        virtual ~Listener() {
            EventHandler::remove(*this);
        }

        /* +++ onEvent() +++
        Gets called by EventHandler every time an event occurs.
        Implemented by subclasses to handle events. */
        virtual void onEvent(std::shared_ptr<Event> e) = 0;
    };
}

#endif