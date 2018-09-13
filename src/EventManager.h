#pragma once

#include <Ogre.h>
#include "EntityManager.h"
#include "ComponentManager.h"

#include <unordered_map>
#include <unordered_set>

namespace ECS
{
    template <typename Component>
    class EventHandler
    {
    	typedef void (Component::* Fptr)(void);
	friend class EventManager;

    public:
    	EventHandler(Component* component, Fptr function)
            : comp(component)
            , func(function)
        {}
            
        void connect();
        void connect(EntityId id);

        void disconnect();
        void disconnect(EntityId id);

    private:
        template <class ...Types>
        void execute(Types... args)
        {
       	    (comp->*func)(args...);
       	}

        Component* comp;
    	Fptr func;
    };    

    class EventManager
    {
    public:
        static EventManager* getSingleton()
        {
            if (!mInstance)
            {
                mInstance = new EventManager();
            }

            return mInstance;
        }

	    void update();

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

        static EventManager* mInstance;
        std::unordered_map<EntityId, std::vector<EventHandler<Component>>> mEntityToHandlers;
    };
}
