#include "EventManager.h"

namespace ECS
{
    /////////////////////////////////////
    EventManager::EventManager()
    {
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
            auto& set = it->second;

            for (auto handler = set.begin(); handler != set.end(); ++handler)
            {
                (*event)(args...);
            }
        }
    }
}
