#include "EventManager.h"

namespace ECS
{
    /////////////////////////////////////
    void EventManager::connect(EventHandler handler, ComponentId componentId)
    {
        if (mHandlerToComponents.find(handler) != mHandlerToComponents.end())
        {
            auto& componentVec = mHandlerToComponents.at(handler);
            componentVec.emplace_back(componentId);
        }
        else
        {
            mHandlerToComponents.emplace(std::vector<ComponentId>(1, componentId));
        }
    }

    void EventManager::disconnect(EventHandler handler, ComponentId componentId)
    {
        if (mHandlerToComponents.find(handler) != mHandlerToComponents.end())
        {
            auto& componentVec = mHandlerToComponents.at(handler);
            
            const auto& it = componentVec.begin();
            while (it != componentVec.end())
            {
                if (*it == componentId)
                {
                    componentVec.erase(it);
                    break;
                }
            }
        }
    }

    /////////////////////////////////////
    EventManager::EventManager()
    {
    }

    EventManager::~EventManager()
    {
    }

    // This should look at the queue of requested events and dispatch them
    void update()
    {
    }

    template <typename Event, class ...Types>
    void EventManager::event(EntityId id, Event event, Types&&... args)
    {
        /*auto& entityToHandlers = getSingleton()->mEntityToHandlers;
        auto& it = entityToHandlers.find(id);

        if (it != entityToHandlers.end())
        {
            auto& set = it->second;

            for (auto handler = set.begin(); handler != set.end(); ++handler)
            {
                (*event)(args...);
            }
        }*/
    }
}
