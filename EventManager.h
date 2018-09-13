#pragma once

#include <Ogre.h>
#include <EntityManager.h>

namespace ECS
{
    class EventManager
    {
    public:
        EventManager();
        ~EventManager();

        EventManager* getEventManager() const;

    protected:
        class Handler
        {
            void connect();
            void connect(EntityId id);

            void disconnect();
        }

    private:
        static EventManager* mEventManager;
    };
}
