#pragma once
#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <memory>
#include <vector>
#include <queue>
#include <mutex>

#include "Event.h"
#include "InputEvents.h"

namespace glb
{
    class Listener;

    class EventHandler
    {
    public:
        static void init();

        static void notify(std::unique_ptr<Event> e);

        static void add(Listener& l);
        static void remove(Listener& l);

    private:
        static void run();

        static inline std::mutex listenerLock;
        static inline std::queue<Event*> pendingEvents;
        static inline std::vector<Listener*> listeners;
    };
} // namespace glb

#endif