#pragma once

#include <Ogre.h>
#include <EntityManager.h>

#include <unordered_map>
#include <unordered_set>

namespace ECS
{
    class EventManager
    {
    public:
        EventManager();
        ~EventManager();

        static EventManager* getSingleton()
        {
            return mEventManager;
        }

        template <typename Event, class ...Types>
        void event(EntityId id, Event event, Types&&... args);

        template <typename Event, class ...Types>
        void broadcast(Event event, Types&&... args);

        template <typename Result, typename Event, class ...Types>
        void eventResult(Result* res, EntityId id, Event event, Types&&... args);
        
        template <typename Result, typename Event, class ...Types>
        void broadcastResult(Result* res, Event event, Types&&... args);

    protected:
        class Handler
        {
            void connect();
            void connect(EntityId id);

            void disconnect();
            void disconnect(EntityId id);
        }

        static EventManager* mEventManager = nullptr;
        std::unordered_map<EntityId, std::unordered_set<Handler>> mEntityToHandlers;
    };
}
