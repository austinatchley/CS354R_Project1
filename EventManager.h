#pragma once

#include <Ogre.h>
#include <EntityManager.h>
#include <ComponentManager.h>

#include <unordered_map>
#include <unordered_set>

namespace ECS
{
    class EventHandlerBase
    {
        //template <class ...Types>
        //virtual void execute(Types&&... args);
    };
    
    template <typename Class>
    class EventHandler
        : public EventHandlerBase
        {
            typedef void (Class::* Fptr)(void);

        public:
            EventHandler(Class* object, Fptr function)
                : obj(object)
                , func(function)
            {}
            
            void connect();
            void connect(EntityId id);

            void disconnect();
            void disconnect(EntityId id);

            template <class ...Types>
            void execute(Types... args)
            {
                (obj->*func)(args...);
            }

        private:
            Class* obj;
            Fptr func;
        };

    class EventManager
    {
    public:
        static EventManager* getSingleton()
        {
            if (!mEventManager)
            {
                mEventManager = new EventManager();
            }

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

    private:
        EventManager();
        ~EventManager();

        static EventManager* mEventManager;
        std::unordered_map<EntityId, std::vector<EventHandler<Component>>> mEntityToHandlers;
    };
}
