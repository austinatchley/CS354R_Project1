#pragma once

#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreRTShaderSystem.h>
#include <OgreApplicationContext.h>

#include <iostream>

#include "EventManager.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "GameObject.h"
#include "Ball.h"

using namespace Ogre;
using namespace OgreBites;

namespace Game
{
    class BasicTutorial1
            : public ApplicationContext
            , public InputListener
    {
    public:
        BasicTutorial1();
        virtual ~BasicTutorial1() {}

        void setup();
        bool keyPressed(const KeyboardEvent& evt);

    private:
        ECS::EventManager* mEventManager;
    };

    struct TestEvent
    {
        int i;
        char c;
    };

    class TestEventSubscriber
        : public ECS::EventSubscriber<TestEvent>
    {
    public:
        virtual ~TestEventSubscriber() {}

        virtual void receive(ECS::EventManager* eventManager, const TestEvent& event) override
        {
            std::cout << "TestEvent was emitted with i = " << event.i << ", c = " << event.c << " " << std::endl;
            event.test();
        }
    };
} //namespace Game
