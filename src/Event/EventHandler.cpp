#include "EventHandler.h"

#include <thread>

#include "Listener.h"



void glb::EventHandler::init()
{
	static bool initialized = false;
	if (initialized) return;
	initialized = true;

	std::thread notify_thread = std::thread(&EventHandler::run);
	notify_thread.detach();
}

void glb::EventHandler::notify(std::unique_ptr<Event> e)
{
	pendingEvents.push(e.release());
}

void glb::EventHandler::add(Listener& l)
{
	listeners.push_back(&l);
}

void glb::EventHandler::remove(Listener& l)
{
	for (size_t i = 0; i < listeners.size(); i++)
	{
		if (listeners[i] == &l)
		{
			listenerLock.lock();
			listeners.erase(listeners.begin() + i);
			listenerLock.unlock();
			break;
		}
	}
}

void glb::EventHandler::run()
{
	while (true)
	{
		if (!pendingEvents.empty())
		{
			auto e = std::shared_ptr<Event> (pendingEvents.front());
			pendingEvents.pop();

			listenerLock.lock();
			for (auto listener : listeners)
			{
				listener->onEvent(e);
			}
			listenerLock.unlock();
		}
	}
}
