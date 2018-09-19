#include "EventManager.h"

namespace ECS
{
    BaseEvent::~BaseEvent()
    {
    }

    /////////////////////////////////////
    EventManager::EventManager(Allocator alloc)
		: mEntAlloc(alloc)
		, mEntities({}, EntityPtrAllocator(alloc))
		, mSubscribers({}, 0, std::hash<TypeIndex>(), std::equal_to<TypeIndex>(), SubscriberPtrAllocator(alloc))
    {
    }

    EventManager::~EventManager()
    {
    }

    // This should look at the queue of requested events and dispatch them
    void EventManager::update()
    {
        if (mEvents.empty())
        {
            return;
        }

        for (auto event : mEvents)
        {
            event();
        }

        mEvents.clear();
    }
}
