#include "EventManager.h"

namespace ECS
{
    EventManager::EventManager()
    {
        if (!mEventManager)
        {
            mEventManager = this;
        }
    }

    EventManager::~EventManager()
    {
    }

    template <typename Event, class ...Types>
    void EventManager::event(EntityId id, Event event, Types&&... args)
    {
        auto& entityToHandlers = getSingleton()->mEntityToHandlers;
        auto& it = entityToHandlers.find(id);

        if (it != entityToHandlers.end())
        {
            auto& set = *it;

            for (auto handler : set)
            {
                handler->(*event)(args...);
            }
        }
    }
}
