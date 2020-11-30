#include "event/EventHandler.h"



void glb::EventThread::start()
{
    terminate();
    shouldStop = false;

    thread = std::thread([]() {
        while (!shouldStop)
        {
            using namespace std::chrono;
            std::this_thread::sleep_for(1ns);

            // Don't use range-based for-loop here because iterators
            // are not thread safe. Declare size independently because
            // it silences the clangtidy modernization warning
            const size_t size = handlers.size();
            for (size_t i = 0; i < size; i++)
            {
                EventThread::handlers[i]();
            }
        }
    });
}

void glb::EventThread::terminate()
{
    shouldStop = true;
    if (thread.joinable()) {
        thread.join();
    }
}

void glb::EventThread::registerHandler(std::function<void()> pollFunc)
{
    std::lock_guard lock(handlerListLock);
    EventThread::handlers.push_back(std::move(pollFunc));
}
