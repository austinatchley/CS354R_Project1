#pragma once

#include <Ogre.h>

#include "EntityManager.h"
#include "ComponentManager.h"

#include <unordered_map>
#include <unordered_set>

namespace ECS
{
    class EventHandler
    {
    	typedef void (Component::* Fptr)(void);
	    friend class EventManager;

    public:
    	EventHandler(Component* component, Fptr& callback)
            : mComponent(component)
            , mCallback(callback)
        {}
            
    private:
        template <class ...Types>
        void execute(Types... args)
        {
       	    (mComponent->*mCallback)(args...);
       	}

        Component* mComponent;
    	Fptr mCallback;
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

        void connect(EventHandler handler, ComponentId componentId);
        void disconnect(EventHandler handler, ComponentId componentId);

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
        std::unordered_map<EventHandler, std::vector<ComponentId>> mHandlerToComponents;
    };
}
